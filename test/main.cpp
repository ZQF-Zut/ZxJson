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


#define ZXTEST(MY_TEST_CONDITION) \
    if ((MY_TEST_CONDITION) == false) \
    { \
        std::println("assert failed -> line:{} -> file:{}", __LINE__, __FILE__); \
        std::abort(); \
    } \
    else \
    { \
        std::println("assert success -> line:{} -> file:{}", __LINE__, __FILE__); \
    } \

#define ZXTEST_EXCEPTION(EXCEPTION_CODE) \
try \
{ \
    EXCEPTION_CODE; \
    std::println("exception failed -> line:{} -> file:{}", __LINE__, __FILE__); \
    std::abort(); \
} \
catch (const std::exception&) \
{ \
    std::println("exception success -> line:{} -> file:{}", __LINE__, __FILE__); \
} \


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


    // # Utils #
[[maybe_unused]] static auto FetchFileData(const std::filesystem::path& phJson) -> std::string
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
[[maybe_unused]] static auto BenchJsonParser() -> void
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

[[maybe_unused]] static auto BenchJsonDumper() -> void
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
[[maybe_unused]] static auto TestJsonParser() -> void
{
    auto raw_data = ::FetchFileData(u8"1.json");
    auto json_value = ZQF::ZxJson::LoadViaMemory(raw_data);
}

[[maybe_unused]] static auto TestJsonDumper() -> void
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

[[maybe_unused]] static auto TestJsonValue() -> void
{
    {
        ZQF::ZxJson::JValue json_value;

        // integral &&
        {
            json_value = 1;
            ZXTEST(json_value.GetNum() == 1);
            ZQF::ZxJson::JValue jv_integral_rr{ 1 };
            ZXTEST(jv_integral_rr.GetNum() == 1);
        }

        // integral &
        {
            size_t num = 666;
            json_value = num;
            ZXTEST(json_value.GetNum() == num);
            ZQF::ZxJson::JValue jv_integral_r{ num };
            ZXTEST(jv_integral_r.GetNum() == num);
        }

        // const integral &
        {
            const size_t num_c = 661236;
            json_value = num_c;
            ZXTEST(json_value.GetNum() == num_c);
            ZQF::ZxJson::JValue jv_integral_cr{ num_c };
            ZXTEST(jv_integral_cr.GetNum() == num_c);
        }

        // floating &&
        {
            json_value = 1.2;
            ZXTEST(json_value.GetFloat() == 1.2);
            ZQF::ZxJson::JValue jv_floating_rr{ 1.2 };
            ZXTEST(jv_floating_rr.GetFloat() == 1.2);
        }

        // floating &
        {
            double float_v = 1.6;
            json_value = float_v;
            ZXTEST(json_value.GetFloat() == float_v);
            ZQF::ZxJson::JValue jv_floating_r{ float_v };
            ZXTEST(jv_floating_r.GetFloat() == float_v);
        }

        // const floating &
        {
            const double float_v_c = 1.5;
            json_value = float_v_c;
            ZXTEST(json_value.GetFloat() == float_v_c);
            ZQF::ZxJson::JValue jv_floating_cr{ float_v_c };
            ZXTEST(jv_floating_cr.GetFloat() == float_v_c);
        }

        // const char[] &
        {
            json_value = "123";
            ZXTEST(json_value.GetStrView() == "123");
            ZQF::ZxJson::JValue jv_char_rr{ "123" };
            ZXTEST(jv_char_rr.GetStrView() == "123");
        }

        // char[] &
        {
            char char_str_r[] = "greiojhger";
            json_value = char_str_r;
            ZXTEST(json_value.GetStrView() == char_str_r);
            ZQF::ZxJson::JValue jv_char_r{ char_str_r };
            ZXTEST(jv_char_r.GetStrView() == char_str_r);
        }

        // const char*
        {
            const char* char_str_c_ptr = "aisgfuasfa";
            json_value = char_str_c_ptr;
            ZXTEST(json_value.GetStrView() == char_str_c_ptr);
            ZQF::ZxJson::JValue jv_char_c_ptr{ char_str_c_ptr };
            ZXTEST(jv_char_c_ptr.GetStrView() == char_str_c_ptr);
        }

        // char*
        {
            std::string char_str_buffer{ "weih8wegn" };
            char* char_str_ptr = char_str_buffer.data();
            json_value = char_str_ptr;
            ZXTEST(json_value.GetStrView() == char_str_ptr);
            ZQF::ZxJson::JValue jv_char_ptr{ char_str_ptr };
            ZXTEST(jv_char_ptr.GetStrView() == char_str_ptr);
        }

        // std::string&&
        {
            json_value = std::string("5666");
            ZXTEST(json_value.GetStr() == "5666");
            ZXTEST(json_value.GetStr() == "5666");
            ZQF::ZxJson::JValue jv_str_rr{ std::string("5666") };
            ZXTEST(jv_str_rr.GetStrView() == "5666");
        }

        // std::string&
        {
            std::string txt_0 = "2352623";
            json_value = txt_0;
            ZXTEST(json_value.GetStr() == txt_0);
            ZXTEST(json_value.GetStr() == txt_0);
            ZQF::ZxJson::JValue jv_str_r{ txt_0 };
            ZXTEST(jv_str_r.GetStrView() == txt_0);
        }

        // const std::string&
        {
            const std::string txt_1 = "asgfjisadf";
            json_value = txt_1;
            ZXTEST(json_value.GetStr() == txt_1);
            ZXTEST(json_value.GetStr() == txt_1);
            ZQF::ZxJson::JValue jv_str_cr{ txt_1 };
            ZXTEST(jv_str_cr.GetStrView() == txt_1);
        }

        // std::string_view&&
        {
            json_value = std::string_view("5666");
            ZXTEST(json_value.GetStr() == "5666");
            ZXTEST(json_value.GetStrView() == "5666");
            ZQF::ZxJson::JValue jv_sv_rr{ std::string_view("5666") };
            ZXTEST(jv_sv_rr.GetStrView() == "5666");
        }

        // std::string_view&
        {
            std::string_view txt_0_sv = "2352623";
            json_value = txt_0_sv;
            ZXTEST(json_value.GetStr() == txt_0_sv);
            ZXTEST(json_value.GetStrView() == txt_0_sv);
            ZQF::ZxJson::JValue jv_sv_r{ txt_0_sv };
            ZXTEST(jv_sv_r.GetStrView() == txt_0_sv);
        }

        // const std::string_view&
        {
            const std::string_view txt_1_sv = "asgfjisadf";;
            json_value = txt_1_sv;
            ZXTEST(json_value.GetStr() == txt_1_sv);
            ZXTEST(json_value.GetStrView() == txt_1_sv);
            ZQF::ZxJson::JValue jv_sv_cr{ txt_1_sv };
            ZXTEST(jv_sv_cr.GetStrView() == txt_1_sv);
        }


        // JValue&&
        {
            ZQF::ZxJson::JValue jv_c0_rr{ "gwegsgwe" };
            jv_c0_rr = ZQF::ZxJson::JValue{ "gtk7ul98tnrt" };
            ZXTEST(jv_c0_rr.GetStrView() == "gtk7ul98tnrt");
            ZQF::ZxJson::JValue jv_c2_rr{ std::move(jv_c0_rr) };
            ZXTEST(jv_c2_rr.GetStrView() == "gtk7ul98tnrt");
            ZXTEST(jv_c2_rr.GetStrView() == "gtk7ul98tnrt");
        }

        // JValue&
        {
            ZQF::ZxJson::JValue jv_c0{ "3523" };
            ZQF::ZxJson::JValue jv_c1{ "wesdjib" };
            jv_c0 = jv_c1;
            ZXTEST(jv_c0.GetStrView() == "wesdjib");
            ZQF::ZxJson::JValue jv_c2{ jv_c0 };
            ZXTEST(jv_c0.GetStrView() == "wesdjib");
            ZXTEST(jv_c2.GetStrView() == "wesdjib");
        }

        // const JValue&
        {
            ZQF::ZxJson::JValue jv_c0{ "gremiobnerobre" };
            const ZQF::ZxJson::JValue jv_c1{ "ewgh9n4893gn893ndns9b3" };
            jv_c0 = jv_c1;
            ZXTEST(jv_c0.GetStrView() == "ewgh9n4893gn893ndns9b3");
            ZQF::ZxJson::JValue jv_c2{ jv_c0 };
            ZXTEST(jv_c2.GetStrView() == "ewgh9n4893gn893ndns9b3");
            ZXTEST(jv_c2.GetStrView() == "ewgh9n4893gn893ndns9b3");
        }
    }


    // JValue Move
    {
        ZQF::ZxJson::JValue jv0{ "asgfjisadf" };
        ZQF::ZxJson::JValue jv1;
        jv1.Move(std::move(jv0));
        ZXTEST(jv1.GetStr() == "asgfjisadf");
        ZXTEST(jv1.GetStrView() == "asgfjisadf");
    }

    // JValue Copy
    {
        ZQF::ZxJson::JValue jv0{ "asgfjisadf" };
        ZQF::ZxJson::JValue jv1;
        jv1.Copy(jv0);
        ZXTEST(jv0.GetStrView() == "asgfjisadf");
        ZXTEST(jv1.GetStrView() == "asgfjisadf");
    }

    // JArray
    {
        ZQF::ZxJson::JArray_t jarr;
        jarr.emplace_back(1);
        jarr.emplace_back("12313");
        jarr.emplace_back(13.41);
        jarr.emplace_back(false);

        ZXTEST(jarr.size() == 4);
        ZXTEST(jarr[0].GetNum() == 1);
        ZXTEST(jarr[1].GetStrView() == "12313");
        ZXTEST(jarr[2].GetFloat() == 13.41);
        ZXTEST(jarr[3].GetBool() == false);

        ZQF::ZxJson::JArray_t jarr_m = std::move(jarr);
        ZXTEST(jarr_m.size() == 4);
        ZXTEST(jarr_m[0].GetNum() == 1);
        ZXTEST(jarr_m[1].GetStrView() == "12313");
        ZXTEST(jarr_m[2].GetFloat() == 13.41);
        ZXTEST(jarr_m[3].GetBool() == false);

        ZQF::ZxJson::JValue jv = std::move(jarr_m);
        ZXTEST(jv[0].GetNum() == 1);
        ZXTEST(jv[1].GetStrView() == "12313");
        ZXTEST(jv[2].GetFloat() == 13.41);
        ZXTEST(jv[3].GetBool() == false);

        ZXTEST(jv.At(1).GetStrView() == "12313");
        ZXTEST_EXCEPTION(jv.At(4))
    }


    // JObject
    {
        ZQF::ZxJson::JArray_t jarr;
        jarr.emplace_back(1);
        jarr.emplace_back("12313");
        jarr.emplace_back(13.41);
        jarr.emplace_back(false);

        ZQF::ZxJson::JArray_t jarr2;
        jarr2.emplace_back(4.34);
        jarr2.emplace_back(124);
        jarr2.emplace_back("asifhguiag");
        jarr2.emplace_back(true);
        jarr2.emplace_back(321231);

        ZQF::ZxJson::JObject_t jobj;
        jobj["532532"] = 1;
        jobj["666"] = 3.11;
        jobj["777"] = "121431";
        jobj["ftftfr"] = false;
        jobj["jarray"] = std::move(jarr);
        jobj["jarray2"] = jarr2;
        jobj["jarray3"] = ZQF::ZxJson::JArray_t{ 1234, "sufgsa", true };

        ZXTEST(jarr2.size() == 5);
        ZXTEST(jarr2[3].GetBool() == true);

        ZXTEST(jobj["532532"].GetNum() == 1);
        ZXTEST(jobj["666"].GetFloat() == 3.11);
        ZXTEST(jobj["777"].GetStrView() == "121431");
        ZXTEST(jobj["jarray"].Check<ZQF::ZxJson::JArray_t>() == true);
        ZXTEST(jobj["jarray2"].Check<ZQF::ZxJson::JArray_t>() == true);
        ZXTEST(jobj["jarray"][0].GetNum() == 1);
        ZXTEST(jobj["jarray2"][3].GetBool() == true);
        ZXTEST(jobj["jarray3"][1].GetStrView() == "sufgsa");

        jobj["jarray"][0] = ZQF::ZxJson::JObject_t{ {"asfhu",21312}, {"fnawh",true}, {"asdiohuis",1.2} };
        ZXTEST(jobj["jarray"][0]["fnawh"].GetBool() == true);

        auto jobj_const_test = [](const ZQF::ZxJson::JObject_t& jobj) {
            ZXTEST(jobj.at("532532").GetNum() == 1);
            ZXTEST(jobj.at("666").GetFloat() == 3.11);
            ZXTEST(jobj.at("777").GetStrView() == "121431");
            ZXTEST(jobj.at("jarray").Check<ZQF::ZxJson::JArray_t>() == true);
            ZXTEST(jobj.at("jarray2").Check<ZQF::ZxJson::JArray_t>() == true);
            ZXTEST(jobj.at("jarray")[0].At("asfhu").GetNum() == 21312);
            ZXTEST(jobj.at("jarray2")[3].GetBool() == true);
            ZXTEST(jobj.at("jarray3")[1].GetStrView() == "sufgsa");
            };
        jobj_const_test(jobj);

        ZQF::ZxJson::JValue jv = std::move(jobj);
        const ZQF::ZxJson::JValue& jv_cr = jv;

        ZXTEST(jv_cr.At("532532").GetNum() == 1);
        ZXTEST(jv_cr.At("666").GetFloat() == 3.11);
        ZXTEST(jv_cr.At("777").GetStrView() == "121431");
        ZXTEST(jv_cr.At("jarray").Check<ZQF::ZxJson::JArray_t>() == true);
        ZXTEST(jv_cr.At("jarray2").Check<ZQF::ZxJson::JArray_t>() == true);
        ZXTEST(jv_cr.At("jarray")[0].At("asfhu").GetNum() == 21312);
        ZXTEST(jv_cr.At("jarray2")[3].GetBool() == true);
        ZXTEST(jv_cr.At("jarray3")[1].GetStrView() == "sufgsa");

    }

    //ZQF::ZxJson::JArray_t jarrx_0{ "12412", 12412, 21.123 };
    //ZQF::ZxJson::JArray_t jarrx_1{ "214234", 12421, 24124.2 };
    //jarrx_0[0] = jarrx_1[0];
    //jarrx_0 = jarrx_1;

    //const auto& jarrray_c = jarrx_0;
    //assert(jarrray_c[0].GetStrView() == "214234");

    //const auto& jobj1_c = json_value;
    //auto& xcx = jobj1_c.GetObject().at("234214");


    //[[maybe_unused]] int a = 0;

    {
        ZQF::ZxJson::JValue jv_null;
        jv_null["131412"] = 666;
    }

    std::println("all passed!");
}

[[maybe_unused]] static auto TestJsonParseRegularEscape() -> bool
{
    constexpr std::string_view str0 = R"JSON("123\n666\r4565\tefwe\"fawfw\\afjasf\bsafasf\fawfasf\fasf\tFDaf\\123")JSON";
    constexpr std::string_view str1 = "123\n666\r4565\tefwe\"fawfw\\afjasf\bsafasf\fawfasf\fasf\tFDaf\\123";
    ZQF::ZxJson::JValue jv;
    ZQF::ZxJson::JParser{ str0 }.Parse(jv);

    if (jv.GetStrView() != str1)
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

[[maybe_unused]] static auto TestJsonParseUnicodeEscape() -> bool
{
    constexpr std::string_view str0 = R"JSON("\u5FAE\u79ef\u5206\u57fa\u672c\u5b9a\u7406\uff08Fundamental Theorem of Calculus\uff09\u53c8\u79f0\u5fae\u79ef\u5206\u57fa\u672c\u516c\u5f0f\uff0c\u8bc1\u5b9e\u5fae\u5206\u548c\u79ef\u5206\u4e92\u4e3a\u9006\u8fd0\u7b97")JSON";
    constexpr std::string_view str1 = R"JSON(微积分基本定理（Fundamental Theorem of Calculus）又称微积分基本公式，证实微分和积分互为逆运算)JSON";

    ZQF::ZxJson::JValue json_value;
    ZQF::ZxJson::JParser{ str0 }.Parse(json_value);
    return json_value.GetStrView() == str1;
}

[[maybe_unused]] static auto TestLoadStoreViaFile() -> void
{
    auto json_value = ZQF::ZxJson::LoadViaFile("1.json");
    if (json_value["hash"].GetStrView() != "7954b83446bdb525c23a8a6677c498e6")
    {
        throw std::runtime_error("TestLoadViaFile(): failed");
    };
    ZQF::ZxJson::StoreViaFile("666.json", json_value, true, true);
}


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
