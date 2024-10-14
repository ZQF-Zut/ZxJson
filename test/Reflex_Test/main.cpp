#include <print>
#include <iostream>
#include <Zut/ZxJson.h>


struct MyStruct
{
    double pos{ 10.233 };
    int age{ 200 };
    std::string sex{ "10086" };
    std::array<uint8_t, 6> arrx{ "123" };
    std::vector<int> vec_x{ 1,2,3 };
    std::vector<std::string> vec_xx{ "10086","666","2233" };
    std::list<std::string> list_xx{ "sgs","sdoigs","soihgew" };
    std::map<std::string, size_t> map_xx{ {"131212",666},{"349y83", 5474} };

};

struct MyStruct2
{
    double pos{ };
    int age{ };
    std::string sex;
    std::array<uint8_t, 6> arrx{};
    std::vector<int> vec_x;
    std::vector<std::string> vec_xx;
    std::list<std::string> list_xx;
    std::map<std::string, size_t> map_xx;
};


auto main(void) -> int
{
    try
    {
        {
            MyStruct2 obj{ 10.22,11,"32523",{ "123" }, { 1,2,3 },{ "10086","666","2233" },{ "sgs","sdoigs","soihgew" },{ {"131212",666},{"349y83", 5474} } };
            ZxJson::StoreViaFile("123.json", ZxJson::ReflexStore(obj), true, true);
        }

        {
            MyStruct2 obj{};
            ZxJson::ReflexLoad(obj, ZxJson::LoadViaFile("123.json"));
        }
    }
    catch (const std::exception& err)
    {
        std::println(std::cerr, "std::exception: {}", err.what());
    }
}
