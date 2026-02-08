# ZxJson
personal json library

## Example
- value
```c++
#include <Zut/ZxJson.h>

auto main() -> int
{
    ZxJson::JValue jvalue_0{ 123 };
    ZxJson::JValue jvalue_1{ 131.310 };
    ZxJson::JValue jvalue_2{ "123124" };
    ZxJson::JValue jvalue_3{ std::string{ "123124" } };
    ZxJson::JValue jvalue_4{ std::string_view{ "123124" } };

    const auto jvalue_0_get_0 = jvalue_0.GetNum<size_t>();
    static_assert(std::is_same_v<decltype(jvalue_0_get_0), const size_t>);
    const auto jvalue_0_get_1 = jvalue_0.GetNum<uint32_t>();
    static_assert(std::is_same_v<decltype(jvalue_0_get_1), const uint32_t>);
    const auto jvalue_1_get_0 = jvalue_1.GetFloat<double_t>();
    static_assert(std::is_same_v<decltype(jvalue_1_get_0), const double_t>);
    const auto jvalue_2_get_0 = jvalue_2.GetStrView();
    static_assert(std::is_same_v<decltype(jvalue_2_get_0), const std::string_view>);

    ZxJson::JValue jvalue_array{};
    auto& jv_4_array = jvalue_array.ToArray();
    jv_4_array.emplace_back(3.14);
    jv_4_array.emplace_back(777);
    jv_4_array.emplace_back("123");
    assert(jv_4_array[1].GetNum<size_t>() == 777);
    assert(jvalue_array[1].GetNum<size_t>() == 777);
    assert(jvalue_array.At(1).GetNum<size_t>() == 777);

    ZxJson::JArray_t jarry{};
    jarry.emplace_back(2.11);
    jarry.emplace_back(666);
    jarry.emplace_back("321");
    assert(jarry[1].GetNum<size_t>() == 666);

    ZxJson::JObject_t jobj;
    jobj["age"] = 6;
    jobj["name"] = "xiao";
    jobj["my_array"] = jvalue_4;
    jobj["data_array"] = std::move(jarry);
    assert(jobj["name"].GetStrView() == "xiao");
    assert(jobj["data_array"][2].GetStrView() == "321");
}
```

- parse/load/store
```cpp
#include <Zut/ZxJson.h>

auto main() -> int
{
    constexpr std::string_view json_str = R"(
    {
      "name": "xiao",
      "age": 6,
      "data_list": [ 6, "1321" ]
    }
    )";
    const auto json_val = ZxJson::LoadViaMemory(json_str);
    assert(json_val.At("data_list").At(0).GetNum<size_t>() == 6);

    ZxJson::StoreViaFile("123.json", json_val, true, true);

    const auto json_val_2 = ZxJson::LoadViaFile("123.json");
    static_assert(std::is_same_v<decltype(json_val_2), const ZxJson::JValue>);
    assert(json_val_2.At("data_list").At(0).GetNum<size_t>() == 6);
}
```

- reflex
```cpp
#include <Zut/ZxJson.h>

auto main() -> int
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
    ZxJson::StoreViaFile("test_reflex.json", ZxJson::ReflexStore(org_obj), true, true);
    /* test_reflex.json
      {
         "pos": 10.220000,
         "age": 11,
         "sex": "23523",
         "arrx": [49,50,51,0,0,0],
         "map_xx": {"349y83": 5474,"131212": 666},
         "list_xx": ["sgs","sdoigs","soihgew"],
         "vec_x": [1,2,3],
         "vec_xx": ["10086","666","2233"]
      }
    */

    Struct reflex_obj{};
    ZxJson::ReflexLoad(reflex_obj, ZxJson::LoadViaFile("test_reflex.json"));
    assert(std::abs(reflex_obj.pos - org_obj.pos) < 0.001);
    assert(reflex_obj.age == org_obj.age);
    assert(reflex_obj.sex == org_obj.sex);
    assert(reflex_obj.vec_x == org_obj.vec_x);
    assert(reflex_obj.vec_xx == org_obj.vec_xx);
    assert(reflex_obj.list_xx == org_obj.list_xx);
    assert(reflex_obj.map_xx == org_obj.map_xx);

    Struct reflex_obj2{};
    ZxJson::ReflexLoad(reflex_obj2, ZxJson::LoadViaFile("test_reflex.json"));
    assert(std::abs(reflex_obj2.pos - org_obj.pos) < 0.001);
    assert(reflex_obj2.age == org_obj.age);
    assert(reflex_obj2.sex == org_obj.sex);
    assert(reflex_obj2.vec_x == org_obj.vec_x);
    assert(reflex_obj2.vec_xx == org_obj.vec_xx);
    assert(reflex_obj2.list_xx == org_obj.list_xx);
    assert(reflex_obj2.map_xx == org_obj.map_xx);
}
```

