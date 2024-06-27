# ZxJson
personal json library

## Example
- value
```c++
#include <ZxJson/JValue.h>

using namespace ZQF;

auto main() -> int
{
    ZxJson::JValue jvalue_0{ 123 };
    ZxJson::JValue jvalue_1{ 131.310 };
    ZxJson::JValue jvalue_2{ "123124" };
    ZxJson::JValue jvalue_3{ std::string{"123124"} };

    auto jvalue_0_get_0 = jvalue_0.Get<size_t>();
    static_assert(std::is_same_v<decltype(jvalue_0_get_0), size_t>);
    auto jvalue_0_get_1 = jvalue_0.Get<uint32_t>();
    static_assert(std::is_same_v<decltype(jvalue_0_get_1), uint32_t>);
    auto jvalue_1_get_0 = jvalue_1.Get<double_t>();
    static_assert(std::is_same_v<decltype(jvalue_1_get_0), double_t>);
    auto jvalue_2_get_0 = jvalue_2.Get<std::string_view>();
    static_assert(std::is_same_v<decltype(jvalue_2_get_0), std::string_view>);

    ZxJson::JValue jvalue_4{};
    auto& jv_4_array = jvalue_4.Sure<ZxJson::JArray_t&>();
    jv_4_array.emplace_back(3.14);
    jv_4_array.emplace_back(777);
    jv_4_array.emplace_back("123");
    assert(jv_4_array[1].Get<size_t>() == 777);
    assert(jvalue_4[1].Get<size_t>() == 777);

    ZxJson::JArray_t jarry{};
    jarry.emplace_back(2.11);
    jarry.emplace_back(666);
    jarry.emplace_back("321");
    assert(jarry[1].Get<size_t>() == 666);

    ZxJson::JObject_t jobj;
    jobj["age"] = 6;
    jobj["name"] = "xiao";
    jobj["data_arrat"] = std::move(jarry);
    assert(jobj["name"].Get<std::string_view>() == "xiao");
    assert(jobj["data_arrat"][2].Get<std::string_view>() == "321");
}
```

- parser
```cpp
#include <ZxJson/JIO.h>
#include <ZxJson/JValue.h>

using namespace ZQF;

auto main() -> int
{
    auto my_jv = ZxJson::LoadViaFile("123.json");
    static_assert(std::is_same_v<decltype(my_jv), ZxJson::JValue>);

    std::string_view json_str =
    R"(
    {
	"name": "xiao",
	"age": 6,
	"data_list": [ 6, "1321" ]
    }
    )";

    auto my_jv2 = ZxJson::LoadViaMemory(json_str);
    assert(my_jv2["data_list"][0].Get<size_t>() == 6);

}
```
