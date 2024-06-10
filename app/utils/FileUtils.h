#pragma once

#include <string_view>

#include "../config/IConfigManager.h"

namespace utils::file {
    struct DelimPath {
        std::string_view dir;
        std::string_view path;
    };

    DelimPath delimFilePathOnDirAndName(std::string_view path) noexcept {
        auto basenameBeginPos = path.find_last_of('/');
        if (basenameBeginPos == std::string_view::npos) {
            basenameBeginPos = 0;
        }

        return {path.substr(0, basenameBeginPos), path.substr(basenameBeginPos + 1)};
    }

    Status parseConfiguration(config::ConfigManagerObsPtr configManager) noexcept {
        static constexpr std::string_view moduleName = "parseConfiguration";
        return boost::leaf::try_handle_some(
                [configManager]() -> Status {
                    BOOST_LEAF_CHECK(configManager->load());
                    BOOST_LEAF_CHECK(configManager->apply());
                    return {};
                },
                [configManager](
                        boost::leaf::match<
                                RetCode,
                                RetCode::ROM_CONFIGURATION_CORRUPT,
                                RetCode::ROM_CONFIGURATION_LOAD_ERROR,
                                RetCode::ROM_CONFIGURATION_SERIALIZE_ERROR> e) -> Status {
                    LOGGER_ERROR(moduleName,
                                 "configuration error: '{:s}' (code: {:d}). Restore default configuration...",
                                 rcToString(e.matched), std::uint8_t(e.matched));
                    BOOST_LEAF_CHECK(configManager->restore());
                    return {};
                }
        );
    }
}