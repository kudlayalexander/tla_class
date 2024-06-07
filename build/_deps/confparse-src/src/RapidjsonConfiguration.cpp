#include "confparse/Configuration/Rapidjson/RapidjsonConfiguration.h"

namespace cp {
    Status RapidjsonConfiguration::parseImpl(std::string_view jsonString) noexcept {
        json.Parse(jsonString.data());
        return (json.Parse(jsonString.data()).HasParseError()) ?
               newError(RetCode::DESERIALIZE_ERROR) :
               Status();
    }

    SerializedConf RapidjsonConfiguration::serializeImpl() noexcept {
        using namespace rapidjson;
        StringBuffer sb;

        PrettyWriter<StringBuffer> writer(sb);
        writer.SetIndent(' ', 2);
        if (!json.Accept(writer)) {
            return newError(RetCode::SERIALIZE_ERROR);
        }

        return std::string{sb.GetString(), sb.GetSize()};
    }

    RapidjsonField RapidjsonConfiguration::getFoldObjectImpl(std::string_view fieldName) noexcept {
        auto &ref = json[rapidjson::Value(fieldName.data(), fieldName.size())];
        return RapidjsonField(ref, json.GetAllocator());
    }

    [[nodiscard]] bool RapidjsonConfiguration::containsImpl(std::string_view fieldName) const noexcept {
        return json.HasMember(rapidjson::Value(fieldName.data(), fieldName.size()));
    }
}