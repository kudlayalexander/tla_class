#include <ctre/ctre.hpp>
#include <ctre/ctll/fixed_string.hpp>

#include <netdb.h>
#include <arpa/inet.h>

#include <Utils/Hash/CRC32/CRC32.h>

#include "law/Utils.h"
#include "law/RetCode/RetCode.h"
#include "law/Socket/Net/InternetAddress/detail/IpResolver.h"

namespace law::socket::net::detail {
    ResolvedIP IpResolver::resolve(std::string_view address) noexcept {
        return boost::leaf::try_handle_some(
                [address]() -> ResolvedIP {
                    if (isLocalhost(address)) {
                        return in_addr{htonl(INADDR_LOOPBACK)};
                    }

                    if (isHostname(address)) {
                        const auto domain = extractDomain(address);
                        if (!utils::validateDNS(domain)) {
                            return boost::leaf::new_error(RetCode::INCORRECT_HOSTNAME);
                        }

                        return hostnameToIp(extractDomain(address));
                    }

                    return in_addr{inet_addr(address.data())};
                }
        );
    }

    bool IpResolver::isHostname(std::string_view address) noexcept {
        static constexpr auto isIpRegex = ctll::fixed_string(R"(^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.?\b){4}$)");
        return !ctre::match<isIpRegex>(address);
    }

    bool IpResolver::isLocalhost(std::string_view hostname) noexcept {
        static constexpr auto localhostStrHash = COMPILE_TIME_CRC32_STR("localhost");
        static constexpr auto localhostIpStrHash = COMPILE_TIME_CRC32_STR("127.0.0.1");
        const auto hostnameHash = Utils::CRC32(hostname.data()).get();
        return (hostnameHash == localhostStrHash) || (hostnameHash == localhostIpStrHash);
    }

    std::string_view IpResolver::extractDomain(std::string_view url) {
        static constexpr auto domainExtractorRegex = ctll::fixed_string(R"(^(?:[^:\n]+://)?([^:#/\n]*))");
        return ctre::match<domainExtractorRegex>(url);
    }

    ResolvedIP IpResolver::hostnameToIp(std::string_view hostname) noexcept {
        if (auto he = gethostbyname(hostname.data()); he) {
            auto addressList = reinterpret_cast<in_addr **>(he->h_addr_list);
            if (!addressList) {
                return boost::leaf::new_error(RetCode::INCORRECT_HOSTNAME);
            }
            return *addressList[0];
        } else {
            return boost::leaf::new_error(RetCode::INVALID_HOSTNAME_RESOLUTION);
        }
    }
}
