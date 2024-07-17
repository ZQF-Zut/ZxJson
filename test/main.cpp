#include <ZxJson/JValue.h>
#include <ZxJson/JParser.h>
#include <ZxJson/JDoc.h>
#include <ZxJson/JIO.h>
#include <ZxJson/Platform.h>
#include <cassert>
#include <chrono>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <print>


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
} // namespace ZQF


namespace
{
    // # Utils #
    [[maybe_unused]] auto FetchFileData(const std::filesystem::path& phJson) -> std::string
    {
        std::string data;
        std::ifstream ifs{ phJson };
        if (!ifs) { throw std::runtime_error(std::format("open file error: {}", phJson.string())); }
        ifs.seekg(0, std::ios::end);
        data.reserve(ifs.tellg());
        ifs.seekg(0, std::ios::beg);
        data.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        return data;
    }

    // # Bench #
    [[maybe_unused]] auto BenchJsonParser() -> void
    {
        ZQF::ZxRecord record;

        auto raw_data = ::FetchFileData(u8"1_あてな　１.txt.json");
        for (size_t i = 0; i < 200; i++)
        {
            record.Beg();
            auto json_value = ZQF::ZxJson::LoadViaMemory(raw_data);
            record.End();
        }

        record.Log();
    }

    [[maybe_unused]] auto BenchJsonDumper() -> void
    {
        ZQF::ZxRecord record;

        ZQF::ZxJson::JDoc jdoc{ "1_あてな　１.txt.json" };
        auto& json_value = jdoc.GetJValue();
        for (size_t i = 0; i < 200; i++)
        {
            record.Beg();
            auto dump_str = ZQF::ZxJson::StoreViaMemory(json_value, true);
            dump_str[0] = {};
            record.End();
        }

        jdoc.StoreViaMemory(true);

        record.Log();
    }

    // # Test #
    [[maybe_unused]] auto TestJsonParser() -> void
    {
        auto raw_data = ::FetchFileData(u8"1.json");
        auto json_value = ZQF::ZxJson::LoadViaMemory(raw_data);
    }

    [[maybe_unused]] auto TestJsonDumper() -> void
    {
        auto raw_data = ::FetchFileData(u8"1.json");
        auto json_value = ZQF::ZxJson::LoadViaMemory(raw_data);

        std::string data0;
        json_value.Dump(data0, true, 0);
        assert(data0.size());

        std::string data1;
        json_value.Dump(data1, false, 0);
        assert(data0.size());
    }

    [[maybe_unused]] auto TestJsonValue() -> void
    {
        ZQF::ZxJson::JValue json_value;
        json_value = 1;
        assert(json_value.Get<size_t>() == 1);
        json_value = 1.2;
        assert(json_value.Get<double>() == 1.2);
        json_value = "123";
        assert(json_value.Get<std::string_view>() == "123");
        json_value = std::string("5666");
        assert(json_value.Get<std::string>() == "5666");
        assert(json_value.Get<std::string&>() == "5666");

        ZQF::ZxJson::JValue jv1 = std::move(json_value);
        assert(json_value.Check<ZQF::ZxJson::JNull_t>());
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
        json_value = std::move(jarr1);

        ZQF::ZxJson::JObject_t jobj1 = std::move(jobj);
        jobj1["arrayx"] = std::move(json_value);
        assert(json_value.Check<ZQF::ZxJson::JNull_t>());

        assert(jobj1["arrayx"][0].Get<size_t>() == 1);

        [[maybe_unused]] auto& jarr2 = jobj1["arrayx"].Get<ZQF::ZxJson::JArray_t&>();
        assert(jarr2.size() == 4);

        json_value = std::move(jobj1);
        assert(json_value["532532"].Get<size_t>() == 1);

        [[maybe_unused]] int a = 0;
    }

    [[maybe_unused]] auto TestJsonParseRegularEscape() -> bool
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

    [[maybe_unused]] auto TestJsonParseUnicodeEscape() -> bool
    {
        constexpr std::string_view str0 = R"JSON("\u5FAE\u79ef\u5206\u57fa\u672c\u5b9a\u7406\uff08Fundamental Theorem of Calculus\uff09\u53c8\u79f0\u5fae\u79ef\u5206\u57fa\u672c\u516c\u5f0f\uff0c\u8bc1\u5b9e\u5fae\u5206\u548c\u79ef\u5206\u4e92\u4e3a\u9006\u8fd0\u7b97")JSON";
        constexpr std::string_view str1 = R"JSON(微积分基本定理（Fundamental Theorem of Calculus）又称微积分基本公式，证实微分和积分互为逆运算)JSON";

        ZQF::ZxJson::JValue json_value;
        ZQF::ZxJson::JParser{ str0 }.Parse(json_value);
        return json_value.Get<std::string_view>() == str1;
    }

    [[maybe_unused]] auto TestLoadStoreViaFile() -> void
    {
        auto json_value = ZQF::ZxJson::LoadViaFile("1.json");
        if (json_value["hash"].Get<std::string_view>() != "7954b83446bdb525c23a8a6677c498e6")
        {
            throw std::runtime_error("TestLoadViaFile(): failed");
        };
        ZQF::ZxJson::StoreViaFile("666.json", json_value, true, true);
    }
} // namespace


auto main(void) -> int
{
    try
    {
        // TestLoadStoreViaFile();
        // TestJsonParser();
        // TestJsonDumper();
        // TestJsonValue();
        // TestJsonParseRegularEscape();
        // TestJsonParseUnicodeEscape();
        // BenchJsonDumper();
        BenchJsonParser();
    }
    catch (const std::exception& err)
    {
        std::println(std::cerr, "std::exception: {}", err.what());
    }
}
