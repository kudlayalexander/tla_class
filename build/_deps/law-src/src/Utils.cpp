#include <array>
#include <sys/ioctl.h>
#include <boost/tokenizer.hpp>

#include <ErrorCode/ErrorCode.h>

#include "law/Utils.h"
#include "law/RetCode/RetCode.h"

namespace law::utils {
    BlobViewRet getHostname() noexcept {
        static std::array<char, 254> hostname{""};
        if ((hostname[0] == '\0') && (gethostname(hostname.data(), hostname.size()) == -1)) {
            return fromErrno();
        }
        return std::string_view(hostname.data(), hostname.size());
    }

    bool validateDNS(std::string_view str) noexcept {
        static constexpr std::uint8_t RFC2181_MAX_LABEL_SIZE = 63;
        static constexpr std::uint8_t RFC3986_MAX_DNS_LENGTH = 255;

        if (str.empty()) {
            return false;
        }

        if (str.size() > RFC3986_MAX_DNS_LENGTH) {
            return false;
        }

        using Separator = boost::char_separator<char>;
        Separator sep(".");
        return std::ranges::all_of(boost::tokenizer<Separator>(str, sep),
                                   [](const auto &it) {
                                       return (!it.empty()) && (it.size() <= RFC2181_MAX_LABEL_SIZE);
                                   });
    }

    AvailableDataSize getAvailableBytesForRead(const fd::FileDescriptor &fd) noexcept {
        return getAvailableBytesForRead(fd.getNativeHandler());
    }

    AvailableDataSize getAvailableBytesForRead(int fd) noexcept {
        int value;
        if (ioctl(fd, FIONREAD, &value) == -1) {
            return fromErrno();
        }

        return value;
    }
}
