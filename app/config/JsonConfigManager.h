#pragma once

#include <ranges>
#include <concepts>

#include <law/File/File.h>
#include <boost/assert.hpp>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <Utils/Hash/CRC32/CRC32.h>
#include <magic_enum/magic_enum.hpp>
#include <static_string/static_string.h>

#include "IConfigManager.h"
#include "utils/FileUtils.h"
#include "utils/Utils.h"

#define THROW_ERROR(err, reason)                                                            \
    return newError(                                                                        \
            err,                                                                            \
            std::string_view(reason.data.data(), reason.data.size()));

#define CHECK_STRING_FIELD_LENGTH(field, type, fieldName)                                   \
    do {                                                                                    \
        if (field.GetStringLength() > type::MAX_SIZE) {                                     \
            using namespace snw1;                                                           \
            constexpr auto kErrorMessage = static_string::make(                             \
                "field with name '",                                                        \
                fieldName,                                                                  \
                "' has incorrect size: max size (",                                         \
                UTOSS(type::MAX_SIZE),                                                      \
                ")"                                                                         \
            );                                                                              \
            THROW_ERROR(                                                                    \
                    RetCode::ROM_CONFIGURATION_STRING_FIELD_LENGTH_TOO_BIG,                 \
                    kErrorMessage                                                           \
            );                                                                              \
        }                                                                                   \
    } while (false)

#define CHECK_FIELD_VALUE_RANGE_SIGNED_INTEGRAL(value, minValue, maxValue, fieldName)       \
    do {                                                                                    \
        if (value < minValue || value > maxValue) {                                         \
            using namespace snw1;                                                           \
            constexpr auto kErrorMessage = static_string::make(                             \
                    "field with name '",                                                    \
                    fieldName,                                                              \
                    "' value not in range (expected: ",                                     \
                    ITOSS(minValue),                                                        \
                    " <= val <= ",                                                          \
                    ITOSS(maxValue),                                                        \
                    ")"                                                                     \
            );                                                                              \
            THROW_ERROR(                                                                    \
                RetCode::ROM_CONFIGURATION_FIELD_VALUE_RANGE_VIOLATION,                     \
                kErrorMessage                                                               \
            );                                                                              \
        }                                                                                   \
    } while (false)

#define CHECK_FIELD_VALUE_RANGE_UNSIGNED_INTEGRAL(value, minValue, maxValue, fieldName)     \
    do {                                                                                    \
        if (value < minValue || value > maxValue) {                                         \
            using namespace snw1;                                                           \
            constexpr auto kErrorMessage = static_string::make(                             \
                "field with name '",                                                        \
                fieldName,                                                                  \
                "' value not in range (expected: ",                                         \
                UTOSS(minValue),                                                            \
                " <= val <= ",                                                              \
                UTOSS(maxValue),                                                            \
                ")"                                                                         \
            );                                                                              \
            THROW_ERROR(                                                                    \
                RetCode::ROM_CONFIGURATION_FIELD_VALUE_RANGE_VIOLATION,                     \
                kErrorMessage                                                               \
                );                                                                          \
        }                                                                                   \
    } while (false)

#define CHECK_FIELD_VALUE_RANGE_FP(value, minValue, maxValue, fieldName)                    \
    do {                                                                                    \
        if (value < minValue || value > maxValue) {                                         \
            using namespace snw1;                                                           \
            constexpr auto kErrorMessage = static_string::make(                             \
                "field with name '",                                                        \
                fieldName,                                                                  \
                "' value not in range"                                                      \
            );                                                                              \
            THROW_ERROR(                                                                    \
                RetCode::ROM_CONFIGURATION_FIELD_VALUE_RANGE_VIOLATION,                     \
                kErrorMessage                                                               \
            );                                                                              \
        }                                                                                   \
    } while (false)

#define CHECK_FIELD_VALUE_ALLOW_VALUES(value, allowValues, fieldName)                       \
    do {                                                                                    \
        if (std::ranges::find(allowValues, value) ==                                        \
                allowValues.cend()) {                                                       \
            using namespace std::literals;                                                  \
            static constexpr auto messagePartOne                                            \
                = "field with name '"sv;                                                    \
            static constexpr std::string_view fieldNameSv(fieldName);                       \
            static constexpr auto messagePartTwo                                            \
                = "' incorrect value, allow values: (expected: "sv;                         \
            static constexpr auto messagePartThree = ")"sv;                                 \
            static constexpr auto kErrorMessage =                                           \
                std::apply(                                                                 \
                    []<auto ...values>(auto ...) {                                          \
                        return utils::StrViewJoinValue<                                     \
                            messagePartOne,                                                 \
                            fieldNameSv,                                                    \
                            messagePartTwo,                                                 \
                            values...,                                                      \
                            messagePartThree                                                \
                        >;                                                                  \
                    },                                                                      \
                    std::tuple_cat(allowValues)                                             \
                );                                                                          \
            return newError(                                                                \
                RetCode::ROM_CONFIGURATION_UNEXPECTED_FIELD_VALUE,                          \
                std::string_view(kErrorMessage)                                             \
            );                                                                              \
        }                                                                                   \
    } while (false)

#define CHECK_FIELD_TYPE(method, f, fieldName, fieldValueTypename)                          \
    do {                                                                                    \
        if (!f.method()) {                                                                  \
            using namespace std::literals;                                                  \
            static constexpr auto messagePartOne                                            \
                = "field with name '"sv;                                                    \
            static constexpr std::string_view fieldNameSv(fieldName);                       \
            static constexpr auto messagePartTwo = "' has incorrect type (expected: "sv;    \
            static constexpr std::string_view fieldValueTypenameSv(fieldValueTypename);     \
            static constexpr auto messagePartThree = ")"sv;                                 \
            static constexpr auto kErrorMessage =                                           \
                utils::StrViewJoinValue<                                                    \
                    messagePartOne,                                                         \
                    fieldNameSv,                                                            \
                    messagePartTwo,                                                         \
                    fieldValueTypenameSv,                                                   \
                    messagePartThree                                                        \
                >;                                                                          \
            return newError(                                                                \
                RetCode::ROM_CONFIGURATION_INCORRECT_FIELD_TYPE,                            \
                std::string_view(kErrorMessage)                                             \
            );                                                                              \
        }                                                                                   \
    } while (false)

#define PARSE_FIELD(\
            fieldVariable, parentFieldVariable, \
            fieldName, fieldType, fieldValueTypename, fieldVariableType)                    \
    BOOST_LEAF_AUTO(&fieldVariable##Field, checkPresence(parentFieldVariable, fieldName));  \
    CHECK_FIELD_TYPE(Is##fieldType, fieldVariable##Field, fieldName, fieldValueTypename);   \
    fieldVariableType fieldVariable = fieldVariable##Field.Get##fieldType()

#define PARSE_BOOL_FIELD(fieldVariable, parentFieldVariable, fieldName)                     \
    PARSE_FIELD(fieldVariable, parentFieldVariable, fieldName, Bool, "bool", bool)

#define PARSE_STRING_FIELD(fieldVariable, parentFieldVariable, fieldName, targetFieldType)  \
    BOOST_LEAF_AUTO(&fieldVariable##Field, checkPresence(parentFieldVariable, fieldName));  \
    CHECK_FIELD_TYPE(IsString, fieldVariable##Field, fieldName, "string");                  \
    CHECK_STRING_FIELD_LENGTH(fieldVariable##Field, targetFieldType, fieldName);            \
    targetFieldType fieldVariable(                                                          \
        fieldVariable##Field.GetString(), fieldVariable##Field.GetStringLength())

#define PARSE_STRING_FIELD_CHECK_ALLOW_VALUES(\
            fieldVariable, parentFieldVariable, fieldName, targetFieldType, allowValues)    \
    PARSE_STRING_FIELD(fieldVariable, parentFieldVariable, fieldName, targetFieldType);     \
    CHECK_FIELD_VALUE_ALLOW_VALUES(                                                         \
        std::string_view(                                                                   \
            fieldVariable.data(), fieldVariable.size()), allowValues, fieldName)

#define PARSE_UINT_FIELD(fieldVariable, parentFieldVariable, fieldName)                     \
    PARSE_FIELD(fieldVariable, parentFieldVariable, fieldName, Uint, "uint", std::uint32_t)

#define PARSE_SIGNED_INTEGER_FIELD_CHECK_MIN_MAX(                                           \
            fieldVariable, parentFieldVariable,                                             \
            fieldName, minValue, maxValue, fieldType, fieldValueTypename)                   \
    PARSE_FIELD(                                                                            \
        fieldVariable, parentFieldVariable,                                                 \
        fieldName, fieldType, fieldValueTypename, decltype(maxValue));                      \
    CHECK_FIELD_VALUE_RANGE_SIGNED_INTEGRAL(fieldVariable, minValue, maxValue, fieldName)

#define PARSE_UNSIGNED_INTEGER_FIELD_CHECK_MIN_MAX(                                         \
            fieldVariable, parentFieldVariable,                                             \
            fieldName, minValue, maxValue, fieldType, fieldValueTypename)                   \
    PARSE_FIELD(                                                                            \
        fieldVariable, parentFieldVariable,                                                 \
        fieldName, fieldType, fieldValueTypename, decltype(maxValue));                      \
    CHECK_FIELD_VALUE_RANGE_UNSIGNED_INTEGRAL(fieldVariable, minValue, maxValue, fieldName)

#define PARSE_FP_FIELD_CHECK_MIN_MAX(\
            fieldVariable, parentFieldVariable, \
            fieldName, minValue, maxValue, fieldType, fieldValueTypename)                   \
    PARSE_FIELD(                                                                            \
        fieldVariable, parentFieldVariable,                                                 \
        fieldName, fieldType, fieldValueTypename, decltype(maxValue));                      \
    CHECK_FIELD_VALUE_RANGE_FP(fieldVariable, minValue, maxValue, fieldName)

#define PARSE_UINT_FIELD_CHECK_MIN_MAX(\
            fieldVariable, parentFieldVariable, fieldName, minValue, maxValue)              \
    PARSE_UNSIGNED_INTEGER_FIELD_CHECK_MIN_MAX(                                             \
        fieldVariable, parentFieldVariable, fieldName, minValue, maxValue, Uint, "uint")

#define PARSE_UINT64_FIELD_CHECK_MIN_MAX(\
            fieldVariable, parentFieldVariable, fieldName, minValue, maxValue)              \
    PARSE_UNSIGNED_INTEGER_FIELD_CHECK_MIN_MAX(                                             \
        fieldVariable, parentFieldVariable, fieldName, minValue, maxValue, Uint64, "uint64")

#define PARSE_INT_FIELD_CHECK_MIN_MAX(\
            fieldVariable, parentFieldVariable, fieldName, minValue, maxValue)              \
    PARSE_SIGNED_INTEGER_FIELD_CHECK_MIN_MAX(                                               \
        fieldVariable, parentFieldVariable, fieldName, minValue, maxValue, Int, "int")

#define PARSE_INT64_FIELD_CHECK_MIN_MAX(\
            fieldVariable, parentFieldVariable, fieldName, minValue, maxValue)              \
    PARSE_SIGNED_INTEGER_FIELD_CHECK_MIN_MAX(                                               \
        fieldVariable, parentFieldVariable, fieldName, minValue, maxValue, Int64, "int64")

#define PARSE_FLOAT_FIELD_CHECK_MIN_MAX(\
            fieldVariable, parentFieldVariable, fieldName, minValue, maxValue)              \
    PARSE_FP_FIELD_CHECK_MIN_MAX(                                                           \
        fieldVariable, parentFieldVariable, fieldName, minValue, maxValue, Float, "float")

#define PARSE_DOUBLE_FIELD_CHECK_MIN_MAX(\
            fieldVariable, parentFieldVariable, fieldName, minValue, maxValue)              \
    PARSE_FP_FIELD_CHECK_MIN_MAX(                                                           \
        fieldVariable, parentFieldVariable, fieldName, minValue, maxValue, Double, "double")

#define PARSE_OBJECT_FIELD(field, parentField, fieldName)                                   \
    BOOST_LEAF_AUTO(&field, checkPresence(parentField, fieldName));                         \
    CHECK_FIELD_TYPE(IsObject, field, fieldName, "object")

namespace config {
    class JsonConfigManager final : public IConfigManager {
    private:
        static constexpr std::string_view moduleName = "JsonConfigManager";

    private:
        using SerializedConf = CustomError<std::string>;
        using ValueRef = rapidjson::Value &;
        using ConstValueRef = const rapidjson::Value &;
        using ConstValueRefRet = CustomError<ConstValueRef>;

        using UdsUdpServerRet = CustomError<tp::UdsUdpServer>;

        using ConfigRet = CustomError<Config>;

        using ApiRet = CustomError<Api>;
        using ApiTpapiRet = CustomError<Api::Tpapi>;
        using ApiTpapiEpRet = CustomError<Api::Tpapi::Ep>;
        
        using CoreRet = CustomError<Core>;
        using CoreBatteryRet = CustomError<Core::Battery>;
        using CoreHeatRet = CustomError<Core::Heat>;
        using CoreChargeRet = CustomError<Core::Charge>;
        using CoreHeatTempRangeCelsiusRet = CustomError<Core::Heat::TempRangeCelsius>;

        using LoggingRet = CustomError<Logging>;

    private:
        Config config;
        rapidjson::Document json;

        std::string_view confFilePath;

    public:
        explicit JsonConfigManager(std::string_view filePath) noexcept: confFilePath(filePath) {
            BOOST_ASSERT_MSG(!filePath.empty(), "configuration file path can't be null");
        }

        [[nodiscard]] const Config &getConfig() const noexcept override {
            return config;
        }

        Status apply() noexcept override {
            BOOST_LEAF_AUTO(conf, parseConfig());
            config = conf;
            return {};
        }

        Status apply(const Config &newConfig) noexcept override {
            using namespace rapidjson;
            return boost::leaf::try_handle_some(
                    [this, &newConfig]() -> Status {
                        static constexpr std::string_view kApiFieldName = "api";
                        applyApi(json[Value(kApiFieldName.data(), kApiFieldName.size())], newConfig.api);

                        static constexpr std::string_view kCoreFieldName = "core";
                        applyCore(
                                json[Value(kCoreFieldName.data(), kCoreFieldName.size())],
                                newConfig.core);

                        static constexpr std::string_view kLoggingFieldName = "logging";
                        applyLogging(
                                json[Value(kLoggingFieldName.data(), kLoggingFieldName.size())],
                                newConfig.logging);

                        config = newConfig;
                        return {};
                    }
            );
        }

        Status load() noexcept override {
            return boost::leaf::try_handle_some(
                    [this]() -> Status {
                        law::file::StaticFile<4096> confFile;
                        BOOST_LEAF_CHECK(confFile.open(confFilePath, law::file::ReadOnly()));
                        BOOST_LEAF_AUTO(fileContent, confFile.read());
                        return parse(fileContent);
                    },
                    [](law::RetCode) -> Status {
                        return newError(RetCode::ROM_CONFIGURATION_LOAD_ERROR);
                    }
            );
        }

        Status save() noexcept override {
            return boost::leaf::try_handle_some(
                    [this]() -> Status {
                        using namespace law::file;
                        File confFile;
                        BOOST_LEAF_CHECK(
                                confFile.open(
                                        confFilePath,
                                        WriteOnly() | Create(),
                                        OwnerRead() | OwnerWrite() | GroupRead() | GroupWrite()
                                )
                        );
                        BOOST_LEAF_AUTO(serializedConf, serialize());
                        BOOST_LEAF_CHECK(confFile.resize(serializedConf.size()));
                        return confFile.write(serializedConf);
                    },
                    [](law::RetCode) -> Status {
                        return newError(RetCode::ROM_CONFIGURATION_SAVE_ERROR);
                    }
            );
        }

        Status restore() noexcept override {
            return boost::leaf::try_handle_some(
                    [this]() -> Status {
                        static constexpr std::string_view kDefaultConfiguration = R"({"api":{"tpapi":{"ep":{"type":"uds_udp_server","uds_udp_server":{"path":"/var/run/gnssm.ctl.sock"}}},"gnss":{"transport":{"type":"tcp_server","tcp_server":{"port":5821}}},"rtcm":{"transport":{"type":"uds_udp_server","tcp_server":{"port":5822},"uds_udp_server":{"path":"/var/run/gnssm.sock"}}}},"core":{"module_type":"ZED_F9P","gnss":{"transport":{"type":"rs232","rs232":{"device":"/dev/ttymxc1","baudrate":230400,"data_bits":8,"parity":"none","stop_bits":1,"flow_control":false}},"properties":{"navigation_solution_rate_ms":1000}},"rtcm":{"transport":{"type":"rs232","rs232":{"device":"/dev/ttymxc2","baudrate":38400,"data_bits":8,"parity":"none","stop_bits":1,"flow_control":false}},"properties":{"rtk_mode":{"mode":"rover","base":{"lat":580063510,"lon":562873283,"alt":172469}}}},"imu":{"enable":true,"properties":{"install_mode":{"mode":"auto","manual":{"x":100,"y":100,"z":100}}},"odo_sensor":{"enable":true,"type":"j1979","j1979":{"if":"can0","dst_addr":"0x7E8"}}}},"logging":{"level":"INFO"}})";
                        BOOST_LEAF_CHECK(parse(kDefaultConfiguration));
                        saveBrokenConfigFile();
                        BOOST_LEAF_CHECK(save());
                        return apply();
                    },
                    [](law::RetCode) -> Status {
                        return newError(RetCode::ROM_CONFIGURATION_RESTORE_ERROR);
                    }
            );
        }

    private:
        Status parse(std::string_view jsonString) noexcept {
            json.Parse(jsonString.data(), jsonString.size());
            return (json.HasParseError() ? newError(RetCode::ROM_CONFIGURATION_CORRUPT) : Status());
        }

        SerializedConf serialize() noexcept {
            using namespace rapidjson;
            StringBuffer sb;

            PrettyWriter<StringBuffer> writer(sb);
            writer.SetIndent(' ', 2);
            if (!json.Accept(writer)) {
                return newError(RetCode::ROM_CONFIGURATION_SERIALIZE_ERROR);
            }

            return std::string{sb.GetString(), sb.GetSize()};
        }

        void saveBrokenConfigFile() noexcept {
            const auto [confFileDir, confFileBasename]
                    = utils::file::delimFilePathOnDirAndName(confFilePath);
            const auto brokenConfFilePath = std::string(
                    confFileDir.data(), confFileDir.size()) +
                                            "/." +
                                            std::string(confFileBasename.data(), confFileBasename.size()) +
                                            ".broken";
            rename(confFilePath.data(), brokenConfFilePath.data());
        }

        ConfigRet parseConfig() noexcept {
            PARSE_OBJECT_FIELD(apiField, json, "api");
            BOOST_LEAF_AUTO(api, parseApi(apiField));

            PARSE_OBJECT_FIELD(coreField, json, "core");
            BOOST_LEAF_AUTO(core, parseCore(coreField));

            PARSE_OBJECT_FIELD(loggingField, json, "logging");
            BOOST_LEAF_AUTO(logging, parseLogging(loggingField));

            return Config{
                    .api = api,
                    .core = core,
                    .logging = logging
            };
        }

        static void applyApi(ValueRef &apiField, const Api &api) {
            using namespace rapidjson;

            static constexpr std::string_view kTpapiFieldName = "tpapi";
            applyApiTpapi(apiField[Value(kTpapiFieldName.data(), kTpapiFieldName.size())], api.tpapi);
        }

        static void applyApiTpapi(ValueRef &tpapiField, const Api::Tpapi &tpapi) {
            using namespace rapidjson;
            static constexpr std::string_view kEpFieldName = "ep";
            applyApiTpapiEp(tpapiField[Value(kEpFieldName.data(), kEpFieldName.size())], tpapi.ep);
        }

        static void applyApiTpapiEp(ValueRef &epField, const Api::Tpapi::Ep &ep) {
            using namespace rapidjson;
            std::visit(
                    [&epField] <typename T>(const T &tp) {
                        static constexpr std::string_view kTypeFieldName = "type";
                        auto &typeField
                                = epField[rapidjson::Value(kTypeFieldName.data(), kTypeFieldName.size())];
                        if constexpr (std::same_as<tp::UdsUdpServer, T>) {
                            static constexpr std::string_view kUdsUdpServerFieldName = "uds_udp_server";
                            typeField = Value(kUdsUdpServerFieldName.data(), kUdsUdpServerFieldName.size());

                            static constexpr std::string_view kPathFieldName = "path";
                            epField
                            [Value(kUdsUdpServerFieldName.data(), kUdsUdpServerFieldName.size())]
                            [Value(kPathFieldName.data(), kPathFieldName.size())] =
                                    Value(tp.path.data(), tp.path.size());
                        }
                    },
                    ep
            );
        }

        static void applyCore(ValueRef &coreField, const Core &core) {
            using namespace rapidjson;

            static constexpr std::string_view kBatteryFieldName = "battery";
            applyCoreBattery(coreField[Value(kBatteryFieldName.data(), kBatteryFieldName.size())],
                    core.battery);

            static constexpr std::string_view kHeatFieldName = "heat";
            applyCoreHeat(coreField[Value(kHeatFieldName.data(), kHeatFieldName.size())],
                    core.heat);

            static constexpr std::string_view kChargeFieldName = "charge";
            applyCoreCharge(coreField[Value(kChargeFieldName.data(), kChargeFieldName.size())],
                    core.charge);
        }

        static void applyCoreBattery(ValueRef &batteryField, const Core::Battery &battery) {
            using namespace rapidjson;
            static constexpr std::string_view kConnectAwaitTimeoutSec = "connect_await_timeout_sec";
            batteryField[Value(kConnectAwaitTimeoutSec.data(), kConnectAwaitTimeoutSec.size())] = 
                Value(battery.connectAwaitTimeoutSec.count());

            static constexpr std::string_view kActPwrSourceCheckTimeoutHours = "act_pwr_source_check_timeout_h";
            batteryField[Value(kActPwrSourceCheckTimeoutHours.data(), kActPwrSourceCheckTimeoutHours.size())] = 
                Value(battery.actPwrSourceCheckTimeoutHours.count());
        }

        static void applyCoreHeat(ValueRef &heatField, const Core::Heat &heat) {
            using namespace rapidjson;

            static constexpr std::string_view kHeatDurationH = "heat_duration_h";
            heatField[Value(kHeatDurationH.data(), kHeatDurationH.size())] = 
                Value(heat.heatDurationH.count());

            static constexpr std::string_view kTempRangeRetryTimeoutH = "temp_range_retry_timeout_h";
            heatField[Value(kTempRangeRetryTimeoutH.data(), kTempRangeRetryTimeoutH.size())] = 
                Value(heat.tempRangeRetryTimeoutH.count());

            static constexpr std::string_view kTempRangeCelsius = "temp_range_celsius";
            applyCoreHeatTempRangeCelsius(heatField[Value(kTempRangeCelsius.data(), kTempRangeCelsius.size())],
                    heat.tempRangeCelsius);
        }

        static void applyCoreCharge(ValueRef &chargeField, const Core::Charge &charge) {
            using namespace rapidjson;

            static constexpr std::string_view kStartChargeAtVolts = "start_charge_at_volts";
            chargeField[Value(kStartChargeAtVolts.data(), kStartChargeAtVolts.size())] = 
                Value(float(charge.startChargeAtVolts));

            static constexpr std::string_view kTargetBatteryVoltage = "target_battery_voltage";
            chargeField[Value(kTargetBatteryVoltage.data(), kTargetBatteryVoltage.size())] = 
                Value(float(charge.targetBatteryVoltage));
            
            static constexpr std::string_view kChargeStatusUpdatePeriodH = "charge_status_update_period_h";
            chargeField[Value(kChargeStatusUpdatePeriodH.data(), kChargeStatusUpdatePeriodH.size())] = 
                Value(charge.chargeStatusUpdatePeriodH.count());
        }

        static void applyCoreHeatTempRangeCelsius(ValueRef &tempRangeCelsiusField, const Core::Heat::TempRangeCelsius &tempRangeCelsius) {
            using namespace rapidjson;

            static constexpr std::string_view kMin = "min";
            tempRangeCelsiusField[Value(kMin.data(), kMin.size())] = 
                Value(tempRangeCelsius.min);

            static constexpr std::string_view kMax = "max";
            tempRangeCelsiusField[Value(kMax.data(), kMax.size())] = 
                Value(tempRangeCelsius.max);
        }

        static void applyLogging(ValueRef &loggingField, const Logging &logging) {
            loggingField["level"].SetString(logging.level.data(), logging.level.size());
        }

        static ApiRet parseApi(ConstValueRef apiField) noexcept {
            PARSE_OBJECT_FIELD(tpapiField, apiField, "tpapi");
            BOOST_LEAF_AUTO(tpapi, parseApiTpapi(tpapiField));

            return Api{
                    .tpapi = tpapi
            };
        }

        static CoreRet parseCore(ConstValueRef coreField) noexcept {
            PARSE_OBJECT_FIELD(batteryField, coreField, "battery");
            BOOST_LEAF_AUTO(battery, parseCoreBattery(batteryField));

            PARSE_OBJECT_FIELD(heatField, coreField, "heat");
            BOOST_LEAF_AUTO(heat, parseCoreHeat(heatField));

            PARSE_OBJECT_FIELD(chargeField, coreField, "charge");
            BOOST_LEAF_AUTO(charge, parseCoreCharge(chargeField));

            return Core{
                    .battery = battery,
                    .heat = heat,
                    .charge = charge
            };
        }
        

        static LoggingRet parseLogging(ConstValueRef loggingField) noexcept {
            PARSE_STRING_FIELD_CHECK_ALLOW_VALUES(level, loggingField, "level", Logging::Level,
                                                  Logging::kLevelAllowValues);
            return Logging{
                    .level = level
            };
        }

        static ApiTpapiRet parseApiTpapi(ConstValueRef tpapiField) noexcept {
            PARSE_OBJECT_FIELD(epField, tpapiField, "ep");
            BOOST_LEAF_AUTO(epValue, parseApiTpapiEp(epField));
            return Api::Tpapi{
                    .ep = epValue
            };
        }

        static ApiTpapiEpRet parseApiTpapiEp(ConstValueRef epField) noexcept {
            static constexpr std::string_view kUdsUdpServerFieldName = "uds_udp_server";

            BOOST_LEAF_AUTO(&type, checkPresence(epField, "type"));
            const auto typeValue = std::string_view(type.GetString(), type.GetStringLength());

            Api::Tpapi::Ep ret;
            if (typeValue == kUdsUdpServerFieldName) {
                BOOST_LEAF_AUTO(&udsUdpServerField, checkPresence(epField, kUdsUdpServerFieldName));
                BOOST_LEAF_AUTO(udsUdpServer, parseUdsUdpServer(udsUdpServerField));
                ret = udsUdpServer;
            } else {
                return newError(RetCode::ROM_CONFIGURATION_UNEXPECTED_FIELD_VALUE, typeValue);
            }

            return ret;
        }

        static CoreBatteryRet parseCoreBattery(ConstValueRef batteryField) noexcept {
            PARSE_UINT_FIELD_CHECK_MIN_MAX(connectAwaitTimeoutSec, batteryField, "connect_await_timeout_sec",
                                            Core::Battery::kConnectAwaitTimeoutSecMinValue.count(), 
                                            Core::Battery::kConnectAwaitTimeoutSecMaxValue.count());

            PARSE_UINT_FIELD_CHECK_MIN_MAX(actPwrSourceCheckTimeoutH, batteryField, "act_pwr_source_check_timeout_h",
                                            Core::Battery::kActPwrSourceCheckTimeoutHoursMinValue.count(), 
                                            Core::Battery::kActPwrSourceCheckTimeoutHoursMaxValue.count());

            return Core::Battery{
                    .connectAwaitTimeoutSec = std::chrono::seconds(connectAwaitTimeoutSec),
                    .actPwrSourceCheckTimeoutHours = std::chrono::hours(actPwrSourceCheckTimeoutH)
            };
        }

        static CoreHeatRet parseCoreHeat(ConstValueRef heatField) noexcept {
            PARSE_UINT_FIELD_CHECK_MIN_MAX(heatDurationH, heatField, "heat_duration_h",
                                            Core::Heat::kHeatDurationHMinValue.count(), 
                                            Core::Heat::kHeatDurationHMaxValue.count());

            PARSE_UINT_FIELD_CHECK_MIN_MAX(tempRangeRetryTimeoutH, heatField, "temp_range_retry_timeout_h",
                                            Core::Heat::kTempRangeRetryTimeoutHMinValue.count(), 
                                            Core::Heat::kTempRangeRetryTimeoutHMaxValue.count());

            PARSE_OBJECT_FIELD(tempRangeCelsiusField, heatField, "temp_range_celsius");
            BOOST_LEAF_AUTO(tempRangeCelsius, parseCoreHeatTempRangeCelsius(tempRangeCelsiusField));

            return Core::Heat{
                    .heatDurationH = std::chrono::hours(heatDurationH),
                    .tempRangeRetryTimeoutH = std::chrono::hours(tempRangeRetryTimeoutH),
                    .tempRangeCelsius = tempRangeCelsius
            };
        }

        static CoreChargeRet parseCoreCharge(ConstValueRef chargeField) noexcept {
            PARSE_UINT_FIELD_CHECK_MIN_MAX(chargeStatusUpdatePeriodH, chargeField, "charge_status_update_period_h",
                                            Core::Charge::kChargeStatusUpdatePeriodHMinValue.count(), 
                                            Core::Charge::kChargeStatusUpdatePeriodHMaxValue.count());
            
            PARSE_FLOAT_FIELD_CHECK_MIN_MAX(startChargeAtVolts, chargeField, "start_charge_at_volts",
                                            Core::Charge::kStartChargeAtVoltsMinValue,
                                            Core::Charge::kStartChargeAtVoltsMaxValue);

            PARSE_FLOAT_FIELD_CHECK_MIN_MAX(targetBatteryVoltage, chargeField, "target_battery_voltage",
                                            Core::Charge::kTargetBatteryVoltageMinValue,
                                            Core::Charge::kTargetBatteryVoltageMaxValue);
            
            return Core::Charge{
                    .chargeStatusUpdatePeriodH = std::chrono::hours(chargeStatusUpdatePeriodH),
                    .startChargeAtVolts = float(startChargeAtVolts),
                    .targetBatteryVoltage = float(targetBatteryVoltage)
            };
        }

        static CoreHeatTempRangeCelsiusRet parseCoreHeatTempRangeCelsius(ConstValueRef tempRangeCelsiusField) noexcept {
            PARSE_INT_FIELD_CHECK_MIN_MAX(min, tempRangeCelsiusField, "min",
                                            Core::Heat::TempRangeCelsius::kMinMinValue,
                                            Core::Heat::TempRangeCelsius::kMinMaxValue);
            
            PARSE_INT_FIELD_CHECK_MIN_MAX(max, tempRangeCelsiusField, "max",
                                            Core::Heat::TempRangeCelsius::kMaxMinValue,
                                            Core::Heat::TempRangeCelsius::kMaxMaxValue);
            
            return Core::Heat::TempRangeCelsius{
                    .min = min,
                    .max = max
            };
        }

        static UdsUdpServerRet parseUdsUdpServer(ConstValueRef udsUdpServerField) noexcept {
            PARSE_STRING_FIELD(path, udsUdpServerField, "path", tp::UdsUdpServer::Path);
            return tp::UdsUdpServer{
                    .path = path
            };
        }

        static ConstValueRefRet checkPresence(ConstValueRef field, std::string_view name) noexcept {
            const auto it =
                    field.FindMember(rapidjson::Value(name.data(), name.size()));
            if (it == field.MemberEnd()) {
                return newError(RetCode::ROM_CONFIGURATION_FIELD_NOT_PRESENCE, name);
            }
            return it->value;
        }
    };
}

#undef THROW_ERROR
#undef CHECK_STRING_FIELD_LENGTH
#undef CHECK_FIELD_VALUE_RANGE_SIGNED_INTEGRAL
#undef CHECK_FIELD_VALUE_RANGE_UNSIGNED_INTEGRAL
#undef CHECK_FIELD_VALUE_RANGE_FP
#undef CHECK_FIELD_VALUE_ALLOW_VALUES
#undef CHECK_FIELD_TYPE
#undef PARSE_FIELD
#undef PARSE_BOOL_FIELD
#undef PARSE_STRING_FIELD
#undef PARSE_STRING_FIELD_CHECK_ALLOW_VALUES
#undef PARSE_UINT_FIELD
#undef PARSE_SIGNED_INTEGER_FIELD_CHECK_MIN_MAX
#undef PARSE_UNSIGNED_INTEGER_FIELD_CHECK_MIN_MAX
#undef PARSE_FP_FIELD_CHECK_MIN_MAX
#undef PARSE_UINT_FIELD_CHECK_MIN_MAX
#undef PARSE_UINT64_FIELD_CHECK_MIN_MAX
#undef PARSE_INT_FIELD_CHECK_MIN_MAX
#undef PARSE_INT64_FIELD_CHECK_MIN_MAX
#undef PARSE_FLOAT_FIELD_CHECK_MIN_MAX
#undef PARSE_DOUBLE_FIELD_CHECK_MIN_MAX
#undef PARSE_OBJECT_FIELD
