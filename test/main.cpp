#include <ZxJson/JDoc.h>
#include <filesystem>
#include <cassert>
#include <iostream>
#include <fstream>
#include <print>
#include <chrono>


// # Utils #

namespace ZQF
{
	class ZxRecord
	{
	private:
		std::chrono::steady_clock::time_point m_tpBeg;
		std::chrono::steady_clock::time_point m_tpEnd;
		std::vector<std::chrono::duration<double, std::milli>> m_vcRecord;

	public:
		auto Beg() -> void;
		auto End() -> void;
		auto Log() -> void;
	};

	auto ZxRecord::Beg() -> void
	{
		m_tpBeg = std::chrono::steady_clock::now();
	}

	auto ZxRecord::End() -> void
	{
		m_tpEnd = std::chrono::steady_clock::now();
		m_vcRecord.emplace_back((m_tpEnd - m_tpBeg));
	}

	auto ZxRecord::Log() -> void
	{
		std::chrono::duration<double, std::milli> cout{};

		for (auto& dur : m_vcRecord)
		{
			cout += dur;
			std::println("{}", dur);
		}

		std::println("Avg:{}", cout / m_vcRecord.size());
	}
}

static auto FetchFileData(const std::filesystem::path phJson) -> std::string
{
	std::string data;
	std::ifstream ifs{ phJson };
	ifs.seekg(0, std::ios::end);
	data.reserve(ifs.tellg());
	ifs.seekg(0, std::ios::beg);
	data.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	return data;
}


// # Bench #

static auto BenchJsonParser() -> void
{
	ZQF::ZxRecord record;

	auto raw_data = ::FetchFileData(u8"1.json");
	for (size_t i = 0; i < 200; i++)
	{
		record.Beg();
		auto jv = ZQF::ZxJson::Load(raw_data);
		record.End();
	}

	record.Log();
}

static auto BenchJsonDumper() -> void
{
	ZQF::ZxRecord record;

	auto raw_data = ::FetchFileData(u8"1.json");
	auto jv = ZQF::ZxJson::Load(raw_data);
	for (size_t i = 0; i < 200; i++)
	{
		record.Beg();
		auto dump_str = ZQF::ZxJson::Dump(jv, true);
		dump_str[0] = {};
		record.End();
	}

	record.Log();
}


// # Test #

static auto TestJsonParser() -> void
{
	auto raw_data = ::FetchFileData(u8"1.json");
	auto jv = ZQF::ZxJson::Load(raw_data);
}

static auto TestJsonDumper() -> void
{
	auto raw_data = ::FetchFileData(u8"1.json");
	auto jv = ZQF::ZxJson::Load(raw_data);

	std::string d0;
	jv.Dump(d0, true, 0);
	assert(d0.size());

	std::string d1;
	jv.Dump(d1, false, 0);
	assert(d1.size());
}

static auto TestJsonValue() -> void
{
	ZQF::ZxJson::JValue jv;
	jv = 1;
	assert(jv.Get<size_t>() == 1);
	jv = 1.2;
	assert(jv.Get<double>() == 1.2);
	jv = "123";
	assert(jv.Get<std::string_view>() == "123");
	jv = std::string("5666");
	assert(jv.Get<std::string>() == "5666");
	assert(jv.Get<std::string&>() == "5666");

	ZQF::ZxJson::JValue jv1 = std::move(jv);
	assert(jv.Check<ZQF::ZxJson::JNull_t>());
	assert(jv1.Get<std::string>() == "5666");
	assert(jv1.Get<std::string&>() == "5666");


	ZQF::ZxJson::JArray_t jarr;
	jarr.emplace_back(1);
	jarr.emplace_back("12313");
	jarr.emplace_back(13.41);
	jarr.emplace_back(false);

	assert(jarr.size() == 4);
	assert(jarr[0].Get<size_t>() == 1);
	assert(jarr[1].Get<std::string_view>() == "12313");
	assert(jarr[2].Get<double>() == 13.41);
	assert(jarr[3].Get<bool>() == false);

	ZQF::ZxJson::JObject_t jobj;
	jobj["532532"] = 1;
	jobj["666"] = 3.11;
	jobj["777"] = "121431";
	jobj["ftftfr"] = false;

	assert(jobj["532532"].Get<size_t>() == 1);
	assert(jobj["666"].Get<double>() == 3.11);
	assert(jobj["777"].Get<std::string_view>() == "121431");
	assert(jobj["ftftfr"].Get<bool>() == false);

	ZQF::ZxJson::JArray_t jarr1 = std::move(jarr);
	assert(jarr.empty());

	jv = std::move(jarr1);
	assert(jarr1.empty());

	ZQF::ZxJson::JObject_t jobj1 = std::move(jobj);
	jobj1["arrayx"] = std::move(jv);
	assert(jv.Check<ZQF::ZxJson::JNull_t>());

	assert(jobj1["arrayx"][0].Get<size_t>() == 1);

	auto& jarr2 = jobj1["arrayx"].Get<ZQF::ZxJson::JArray_t&>();
	assert(jarr2.size() == 4);

	jv = std::move(jobj1);
	assert(jv["532532"].Get<size_t>() == 1);

	int a = 0;

}

static auto TestJsonParseRegularEscape() -> bool
{
	constexpr std::string_view str0 = R"JSON("123\n666\r4565\tefwe\"fawfw\\afjasf\bsafasf\fawfasf\fasf\tFDaf\\123")JSON";
	constexpr std::string_view str1 = "123\n666\r4565\tefwe\"fawfw\\afjasf\bsafasf\fawfasf\fasf\tFDaf\\123";
	ZQF::ZxJson::JValue jv;
	ZQF::ZxJson::JParser{ str0 }.Parse(jv);

	if (jv.Get<std::string_view>() != str1)
	{
		return false;
	}

	// test unknown escape
	try
	{
		constexpr std::string_view str2 = R"JSON("iurbguiwe\x14141")JSON";
		ZQF::ZxJson::JValue jv1;
		ZQF::ZxJson::JParser{ str2 }.Parse(jv1);
		return false;
	}
	catch (const std::exception& err)
	{
		if (::strcmp(err.what(), "ZxJson::JParser::ParseString: unknown escape character!"))
		{
			return false;
		}
	}

	return true;
}

static auto TestJsonParseUnicodeEscape() -> bool
{
	constexpr std::string_view str0 = R"JSON("\u5FAE\u79ef\u5206\u57fa\u672c\u5b9a\u7406\uff08Fundamental Theorem of Calculus\uff09\u53c8\u79f0\u5fae\u79ef\u5206\u57fa\u672c\u516c\u5f0f\uff0c\u8bc1\u5b9e\u5fae\u5206\u548c\u79ef\u5206\u4e92\u4e3a\u9006\u8fd0\u7b97")JSON";
	constexpr std::string_view str1 = R"JSON(微积分基本定理（Fundamental Theorem of Calculus）又称微积分基本公式，证实微分和积分互为逆运算)JSON";

	ZQF::ZxJson::JValue jv;
	ZQF::ZxJson::JParser{ str0 }.Parse(jv);
	return jv.Get<std::string_view>() == str1;
}


auto main() -> int
{
	try
	{
		// TestJsonParser();
		// TestJsonDumper();
		// TestJsonValue();
		// TestJsonParseRegularEscape();
		// TestJsonParseUnicodeEscape();
		BenchJsonParser();
	}
	catch (const std::exception& err)
	{
		std::println(std::cerr, "std::exception: {}", err.what());
	}

}