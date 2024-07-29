#pragma once
#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <optional>


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

    class JValue
    {
    private:
        JDataType m_Data;

    public:
        JValue();
        JValue(const JValue& rfJValue);
        template <class T> JValue(T&& rfData);
        template <>        JValue(JValue& rfJValue);
        template <>        JValue(JValue&& rfJValue) noexcept;

        auto operator=(const JValue& rfJValue) -> JValue&;
        template <class T> auto operator=(T&& rfData) -> JValue&;
        template <>        auto operator=(JValue& rfJValue) -> JValue&;
        template <>        auto operator=(JValue&& rfJValue) noexcept -> JValue&;

        template <class T> auto operator[](T&& rfData) -> JValue&;
        template <class T> auto operator[](T&& rfData) const -> const JValue&;

    public:
        auto Move(JValue&& rfJValue) -> JValue&;
        auto Copy(const JValue& rfJValue) -> JValue&;

    public:
        template <class T> auto Check() const -> bool;
        template <class T> auto Assign(T&& rfData) -> JValue&;

    public:
        template<class T = std::size_t> auto GetNum() const->T;
        template<class T = std::double_t> auto GetFloat() const->T;
        auto GetBool() const -> bool;
        auto GetStrView() const-> std::string_view;
        auto GetStr() -> std::string&;
        auto GetStr() const -> const std::string&;
        auto GetArray() -> JArray_t&;
        auto GetArray() const -> const JArray_t&;
        auto GetObject() -> JObject_t&;
        auto GetObject() const -> const JObject_t&;

    public:
        template <class T> auto At(T&& rfKeyorIndex) -> JValue&;
        template <class T> auto At(T&& rfKeyorIndex) const -> const JValue&;
        
    public:
        auto Clear() -> void;
        auto ToArray() -> JArray_t&;
        auto ToObject() -> JObject_t&;

    public:
        auto Dump(const bool isFormat) const -> std::string;
        auto Dump(std::string& wsText, const bool isFormat, std::size_t nIndent = 0) const -> void;
    };


    inline JValue::JValue()
    {
        m_Data = JNull_t{};
    }

    template <class T>
    inline JValue::JValue(T&& rfData)
    {
        this->Assign<T>(std::forward<T>(rfData));
    }

    template <>
    inline JValue::JValue(JValue& rfJValue)
    {
        this->Copy(rfJValue);
    }

    inline JValue::JValue(const JValue& rfJValue)
    {
        this->Copy(rfJValue);
    }

    template <>
    inline JValue::JValue(JValue&& rfJValue) noexcept
    {
        this->Move(std::move(rfJValue));
    }

    template <class T>
    inline auto JValue::operator=(T&& rfData) -> JValue&
    {
        return this->Assign<T>(std::forward<T>(rfData));
    }

    template <>
    inline auto JValue::operator=(JValue& rfJValue) -> JValue&
    {
        return this->Copy(rfJValue);
    }

    auto inline JValue::operator=(const JValue& rfJValue) -> JValue&
    {
        return this->Copy(rfJValue);
    }

    template <>
    inline auto JValue::operator=(JValue&& rfJValue) noexcept -> JValue&
    {
        return this->Move(std::move(rfJValue));
    }


    template <class T>
    inline auto JValue::operator[](T&& rfData) -> JValue&
    {
        if constexpr (std::is_integral_v<T>)
        {
            return this->GetArray()[std::forward<T>(rfData)];
        }
        else
        {
            return this->GetObject()[std::forward<T>(rfData)];
        }
    }

    template <class T>
    inline auto JValue::operator[](T&& rfData) const -> const JValue&
    {
        if constexpr (std::is_integral_v<T>)
        {
            return this->GetArray()[std::forward<T>(rfData)];
        }
        else
        {
            static_assert(false, "ZxJson::JValue::operator[]() const: error type.");
        }
    }

    inline auto JValue::Move(JValue&& rfJValue) -> JValue&
    {
        this->m_Data = std::move(rfJValue.m_Data);
        rfJValue.m_Data = JNull_t{};
        return *this;
    }

    inline auto JValue::Copy(const JValue& rfJValue) -> JValue&
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
    }

    template<class T>
    inline auto JValue::Assign(T&& rfData) -> JValue&
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
        else if constexpr (std::is_same_v<T, JValue&> || std::is_same_v<T, const JValue&>)
        {
            this->Copy(rfData);
        }
        else if constexpr (std::is_same_v<T, JValue>)
        {
            this->Move(std::move(rfData));
        }
        else if constexpr (std::is_integral_v<T_decay>)
        {
            m_Data = static_cast<JInt_t>(rfData);
        }
        else if constexpr (std::is_integral_v<T_decay>)
        {
            m_Data = static_cast<JInt_t>(rfData);
        }
        else if constexpr (std::is_bounded_array_v<std::remove_cvref_t<T>> || std::is_same_v<T_decay, char*> || std::is_same_v<T_decay, const char*>)
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

    inline auto JValue::GetBool() const -> bool
    {
        assert(this->Check<JBool_t>());
        return std::get<JBool_t>(m_Data);
    }

    template<class T>
    inline auto JValue::GetNum() const -> T
    {
        assert(this->Check<JInt_t>());
        return static_cast<T>(std::get<JInt_t>(m_Data));
    }

    template<class T>
    inline auto JValue::GetFloat() const -> T
    {
        assert(this->Check<JDouble_t>());
        return static_cast<T>(std::get<JDouble_t>(m_Data));
    }

    inline auto JValue::GetStrView() const -> std::string_view
    {
        return this->GetStr();
    }

    inline auto JValue::GetStr() -> std::string&
    {
        assert(this->Check<JString_t>());
        return *std::get<std::unique_ptr<JString_t>>(m_Data);
    }

    inline auto JValue::GetStr() const -> const std::string&
    {
        assert(this->Check<JString_t>());
        return *std::get<std::unique_ptr<JString_t>>(m_Data);
    }

    inline auto JValue::GetArray() -> JArray_t&
    {
        assert(this->Check<JArray_t>());
        return *std::get<std::unique_ptr<JArray_t>>(m_Data);
    }

    inline auto JValue::GetArray() const -> const JArray_t&
    {
        assert(this->Check<JArray_t>());
        return *std::get<std::unique_ptr<JArray_t>>(m_Data);
    }

    inline auto JValue::GetObject() -> JObject_t&
    {
        assert(this->Check<JObject_t>());
        return *std::get<std::unique_ptr<JObject_t>>(m_Data);
    }

    inline auto JValue::GetObject() const -> const JObject_t&
    {
        assert(this->Check<JObject_t>());
        return *std::get<std::unique_ptr<JObject_t>>(m_Data);
    }

    template <class T>
    inline auto JValue::At(T&& rfKeyorIndex) -> JValue&
    {
        if constexpr (std::is_integral_v<T>)
        {
            if (!this->Check<JArray_t>()) { throw std::runtime_error("ZxJson::JValue::At<>(): not a array type!"); }

            auto& jarray = this->GetArray();
            if (jarray.size() > rfKeyorIndex)
            {
                return jarray[rfKeyorIndex];
            }

            throw std::runtime_error("ZxJson::JValue::QueryKey<>(): out of idx!");
        }
        else
        {
            if (!this->Check<JObject_t>()) { throw std::runtime_error("ZxJson::JValue::At<>(): not a object type!"); }

            auto& obj = this->GetObject();
            if (auto ite = obj.find(std::forward<T>(rfKeyorIndex));ite != obj.end())
            {
                return ite->second;
            }

            throw std::runtime_error("ZxJson::JValue::QueryKey<>(): not find key!");
        }
    }

    template <class T>
    inline auto JValue::At(T&& rfKeyorIndex) const -> const JValue&
    {
        if constexpr (std::is_integral_v<T>)
        {
            if (!this->Check<JArray_t>()) { throw std::runtime_error("ZxJson::JValue::At<>() const: not a array type!"); }

            const auto& jarray = this->GetArray();
            if (jarray.size() > rfKeyorIndex)
            {
                return jarray[rfKeyorIndex];
            }

            throw std::runtime_error("ZxJson::JValue::QueryKey<>(): not find idx!");
        }
        else
        {
            if (!this->Check<JObject_t>()) { throw std::runtime_error("ZxJson::JValue::At<>() const: not a object type!"); }

            const auto& obj = this->GetObject();
            if (const auto ite = obj.find(std::forward<T>(rfKeyorIndex));ite != obj.end())
            {
                return ite->second;
            }

            throw std::runtime_error("ZxJson::JValue::QueryKey<>() const: not find key!");
        }
    }

    template <class T>
    inline auto JValue::Check() const -> bool
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

    inline auto JValue::Clear() -> void
    {
        m_Data = {};
    }

    inline auto JValue::ToArray() -> JArray_t&
    {
        if (this->Check<JNull_t>())
        {
            m_Data = std::make_unique<JArray_t>();
        }
        else if (!this->Check<JArray_t>())
        {
            throw std::runtime_error("ZxJson::JValue::ToArray<>(): error!");
        }

        return this->GetArray();
    }

    inline auto JValue::ToObject() -> JObject_t&
    {
        if (this->Check<JNull_t>())
        {
            m_Data = std::make_unique<JObject_t>();
        }
        else if (!this->Check<JObject_t>())
        {
            throw std::runtime_error("ZxJson::JValue::ToObject<>(): error!");
        }

        return this->GetObject();
    }

} // namespace ZQF::ZxJson
