#pragma once

#include <rapidjson/prettywriter.h>

#include "RapidjsonField.h"
#include "confparse/Configuration/IConfiguration.h"

namespace cp {
    class RapidjsonConfiguration : public IConfiguration<RapidjsonConfiguration> {
    private:
        friend struct IConfiguration<RapidjsonConfiguration>;

    private:
        rapidjson::Document json;

    private:
        Status parseImpl(std::string_view jsonString) noexcept;

        SerializedConf serializeImpl() noexcept;

        RapidjsonField getFoldObjectImpl(std::string_view fieldName) noexcept;

        [[nodiscard]] bool containsImpl(std::string_view fieldName) const noexcept;
    };
}