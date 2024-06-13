#pragma once
#include <span>
#include <memory>
#include <string>
#include <vector>
#include <variant>
#include <cassert>
#include <stdexcept>
#include <unordered_map>


namespace ZQF::ZxJson
{
	class JValue;
	using JNull_t = std::monostate;
	using JBool_t = bool;
	using JInt_t = uint64_t;
	using JFloat_t = float;
	using JDouble_t = double;
	using JString_t = std::string;
	using JStringView_t = std::string_view;
	using JArray_t = std::vector<JValue>;
	using JObject_t = std::unordered_map<std::string, JValue>;
	using JDataType = std::variant<JNull_t, JBool_t, JInt_t, JDouble_t, std::unique_ptr<JString_t>, std::unique_ptr<JArray_t>, std::unique_ptr<JObject_t>>;

	class JValue
	{
	private:
		JDataType m_Data;

	public:
		JValue();
		JValue(const JValue& rfJValue);
		JValue(JValue&& rfJar) noexcept;
		auto operator=(const JValue& rfJValue)->JValue&;
		auto operator=(JValue&& rfJValue) noexcept -> JValue&;

	public:
		template <class T> JValue(const T& rfData);
		template <class T> JValue(T&& rfData) noexcept;
		template <class T> auto operator=(const T& rfData)->JValue&;
		template <class T> auto operator=(T&& rfData) noexcept -> JValue&;
		template <class T> auto operator[](T&& rfData)->JValue&;

	public:
		template <class T> auto Get() -> T;
		template <class T> auto Sure() -> T;
		template <class T> auto Check() -> bool;

	public:
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

	template <class T> inline JValue::JValue(const T& rfData)
	{
		this->operator= <T>(rfData);
	}

	template <class T> inline JValue::JValue(T&& rfData) noexcept
	{
		this->operator= <T>(std::forward<T>(rfData));
	}

	template <class T> auto JValue::operator=(const T& rfData) -> JValue&
	{
		using T_decay = std::decay_t<decltype(rfData)>;

		if constexpr (std::is_same_v<T_decay, JNull_t>)
		{
			m_Data = JNull_t{};
		}
		else if constexpr (std::is_same_v<T_decay, JFloat_t> || std::is_same_v<T_decay, JDouble_t> || std::is_same_v<T_decay, JBool_t>)
		{
			m_Data = rfData;
		}
		else if constexpr (std::is_integral_v<T_decay>)
		{
			m_Data = static_cast<JInt_t>(rfData);
		}
		else if constexpr (std::is_bounded_array_v<T>)
		{
			m_Data = std::make_unique<JString_t>(rfData);
		}
		else if constexpr (std::is_same_v<T_decay, JStringView_t>)
		{
			m_Data = std::make_unique<JString_t>(rfData.data(), rfData.size());
		}
		else if constexpr (std::is_same_v<T_decay, JString_t> || std::is_same_v<T_decay, JArray_t> || std::is_same_v<T_decay, JObject_t>)
		{
			m_Data = std::make_unique<T_decay>(rfData);
		}
		else
		{
			static_assert(false, "ZxJson::JValue::operator=<>(): error type!");
		}

		return *this;
	}

	template <class T> auto JValue::operator=(T&& rfData) noexcept -> JValue&
	{
		using T_decay = std::decay_t<decltype(rfData)>;

		if constexpr (std::is_same_v<T_decay, JNull_t>)
		{
			m_Data = JNull_t{};
		}
		else if constexpr (std::is_same_v<T_decay, JFloat_t> || std::is_same_v<T_decay, JDouble_t> || std::is_same_v<T_decay, JBool_t>)
		{
			m_Data = rfData;
		}
		else if constexpr (std::is_integral_v<T_decay>)
		{
			m_Data = static_cast<JInt_t>(rfData);
		}
		else if constexpr (std::is_same_v<T_decay, std::string_view>)
		{
			m_Data = std::make_unique<JString_t>(rfData.data(), rfData.size());
		}
		else if constexpr (std::is_same_v<T_decay, JString_t> || std::is_same_v<T_decay, JArray_t> || std::is_same_v<T_decay, JObject_t>)
		{
			m_Data = std::make_unique<T_decay>(std::move(rfData));
		}
		else
		{
			static_assert(false, "ZxJson::JValue::operator=<>(): error type!");
		}

		return *this;
	}

	template <class T> inline auto JValue::operator[](T&& rfData) -> JValue&
	{
		using T_decay = std::decay_t<decltype(rfData)>;

		if constexpr (std::is_bounded_array_v<std::remove_cvref_t<T>> || std::is_same_v<T_decay, std::string>)
		{
			return this->Get<JObject_t&>()[rfData];
		}
		else if constexpr (std::is_same_v<T_decay, std::string_view>)
		{
			return this->Get<JObject_t&>()[std::string(rfData)];
		}
		else if constexpr (std::is_integral_v<T_decay>)
		{
			return this->Get<JArray_t&>()[rfData];
		}
		else
		{
			static_assert(false, "ZxJson::JValue::operator[]<T>: error type");
		}
	}


	template <class T> inline auto JValue::Get() -> T
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
			assert(std::holds_alternative< std::unique_ptr<JString_t>>(m_Data));
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

	template <class T> inline auto JValue::Sure() -> T
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

	template<class T> inline auto JValue::Check() -> bool
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

}

