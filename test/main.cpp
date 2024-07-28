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

        // integral &&
        {
            json_value = 1;
            assert(json_value.GetNum() == 1);
            ZQF::ZxJson::JValue jv_integral_rr{ 1 };
            assert(jv_integral_rr.GetNum() == 1);
        }

        // integral &
        {
            size_t num = 666;
            json_value = num;
            assert(json_value.GetNum() == num);
            ZQF::ZxJson::JValue jv_integral_r{ num };
            assert(jv_integral_r.GetNum() == num);
        }

        // const integral &
        {
            const size_t num_c = 661236;
            json_value = num_c;
            assert(json_value.GetNum() == num_c);
            ZQF::ZxJson::JValue jv_integral_cr{ num_c };
            assert(jv_integral_cr.GetNum() == num_c);
        }

        // floating &&
        {
            json_value = 1.2;
            assert(json_value.GetFloat() == 1.2);
            ZQF::ZxJson::JValue jv_floating_rr{ 1.2 };
            assert(jv_floating_rr.GetFloat() == 1.2);
        }

        // floating &
        {
            double float_v = 1.6;
            json_value = float_v;
            assert(json_value.GetFloat() == float_v);
            ZQF::ZxJson::JValue jv_floating_r{ float_v };
            assert(jv_floating_r.GetFloat() == float_v);
        }

        // const floating &
        {
            const double float_v_c = 1.5;
            json_value = float_v_c;
            assert(json_value.GetFloat() == float_v_c);
            ZQF::ZxJson::JValue jv_floating_cr{ float_v_c };
            assert(jv_floating_cr.GetFloat() == float_v_c);
        }

        // const char[] &
        {
            json_value = "123";
            assert(json_value.GetStrV() == "123");
            ZQF::ZxJson::JValue jv_char_rr{ "123" };
            assert(jv_char_rr.GetStrV() == "123");
        }

        // char[] &
        {
            char char_str_r[] = "greiojhger";
            json_value = char_str_r;
            assert(json_value.GetStrV() == char_str_r);
            ZQF::ZxJson::JValue jv_char_r{ char_str_r };
            assert(jv_char_r.GetStrV() == char_str_r);
        }

        // const char*
        {
            const char* char_str_c_ptr = "aisgfuasfa";
            json_value = char_str_c_ptr;
            assert(json_value.GetStrV() == char_str_c_ptr);
            ZQF::ZxJson::JValue jv_char_c_ptr{ char_str_c_ptr };
            assert(jv_char_c_ptr.GetStrV() == char_str_c_ptr);
        }

        // char*
        {
            std::string char_str_buffer{ "weih8wegn" };
            char* char_str_ptr = char_str_buffer.data();
            json_value = char_str_ptr;
            assert(json_value.GetStrV() == char_str_ptr);
            ZQF::ZxJson::JValue jv_char_ptr{ char_str_ptr };
            assert(jv_char_ptr.GetStrV() == char_str_ptr);
        }

        // std::string&&
        {
            json_value = std::string("5666");
            assert(json_value.GetStr() == "5666");
            assert(json_value.GetStr() == "5666");
            ZQF::ZxJson::JValue jv_str_rr{ std::string("5666") };
            assert(jv_str_rr.GetStrV() == "5666");
        }

        // std::string&
        {
            std::string txt_0 = "2352623";
            json_value = txt_0;
            assert(json_value.GetStr() == txt_0);
            assert(json_value.GetStr() == txt_0);
            ZQF::ZxJson::JValue jv_str_r{ txt_0 };
            assert(jv_str_r.GetStrV() == txt_0);
        }

        // const std::string&
        {
            const std::string txt_1 = "asgfjisadf";
            json_value = txt_1;
            assert(json_value.GetStr() == txt_1);
            assert(json_value.GetStr() == txt_1);
            ZQF::ZxJson::JValue jv_str_cr{ txt_1 };
            assert(jv_str_cr.GetStrV() == txt_1);
        }

        // std::string_view&&
        {
            json_value = std::string_view("5666");
            assert(json_value.GetStr() == "5666");
            assert(json_value.GetStrV() == "5666");
            ZQF::ZxJson::JValue jv_sv_rr{ std::string_view("5666") };
            assert(jv_sv_rr.GetStrV() == "5666");
        }

        // std::string_view&
        {
            std::string_view txt_0_sv = "2352623";
            json_value = txt_0_sv;
            assert(json_value.GetStr() == txt_0_sv);
            assert(json_value.GetStrV() == txt_0_sv);
            ZQF::ZxJson::JValue jv_sv_r{ txt_0_sv };
            assert(jv_sv_r.GetStrV() == txt_0_sv);
        }

        // const std::string_view&
        {
            const std::string_view txt_1_sv = "asgfjisadf";;
            json_value = txt_1_sv;
            assert(json_value.GetStr() == txt_1_sv);
            assert(json_value.GetStrV() == txt_1_sv);
            ZQF::ZxJson::JValue jv_sv_cr{ txt_1_sv };
            assert(jv_sv_cr.GetStrV() == txt_1_sv);
        }


        // JValue&&
        {
            ZQF::ZxJson::JValue jv_c0_rr{ "gwegsgwe" };
            jv_c0_rr = ZQF::ZxJson::JValue{ "gtk7ul98tnrt" };
            assert(jv_c0_rr.GetStrV() == "gtk7ul98tnrt");
            ZQF::ZxJson::JValue jv_c2_rr{ std::move(jv_c0_rr) };
            assert(jv_c2_rr.GetStrV() == "gtk7ul98tnrt");
            assert(jv_c2_rr.GetStrV() == "gtk7ul98tnrt");
        }

        // JValue&
        {
            ZQF::ZxJson::JValue jv_c0{ "3523" };
            ZQF::ZxJson::JValue jv_c1{ "wesdjib" };
            jv_c0 = jv_c1;
            assert(jv_c0.GetStrV() == "wesdjib");
            ZQF::ZxJson::JValue jv_c2{ jv_c0 };
            assert(jv_c0.GetStrV() == "wesdjib");
            assert(jv_c2.GetStrV() == "wesdjib");
        }

        // const JValue&
        {
            ZQF::ZxJson::JValue jv_c0{ "gremiobnerobre" };
            const ZQF::ZxJson::JValue jv_c1{ "ewgh9n4893gn893ndns9b3" };
            jv_c0 = jv_c1;
            assert(jv_c0.GetStrV() == "ewgh9n4893gn893ndns9b3");
            ZQF::ZxJson::JValue jv_c2{ jv_c0 };
            assert(jv_c2.GetStrV() == "ewgh9n4893gn893ndns9b3");
            assert(jv_c2.GetStrV() == "ewgh9n4893gn893ndns9b3");
        }


        ZQF::ZxJson::JValue jv1 = std::move(json_value);
        assert(jv1.GetStr() == "asgfjisadf");
        assert(jv1.GetStr() == "asgfjisadf");

        ZQF::ZxJson::JArray_t jarr;
        jarr.emplace_back(1);
        jarr.emplace_back("12313");
        jarr.emplace_back(13.41);
        jarr.emplace_back(false);

        assert(jarr.size() == 4);
        assert(jarr[0].GetNum() == 1);
        assert(jarr[1].GetStrV() == "12313");
        assert(jarr[2].GetFloat() == 13.41);
        assert(jarr[3].GetBool() == false);

        ZQF::ZxJson::JObject_t jobj;
        jobj["532532"] = 1;
        jobj["666"] = 3.11;
        jobj["777"] = "121431";
        jobj["ftftfr"] = false;

        assert(jobj["532532"].GetNum() == 1);
        assert(jobj["666"].GetFloat() == 3.11);
        assert(jobj["777"].GetStrV() == "121431");
        assert(jobj["ftftfr"].GetBool() == false);

        ZQF::ZxJson::JArray_t jarr1 = std::move(jarr);
        json_value = std::move(jarr1);

        ZQF::ZxJson::JObject_t jobj1 = std::move(jobj);
        jobj1["arrayx"] = std::move(json_value);

        assert(jobj1["arrayx"][0].GetNum() == 1);

        [[maybe_unused]] const auto& jarr2 = jobj1["arrayx"].GetArray();
        assert(jarr2.size() == 4);

        json_value = std::move(jobj1);
        assert(json_value["532532"].GetNum() == 1);


        ZQF::ZxJson::JArray_t jarrx_0{ "12412", 12412, 21.123 };
        ZQF::ZxJson::JArray_t jarrx_1{ "214234", 12421, 24124.2 };
        jarrx_0[0] = jarrx_1[0];
        jarrx_0 = jarrx_1;


        [[maybe_unused]] int a = 0;
    }

    [[maybe_unused]] auto TestJsonParseRegularEscape() -> bool
    {
        constexpr std::string_view str0 = R"JSON("123\n666\r4565\tefwe\"fawfw\\afjasf\bsafasf\fawfasf\fasf\tFDaf\\123")JSON";
        constexpr std::string_view str1 = "123\n666\r4565\tefwe\"fawfw\\afjasf\bsafasf\fawfasf\fasf\tFDaf\\123";
        ZQF::ZxJson::JValue jv;
        ZQF::ZxJson::JParser{ str0 }.Parse(jv);

        if (jv.GetStrV() != str1)
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
        return json_value.GetStrV() == str1;
    }

    [[maybe_unused]] auto TestLoadStoreViaFile() -> void
    {
        auto json_value = ZQF::ZxJson::LoadViaFile("1.json");
        if (json_value["hash"].GetStrV() != "7954b83446bdb525c23a8a6677c498e6")
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
        TestJsonValue();
        // TestJsonParseRegularEscape();
        // TestJsonParseUnicodeEscape();
        // BenchJsonDumper();
        // BenchJsonParser();
    }
    catch (const std::exception& err)
    {
        std::println(std::cerr, "std::exception: {}", err.what());
    }
}
