#pragma once
#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>


namespace ZQF::ZxJson
{
    // https://www.cppstories.com/2021/heterogeneous-access-cpp20/
    struct string_hash
    {
        using is_transparent = void;
        [[nodiscard]] size_t operator()(const char* cpKey) const
        {
            return std::hash<std::string_view>{}(cpKey);
        }

        [[nodiscard]] size_t operator()(const std::string_view msKey) const
        {
            return std::hash<std::string_view>{}(msKey);
        }

        [[nodiscard]] size_t operator()(const std::string& msKey) const
        {
            return std::hash<std::string>{}(msKey);
        }
    };

    class JValue;
    using JNull_t = std::monostate;
    using JBool_t = bool;
    using JInt_t = uint64_t;
    using JFloat_t = float;
    using JDouble_t = double;
    using JString_t = std::string;
    using JStringView_t = std::string_view;
    using JArray_t = std::vector<JValue>;
    using JObject_t = std::unordered_map<std::string, JValue, string_hash, std::equal_to<>>;
    using JDataType = std::variant<JNull_t, JBool_t, JInt_t, JDouble_t, std::unique_ptr<JString_t>, std::unique_ptr<JArray_t>, std::unique_ptr<JObject_t>>;

    class JValue // NOLINT
    {
    private:
        JDataType m_Data;

    public:
        JValue();
        JValue(const JValue& rfJValue);
        JValue(JValue&& rfJValue) noexcept;
        auto operator=(const JValue& rfJValue)->JValue&;
        auto operator=(JValue&& rfJValue) noexcept -> JValue&;

        template <class T>
        JValue(T&& rfData);
        template <class T>
        auto operator=(T&& rfData) -> JValue&;
        template <class T>
        auto operator[](T&& rfData)->JValue&;

        template <class T>
        auto Get() -> T;
        template <class T>
        auto Sure() -> T;
        template <class T>
        auto Check() -> bool;

        auto Dump(std::string& wsText, bool isFormat, size_t nIndent = 0) const -> void;
    };

    inline JValue::JValue()
    {
        m_Data = JNull_t{};
    }

    inline JValue::JValue(const JValue& rfJValue)
    {
        this->operator=(rfJValue);
    }

    inline JValue::JValue(JValue&& rfJValue) noexcept
    {
        this->operator=(std::forward<JValue>(rfJValue));
    }

    inline auto JValue::operator=(const JValue& rfJValue) -> JValue&
    {
        std::visit(
            [this](auto&& data) {
                using T_data = std::decay_t<decltype(data)>;
                if constexpr (std::is_same_v<T_data, std::unique_ptr<JString_t>> || std::is_same_v<T_data, std::unique_ptr<JArray_t>> || std::is_same_v<T_data, std::unique_ptr<JObject_t>>)
                {
                    (*this) = *data;
                }
                else if constexpr (std::is_same_v<T_data, JNull_t> || std::is_same_v<T_data, JBool_t> || std::is_same_v<T_data, JInt_t> || std::is_same_v<T_data, JDouble_t>)
                {
                    (*this) = data;
                }
                else
                {
                    static_assert(false, "ZxJson::JValue::operator=<>(): error type!");
                }
            },
            rfJValue.m_Data);

        return *this;
    };

    inline auto JValue::operator=(JValue&& rfJValue) noexcept -> JValue&
    {
        this->m_Data = std::move(rfJValue.m_Data);
        rfJValue.m_Data = JNull_t{};
        return *this;
    };

    template <class T>
    inline JValue::JValue(T&& rfData)
    {
        this->operator=(std::forward<T>(rfData));
    }

    template <class T>
    auto JValue::operator=(T&& rfData) -> JValue& // NOLINT
    {
        using T_decay = std::decay_t<decltype(rfData)>;

        if constexpr (std::is_same_v<T_decay, JNull_t>)
        {
            m_Data = JNull_t{};
        }
        else if constexpr (std::is_floating_point_v<T_decay> || std::is_same_v<T_decay, JBool_t>)
        {
            m_Data = rfData;
        }
        else if constexpr (std::is_same_v<T_decay, JValue>)
        {
            this->operator=(std::forward<T>(rfData));
        }
        else if constexpr (std::is_integral_v<T_decay>)
        {
            m_Data = static_cast<JInt_t>(rfData);
        }
        else if constexpr (std::is_bounded_array_v<std::remove_cvref_t<T>>)
        {
            m_Data = std::make_unique<JString_t>(rfData);
        }
        else if constexpr (std::is_same_v<T_decay, std::string_view>)
        {
            m_Data = std::make_unique<JString_t>(rfData.data(), rfData.size());
        }
        else if constexpr (std::is_same_v<T_decay, JString_t> || std::is_same_v<T_decay, JArray_t> || std::is_same_v<T_decay, JObject_t>)
        {
            m_Data = std::make_unique<T_decay>(std::forward<T>(rfData));
        }
        else
        {
            static_assert(false, "ZxJson::JValue::operator=<>(): error type!");
        }

        return *this;
    }

    template <class T>
    inline auto JValue::operator[](T&& rfData) -> JValue& // NOLINT
    {
        if constexpr (std::is_integral_v<T>)
        {
            return this->Get<JArray_t&>()[std::forward<T>(rfData)];
        }
        else
        {
            return this->Get<JObject_t&>()[std::forward<T>(rfData)];
        }
    }

    template <class T>
    inline auto JValue::Get() -> T
    {
        using T_decay = std::decay_t<T>;

        if constexpr (std::is_same_v<T_decay, JDataType>)
        {
            return m_Data;
        }
        else if constexpr (std::is_same_v<T_decay, JBool_t>)
        {
            assert(std::holds_alternative<JBool_t>(m_Data));
            return std::get<JBool_t>(m_Data);
        }
        else if constexpr (std::is_integral_v<T_decay>)
        {
            assert(std::holds_alternative<JInt_t>(m_Data));
            return static_cast<T>(std::get<JInt_t>(m_Data));
        }
        else if constexpr (std::is_same_v<T_decay, JFloat_t> || std::is_same_v<T_decay, JDouble_t>)
        {
            assert(std::holds_alternative<JDouble_t>(m_Data));
            return static_cast<T>(std::get<JDouble_t>(m_Data));
        }
        else if constexpr (std::is_same_v<T_decay, JStringView_t>)
        {
            assert(std::holds_alternative<std::unique_ptr<JString_t>>(m_Data));
            return JStringView_t{ *std::get<std::unique_ptr<JString_t>>(m_Data) };
        }
        else if constexpr (std::is_same_v<T_decay, JString_t> || std::is_same_v<T_decay, JArray_t> || std::is_same_v<T_decay, JObject_t>)
        {
            assert(std::holds_alternative<std::unique_ptr<T_decay>>(m_Data));
            return *std::get<std::unique_ptr<T_decay>>(m_Data);
        }
        else
        {
            static_assert(false, "ZxJson::JValue::Get<T>(): error type!");
        }
    }

    template <class T>
    inline auto JValue::Sure() -> T
    {
        using T_decay = std::decay_t<T>;

        static_assert((std::is_same_v<T_decay, JString_t> || std::is_same_v<T_decay, JArray_t> || std::is_same_v<T_decay, JObject_t>), "ZxJson::JValue::Sure<T>(): error type");

        if (std::holds_alternative<JNull_t>(m_Data))
        {
            m_Data = std::make_unique<T_decay>();
        }
        else if (!std::holds_alternative<std::unique_ptr<T_decay>>(m_Data))
        {
            throw std::runtime_error("ZxJson::JValue::Sure<>(): error!");
        }

        return *std::get<std::unique_ptr<T_decay>>(m_Data);
    }

    template <class T>
    inline auto JValue::Check() -> bool
    {
        if constexpr (std::is_same_v<T, JNull_t> || std::is_same_v<T, JBool_t> || std::is_same_v<T, JInt_t> || std::is_same_v<T, JDouble_t>)
        {
            return std::holds_alternative<T>(m_Data);
        }
        else if constexpr (std::is_same_v<T, JString_t> || std::is_same_v<T, JArray_t> || std::is_same_v<T, JObject_t>)
        {
            return std::holds_alternative<std::unique_ptr<T>>(m_Data);
        }
        else
        {
            static_assert(false, "ZxJson::JValue::Check<T>(): error type!");
        }
    }

} // namespace ZQF::ZxJson
