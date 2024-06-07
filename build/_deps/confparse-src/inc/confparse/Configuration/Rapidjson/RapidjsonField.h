#pragma once

#include <rapidjson/document.h>

#include "confparse/Configuration/IField.h"

#define CHECK_FIELD_TYPE(method, f) if (!f.method()) return newError(RetCode::INVALID_TYPE)

namespace cp {
    namespace {
        template <typename T>
        concept IsBoolField = std::is_same_v<T, bool>;

        template <typename T>
        concept IsUintField = std::is_unsigned_v<T> && (sizeof(T) <= sizeof(std::uint32_t)) && !IsBoolField<T>;

        template <typename T>
        concept IsUint64Field = std::is_unsigned_v<T> && (sizeof(T) == sizeof(std::uint64_t)) && !IsBoolField<T>;

        template <typename T>
        concept IsIntField = std::is_signed_v<T> && (sizeof(T) <= sizeof(std::int32_t)) && !IsBoolField<T>;

        template <typename T>
        concept IsInt64Field = std::is_signed_v<T> && (sizeof(T) == sizeof(std::int64_t)) && !IsBoolField<T>;

        template <typename T>
        concept IsFloatField = std::is_floating_point_v<T> && (sizeof(T) == sizeof(float));

        template <typename T>
        concept IsDoubleField = std::is_floating_point_v<T> && (sizeof(T) == sizeof(double));

        template <typename T>
        concept IsStringField = std::is_convertible_v<std::string_view, T>;

        template <typename T>
        concept IsStaticString = requires{ T::full(); };
    }

    class RapidjsonField : public IField<RapidjsonField> {
    private:
        friend struct IField<RapidjsonField>;

        template <typename T>
        using FieldValue = CustomError<T>;

    private:
        rapidjson::Value &valueRef;
        RAPIDJSON_DEFAULT_ALLOCATOR &allocatorRef;

    public:
        explicit RapidjsonField(rapidjson::Value &valRef, RAPIDJSON_DEFAULT_ALLOCATOR &allocatorRef) :
                valueRef(valRef), allocatorRef(allocatorRef) {}

    private:
        template <typename T>
        FieldValue<T> getImpl() const noexcept {
            using namespace boost::leaf;
            return checkTypeAndGet<T>(valueRef);
        }

        RapidjsonField getFoldObjectImpl(std::string_view fieldName) noexcept {
            auto &ref = valueRef[rapidjson::Value(fieldName.data(), fieldName.size())];
            return RapidjsonField(ref, allocatorRef);
        }

        [[nodiscard]] bool containsImpl(std::string_view fieldName) const noexcept {
            return valueRef.HasMember(fieldName.data());
        }

        template <IsBoolField T>
        [[nodiscard]] FieldValue<bool> checkTypeAndGet(const rapidjson::Value &field) const noexcept {
            CHECK_FIELD_TYPE(IsBool, field);
            return field.GetBool();
        }

        template <IsUintField T>
        [[nodiscard]] FieldValue<std::uint32_t> checkTypeAndGet(const rapidjson::Value &field) const noexcept {
            CHECK_FIELD_TYPE(IsUint, field);
            return field.GetUint();
        }

        template <IsUint64Field T>
        [[nodiscard]] FieldValue<std::uint64_t> checkTypeAndGet(const rapidjson::Value &field) const noexcept {
            CHECK_FIELD_TYPE(IsUint64, field);
            return field.GetUint64();
        }

        template <IsIntField T>
        [[nodiscard]] FieldValue<std::int32_t> checkTypeAndGet(const rapidjson::Value &field) const noexcept {
            CHECK_FIELD_TYPE(IsInt, field);
            return field.GetInt();
        }

        template <IsInt64Field T>
        [[nodiscard]] FieldValue<std::int64_t> checkTypeAndGet(const rapidjson::Value &field) const noexcept {
            CHECK_FIELD_TYPE(IsInt64, field);
            return field.GetInt64();
        }

        template <IsFloatField T>
        [[nodiscard]] FieldValue<float> checkTypeAndGet(const rapidjson::Value &field) const noexcept {
            CHECK_FIELD_TYPE(IsFloat, field);
            return field.GetFloat();
        }

        template <IsDoubleField T>
        [[nodiscard]] FieldValue<double> checkTypeAndGet(const rapidjson::Value &field) const noexcept {
            CHECK_FIELD_TYPE(IsDouble, field);
            return field.GetDouble();
        }

        template <typename T>
        [[nodiscard]] FieldValue<T> checkTypeAndGet(const rapidjson::Value &field) const noexcept {
            CHECK_FIELD_TYPE(IsString, field);

            if constexpr (IsStaticString<T>) {
                if (field.GetStringLength() > T::MAX_SIZE) {
                    return newError(RetCode::INVALID_LENGTH);
                }
            }

            return T(field.GetString(), field.GetStringLength());
        }
    };
}

#undef CHECK_FIELD_TYPE