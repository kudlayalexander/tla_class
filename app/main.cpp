#include "Startup.h"
#include "config/JsonConfigManager.h"

int main() noexcept {
    static constexpr std::string_view kModuleName = "MainModule";
    return boost::leaf::try_handle_all(
            []() -> boost::leaf::result<int> {
                static constexpr std::string_view kConfigFilePath = "/etc/bch/config.json";
                config::JsonConfigManager jsonConfigManager(kConfigFilePath);
                config::ConfigManagerObsPtr jsonConfigManagerPtr(&jsonConfigManager);
                BOOST_LEAF_CHECK(utils::file::parseConfiguration(jsonConfigManagerPtr));

                Startup startup;
                BOOST_LEAF_CHECK(startup.run(jsonConfigManagerPtr));

                return EXIT_SUCCESS;
            },
            [](boost::leaf::match<RetCode, isRomConfigurationError> e, std::string_view reason) {
                const auto rcAsInt = int(e.matched);
                LOGGER_ERROR(kModuleName,
                             "configuration error: '{:s}' (code: {:d}), reason: {:s}",
                             rcToString(e.matched), rcAsInt, reason);
                return rcAsInt;
            },
            [](RetCode rc, std::string_view reason) {
                const auto rcAsInt = int(rc);
                LOGGER_ERROR(kModuleName,
                             "caught unhandled error: {:s} (code: {:d}), reason: {:s}",
                             rcToString(rc), rcAsInt, reason);
                return rcAsInt;
            },
            [](RetCode rc) {
                const auto rcAsInt = int(rc);
                LOGGER_ERROR(kModuleName,
                             "caught unhandled error: {:s} (code: {:d})",
                             rcToString(rc), rcAsInt);
                return rcAsInt;
            },
            [](const std::exception &e) {
                LOGGER_ERROR(kModuleName, "caught unhandled exception: {:s}", e.what());
                return EXIT_FAILURE;
            },
            []() {
                LOGGER_ERROR(kModuleName, "caught unhandled exception/error");
                return EXIT_FAILURE;
            }
    );
}
