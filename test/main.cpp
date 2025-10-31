#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <Zut/ZxJson.h>
#include <filesystem>

TEST_CASE("value")
{
  ZxJson::JValue json_value;

  // integral &&
  {
    json_value = 1;
    REQUIRE(json_value.GetNum() == 1);
    ZxJson::JValue jv_integral_rr{ 1 };
    REQUIRE(json_value.GetNum() == 1);
    REQUIRE(jv_integral_rr.GetNum() == 1);
  }

  // integral &
  {
    size_t num = 666;
    json_value = num;
    REQUIRE(json_value.GetNum() == num);
    ZxJson::JValue jv_integral_r{ num };
    REQUIRE(jv_integral_r.GetNum() == num);
  }

  // const integral &
  {
    const size_t num_c = 661236;
    json_value = num_c;
    REQUIRE(json_value.GetNum() == num_c);
    ZxJson::JValue jv_integral_cr{ num_c };
    REQUIRE(jv_integral_cr.GetNum() == num_c);
  }

  // floating &&
  {
    json_value = 1.2;
    REQUIRE(json_value.GetFloat() == 1.2);
    ZxJson::JValue jv_floating_rr{ 1.2 };
    REQUIRE(jv_floating_rr.GetFloat() == 1.2);
  }

  // floating &
  {
    double float_v = 1.6;
    json_value = float_v;
    REQUIRE(json_value.GetFloat() == float_v);
    ZxJson::JValue jv_floating_r{ float_v };
    REQUIRE(jv_floating_r.GetFloat() == float_v);
  }

  // const floating &
  {
    const double float_v_c = 1.5;
    json_value = float_v_c;
    REQUIRE(json_value.GetFloat() == float_v_c);
    ZxJson::JValue jv_floating_cr{ float_v_c };
    REQUIRE(jv_floating_cr.GetFloat() == float_v_c);
  }

  // const char[] &
  {
    json_value = "123";
    REQUIRE(json_value.GetStrView() == "123");
    ZxJson::JValue jv_char_rr{ "123" };
    REQUIRE(jv_char_rr.GetStrView() == "123");
  }

  // char[] &
  {
    char char_str_r[] = "greiojhger";               // NOLINT
    json_value = char_str_r;
    REQUIRE(json_value.GetStrView() == char_str_r); // NOLINT
    ZxJson::JValue jv_char_r{ char_str_r };
    REQUIRE(jv_char_r.GetStrView() == char_str_r);  // NOLINT
  }

  // const char*
  {
    const char* char_str_c_ptr = "aisgfuasfa";
    json_value = char_str_c_ptr;
    REQUIRE(json_value.GetStrView() == char_str_c_ptr);
    ZxJson::JValue jv_char_c_ptr{ char_str_c_ptr };
    REQUIRE(jv_char_c_ptr.GetStrView() == char_str_c_ptr);
  }

  // char*
  {
    std::string char_str_buffer{ "weih8wegn" };
    char* char_str_ptr = char_str_buffer.data();
    json_value = char_str_ptr;
    REQUIRE(json_value.GetStrView() == char_str_ptr);
    ZxJson::JValue jv_char_ptr{ char_str_ptr };
    REQUIRE(jv_char_ptr.GetStrView() == char_str_ptr);
  }

  // std::string&&
  {
    json_value = std::string("5666");
    REQUIRE(json_value.GetStr() == "5666");
    REQUIRE(json_value.GetStr() == "5666");
    ZxJson::JValue jv_str_rr{ std::string("5666") };
    REQUIRE(jv_str_rr.GetStrView() == "5666");
  }

  // std::string&
  {
    std::string txt_0 = "2352623";
    json_value = txt_0;
    REQUIRE(json_value.GetStr() == txt_0);
    REQUIRE(json_value.GetStr() == txt_0);
    ZxJson::JValue jv_str_r{ txt_0 };
    REQUIRE(jv_str_r.GetStrView() == txt_0);
  }

  // const std::string&
  {
    const std::string txt_1 = "asgfjisadf";
    json_value = txt_1;
    REQUIRE(json_value.GetStr() == txt_1);
    REQUIRE(json_value.GetStr() == txt_1);
    ZxJson::JValue jv_str_cr{ txt_1 };
    REQUIRE(jv_str_cr.GetStrView() == txt_1);
  }

  // std::string_view&&
  {
    json_value = std::string_view("5666");
    REQUIRE(json_value.GetStr() == "5666");
    REQUIRE(json_value.GetStrView() == "5666");
    ZxJson::JValue jv_sv_rr{ std::string_view("5666") };
    REQUIRE(jv_sv_rr.GetStrView() == "5666");
  }

  // std::string_view&
  {
    std::string_view txt_0_sv = "2352623";
    json_value = txt_0_sv;
    REQUIRE(json_value.GetStr() == txt_0_sv);
    REQUIRE(json_value.GetStrView() == txt_0_sv);
    ZxJson::JValue jv_sv_r{ txt_0_sv };
    REQUIRE(jv_sv_r.GetStrView() == txt_0_sv);
  }

  // const std::string_view&
  {
    const std::string_view txt_1_sv = "asgfjisadf";
    ;
    json_value = txt_1_sv;
    REQUIRE(json_value.GetStr() == txt_1_sv);
    REQUIRE(json_value.GetStrView() == txt_1_sv);
    ZxJson::JValue jv_sv_cr{ txt_1_sv };
    REQUIRE(jv_sv_cr.GetStrView() == txt_1_sv);
  }

  // JValue&&
  {
    ZxJson::JValue jv_c0_rr{ "gwegsgwe" };
    jv_c0_rr = ZxJson::JValue{ "gtk7ul98tnrt" };
    REQUIRE(jv_c0_rr.GetStrView() == "gtk7ul98tnrt");
    ZxJson::JValue jv_c2_rr{ std::move(jv_c0_rr) };
    REQUIRE(jv_c2_rr.GetStrView() == "gtk7ul98tnrt");
    REQUIRE(jv_c2_rr.GetStrView() == "gtk7ul98tnrt");
  }

  // JValue&
  {
    ZxJson::JValue jv_c0{ "3523" };
    ZxJson::JValue jv_c1{ "wesdjib" };
    jv_c0 = jv_c1;
    REQUIRE(jv_c0.GetStrView() == "wesdjib");
    ZxJson::JValue jv_c2{ jv_c0 };
    REQUIRE(jv_c0.GetStrView() == "wesdjib");
    REQUIRE(jv_c2.GetStrView() == "wesdjib");
  }

  // const JValue&
  {
    ZxJson::JValue jv_c0{ "gremiobnerobre" };
    const ZxJson::JValue jv_c1{ "ewgh9n4893gn893ndns9b3" };
    jv_c0 = jv_c1;
    REQUIRE(jv_c0.GetStrView() == "ewgh9n4893gn893ndns9b3");
    ZxJson::JValue jv_c2{ jv_c0 };
    REQUIRE(jv_c2.GetStrView() == "ewgh9n4893gn893ndns9b3");
    REQUIRE(jv_c2.GetStrView() == "ewgh9n4893gn893ndns9b3");
  }
}

TEST_CASE("move")
{
  ZxJson::JValue jv0{ "asgfjisadf" };
  ZxJson::JValue jv1;
  jv1.Move(std::move(jv0));
  REQUIRE(jv1.GetStr() == "asgfjisadf");
  REQUIRE(jv1.GetStrView() == "asgfjisadf");
}

TEST_CASE("copy")
{
  ZxJson::JValue jv0{ "asgfjisadf" };
  ZxJson::JValue jv1;
  jv1.Copy(jv0);
  REQUIRE(jv0.GetStrView() == "asgfjisadf");
  REQUIRE(jv1.GetStrView() == "asgfjisadf");
}

TEST_CASE("array")
{
  ZxJson::JArray_t jarr;
  jarr.emplace_back(1);
  jarr.emplace_back("12313");
  jarr.emplace_back(13.41);
  jarr.emplace_back(false);

  REQUIRE(jarr.size() == 4);
  REQUIRE(jarr[0].GetNum() == 1);
  REQUIRE(jarr[1].GetStrView() == "12313");
  REQUIRE(jarr[2].GetFloat() == 13.41);
  REQUIRE(jarr[3].GetBool() == false);

  ZxJson::JArray_t jarr_m = std::move(jarr);
  REQUIRE(jarr_m.size() == 4);
  REQUIRE(jarr_m[0].GetNum() == 1);
  REQUIRE(jarr_m[1].GetStrView() == "12313");
  REQUIRE(jarr_m[2].GetFloat() == 13.41);
  REQUIRE(jarr_m[3].GetBool() == false);

  ZxJson::JValue jval = std::move(jarr_m);
  REQUIRE(jval[0].GetNum() == 1);
  REQUIRE(jval[1].GetStrView() == "12313");
  REQUIRE(jval[2].GetFloat() == 13.41);
  REQUIRE(jval[3].GetBool() == false);

  REQUIRE(jval.At(1).GetStrView() == "12313");
  REQUIRE_THROWS(jval.At(4));
}

TEST_CASE("object")
{
  {
    ZxJson::JValue jv_null;
    jv_null["131412"] = 666;
    REQUIRE(jv_null["131412"].GetNum() == 666);
  }

  {
    ZxJson::JArray_t jarrx_0{ "12412", 12412, 21.123 };
    ZxJson::JArray_t jarrx_1{ "214234", 12421, 24124.2 };
    jarrx_0[0] = jarrx_1[0];
    jarrx_0 = jarrx_1;
    const auto& jarrray_c = jarrx_0;
    REQUIRE(jarrray_c[0].GetStrView() == "214234");
  }

  ZxJson::JArray_t jarr;
  jarr.emplace_back(1);
  jarr.emplace_back("12313");
  jarr.emplace_back(13.41);
  jarr.emplace_back(false);

  ZxJson::JArray_t jarr2;
  jarr2.emplace_back(4.34);
  jarr2.emplace_back(124);
  jarr2.emplace_back("asifhguiag");
  jarr2.emplace_back(true);
  jarr2.emplace_back(321231);

  ZxJson::JObject_t jobj;
  jobj["532532"] = 1;
  jobj["666"] = 3.11;
  jobj["777"] = "121431";
  jobj["ftftfr"] = false;
  jobj["jarray"] = std::move(jarr);
  jobj["jarray2"] = jarr2;
  jobj["jarray3"] = ZxJson::JArray_t{ 1234, "sufgsa", true };

  REQUIRE(jarr2.size() == 5);
  REQUIRE(jarr2[3].GetBool() == true);

  REQUIRE(jobj["532532"].GetNum() == 1);
  REQUIRE(jobj["666"].GetFloat() == 3.11);
  REQUIRE(jobj["777"].GetStrView() == "121431");
  REQUIRE(jobj["jarray"].Check<ZxJson::JArray_t>() == true);
  REQUIRE(jobj["jarray2"].Check<ZxJson::JArray_t>() == true);
  REQUIRE(jobj["jarray"][0].GetNum() == 1);
  REQUIRE(jobj["jarray2"][3].GetBool() == true);
  REQUIRE(jobj["jarray3"][1].GetStrView() == "sufgsa");

  jobj["jarray"][0] = ZxJson::JObject_t{ { "asfhu", 21312 }, { "fnawh", true }, { "asdiohuis", 1.2 } };
  REQUIRE(jobj["jarray"][0]["fnawh"].GetBool() == true);

  const auto jobj_const_test = [](const ZxJson::JObject_t& jobj) // NOLINT
  {
    REQUIRE(jobj.at("532532").GetNum() == 1);
    REQUIRE(jobj.at("666").GetFloat() == 3.11);
    REQUIRE(jobj.at("777").GetStrView() == "121431");
    REQUIRE(jobj.at("jarray").Check<ZxJson::JArray_t>() == true);
    REQUIRE(jobj.at("jarray2").Check<ZxJson::JArray_t>() == true);
    REQUIRE(jobj.at("jarray")[0].At("asfhu").GetNum() == 21312);
    REQUIRE(jobj.at("jarray2")[3].GetBool() == true);
    REQUIRE(jobj.at("jarray3")[1].GetStrView() == "sufgsa");
  };
  jobj_const_test(jobj);

  ZxJson::JValue jval = std::move(jobj);
  const ZxJson::JValue& jv_cr = jval;

  REQUIRE(jv_cr.At("532532").GetNum() == 1);
  REQUIRE(jv_cr.At("666").GetFloat() == 3.11);
  REQUIRE(jv_cr.At("777").GetStrView() == "121431");
  REQUIRE(jv_cr.At("jarray").Check<ZxJson::JArray_t>() == true);
  REQUIRE(jv_cr.At("jarray2").Check<ZxJson::JArray_t>() == true);
  REQUIRE(jv_cr.At("jarray")[0].At("asfhu").GetNum() == 21312);
  REQUIRE(jv_cr.At("jarray2")[3].GetBool() == true);
  REQUIRE(jv_cr.At("jarray3")[1].GetStrView() == "sufgsa");
}

TEST_CASE("test_dump_parse")
{
  const ZxJson::JValue jval{ ZxJson::JObject_t{
      { "21412", 2352 },
      { "segwe", 1.0 },
      { "egwe", "esighwe" },
  } };

  {
    const auto str = jval.Dump(false);
    REQUIRE(str.starts_with('{'));
    REQUIRE(str.find("\"21412\"") != std::string::npos);
    REQUIRE(str.find("2352") != std::string::npos);
    REQUIRE(str.find("\"segwe\"") != std::string::npos);
    REQUIRE(str.find("\"egwe\"") != std::string::npos);
    REQUIRE(str.find("\"esighwe\"") != std::string::npos);
    REQUIRE(str.ends_with('}'));
    REQUIRE(str.find('\n') == std::string::npos);
    REQUIRE(str.find('\t') == std::string::npos);
  }

  {
    const auto str_2 = jval.Dump(true);
    REQUIRE(str_2.starts_with('{'));
    REQUIRE(str_2.find("\"21412\"") != std::string::npos);
    REQUIRE(str_2.find("2352") != std::string::npos);
    REQUIRE(str_2.find("\"segwe\"") != std::string::npos);
    REQUIRE(str_2.find("\"egwe\"") != std::string::npos);
    REQUIRE(str_2.find("\"esighwe\"") != std::string::npos);
    REQUIRE(str_2.ends_with('}'));
    bool testv = ((str_2.find('\n') != std::string::npos) || (str_2.find('\t') != std::string::npos));
    REQUIRE(testv);
  }

  {
    const auto jval2 = ZxJson::LoadViaMemory(R"({"21412":2352,"segwe":1.000000,"egwe":"esighwe"})");
    REQUIRE(jval2.At("21412").GetNum() == 2352);
    REQUIRE(jval2.At("segwe").GetFloat() >= 0.99);
    REQUIRE(jval2.At("segwe").GetFloat() <= 1.00);
    REQUIRE(jval2.At("egwe").GetStrView() == "esighwe");
  }
}

TEST_CASE("parse_escape")
{
  constexpr std::string_view str0 = R"JSON("123\n666\r4565\tefwe\"fawfw\\afjasf\bsafasf\fawfasf\fasf\tFDaf\\123")JSON";
  constexpr std::string_view str1 = "123\n666\r4565\tefwe\"fawfw\\afjasf\bsafasf\fawfasf\fasf\tFDaf\\123";
  ZxJson::JValue jval;
  ZxJson::JParser{ str0 }.Parse(jval);
  REQUIRE(jval.GetStrView() == str1);

  // test unknown escape
  constexpr std::string_view str2 = R"JSON("iurbguiwe\x14141")JSON";
  ZxJson::JValue jv1;
  REQUIRE_THROWS(ZxJson::JParser{ str2 }.Parse(jv1));
}

TEST_CASE("parse_unicode_escape")
{
  constexpr std::string_view str0 =
      R"JSON("\u5FAE\u79ef\u5206\u57fa\u672c\u5b9a\u7406\uff08Fundamental Theorem of Calculus\uff09\u53c8\u79f0\u5fae\u79ef\u5206\u57fa\u672c\u516c\u5f0f\uff0c\u8bc1\u5b9e\u5fae\u5206\u548c\u79ef\u5206\u4e92\u4e3a\u9006\u8fd0\u7b97")JSON";
  constexpr std::string_view str1 = R"JSON(微积分基本定理（Fundamental Theorem of Calculus）又称微积分基本公式，证实微分和积分互为逆运算)JSON";

  ZxJson::JValue json_value;
  ZxJson::JParser{ str0 }.Parse(json_value);
  REQUIRE(json_value.GetStrView() == str1);
}

TEST_CASE("load_store")
{
  ZxJson::StoreViaFile(
      "test.json",
      ZxJson::JObject_t{
          { "qq", 123 },
          { "ger", 1.34234 },
          { "afsdgds", "segse" },
      },
      true, true);

  REQUIRE(std::filesystem::exists("test.json"));
  const auto jval = ZxJson::LoadViaFile("test.json");
  REQUIRE(jval.At("qq").GetNum() == 123);
  REQUIRE(jval.At("ger").GetFloat() > 1.3);
  REQUIRE(jval.At("ger").GetFloat() < 1.4);
  REQUIRE(jval.At("afsdgds").GetStrView() == "segse");

  REQUIRE(std::filesystem::remove("test.json"));
}

TEST_CASE("force_save")
{
  ZxJson::StoreViaFile("force_save_test.json", ZxJson::JObject_t{ { "qq", 123 } }, true, true);
  REQUIRE_THROWS(ZxJson::StoreViaFile("force_save_test.json", ZxJson::JObject_t{ { "qq", 123 } }, true, false));
  REQUIRE(std::filesystem::remove("force_save_test.json"));
}

TEST_CASE("utf8_file_name_save_load")
{
  const auto u8_file_name = u8"这是utf8的文件名.json";
  ZxJson::StoreViaFile(reinterpret_cast<const char*>(u8_file_name), ZxJson::JObject_t{ { "dsgsdgs", 2141241 } }, true, true);
  REQUIRE(std::filesystem::exists(u8_file_name));

  const auto jval = ZxJson::LoadViaFile(reinterpret_cast<const char*>(u8_file_name));
  REQUIRE(jval.At("dsgsdgs").GetNum() == 2141241);

  REQUIRE(std::filesystem::remove(u8_file_name));
}

TEST_CASE("reflex")
{
  struct Struct
  {
    double pos;
    int age;
    std::string sex;
    std::array<uint8_t, 6> arrx;
    std::vector<int> vec_x;
    std::vector<std::string> vec_xx;
    std::list<std::string> list_xx;
    std::map<std::string, std::size_t> map_xx;
  };

  const Struct org_obj{
    .pos = 10.22,
    .age = 11,
    .sex{ "23523" },
    .arrx{ "123" },
    .vec_x{ 1, 2, 3 },
    .vec_xx{ "10086", "666", "2233" },
    .list_xx{ "sgs", "sdoigs", "soihgew" },
    .map_xx{ { "131212", 666 }, { "349y83", 5474 } },
  };
  REQUIRE_NOTHROW(ZxJson::StoreViaFile("test_reflex.json", ZxJson::ReflexStore(org_obj), true, true));
  REQUIRE(std::filesystem::exists("test_reflex.json"));

  Struct reflex_obj{
    .pos = 32532.22,
    .age = 23523,
    .sex{ "23523" },
    .arrx{ "325" },
    .vec_x{ 1, 2, 3, 5, 6, 7 },
    .vec_xx{ "326", "236", "623" },
    .list_xx{ "623", "236", "236" },
    .map_xx{ { "623", 236 }, { "236", 236 } },
  };
  REQUIRE_NOTHROW(ZxJson::ReflexLoad(reflex_obj, ZxJson::LoadViaFile("test_reflex.json")));
  REQUIRE(std::abs(reflex_obj.pos - org_obj.pos) < 0.001);
  REQUIRE(reflex_obj.age == org_obj.age);
  REQUIRE(reflex_obj.sex == org_obj.sex);
  REQUIRE(reflex_obj.vec_x == org_obj.vec_x);
  REQUIRE(reflex_obj.vec_xx == org_obj.vec_xx);
  REQUIRE(reflex_obj.list_xx == org_obj.list_xx);
  REQUIRE(reflex_obj.map_xx == org_obj.map_xx);

  Struct reflex_obj2{};
  REQUIRE_NOTHROW(ZxJson::ReflexLoad(reflex_obj2, ZxJson::LoadViaFile("test_reflex.json")));
  REQUIRE(std::abs(reflex_obj2.pos - org_obj.pos) < 0.001);
  REQUIRE(reflex_obj2.age == org_obj.age);
  REQUIRE(reflex_obj2.sex == org_obj.sex);
  REQUIRE(reflex_obj2.vec_x == org_obj.vec_x);
  REQUIRE(reflex_obj2.vec_xx == org_obj.vec_xx);
  REQUIRE(reflex_obj2.list_xx == org_obj.list_xx);
  REQUIRE(reflex_obj2.map_xx == org_obj.map_xx);

  REQUIRE(std::filesystem::remove("test_reflex.json"));
}
