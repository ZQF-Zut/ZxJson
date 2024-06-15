#include <ZxJson/JDoc.h>
#include <ZxJson/Platform.h>

namespace ZQF::ZxJson
{
JDoc::JDoc(){

};

JDoc::JDoc(const std::string_view msPath)
{
    this->LoadViaFile(msPath);
};

JDoc::~JDoc(){

};

auto JDoc::GetJValue() -> JValue&
{
    return m_JValue;
}

auto JDoc::GetJArray() -> JArray_t&
{
    return m_JValue.Sure<JArray_t&>();
}

auto JDoc::GetJObject() -> JObject_t&
{
    return m_JValue.Sure<JObject_t&>();
}

auto JDoc::LoadViaFile(const std::string_view msPath) -> bool
{
    auto [file_size, file_data] = ZxJson::RreadAllBytes(msPath);
    return this->LoadViaMemory(std::span{ file_data.get(), file_size });
}

auto JDoc::LoadViaMemory(std::span<char> spData) -> bool
{
    return JParser{ spData }.Parse(m_JValue);
}

auto JDoc::Dump(bool isFormat) const -> std::string
{
    std::string json;
    m_JValue.Dump(json, isFormat, 0);
    return json;
}

auto LoadViaFile(const std::string_view msPath) -> JValue
{
    auto [file_size, file_data] = ZxJson::RreadAllBytes(msPath);
    return ZxJson::LoadViaMemory(std::span{ file_data.get(), file_size });
}

auto LoadViaMemory(std::span<char> spData) -> JValue
{
    JValue json_value;
    JParser{ spData }.Parse(json_value);
    return json_value;
}

auto StoreViaString(JValue& rfJValue, bool isFormat) -> std::string
{
    std::string dump_str;
    rfJValue.Dump(dump_str, isFormat, 0);
    return dump_str;
}

auto StoreViaFile(const std::string_view msPath, JValue& rfJValue, bool isFormat, bool isForceSave) -> void
{
    std::string dump_str = ZxJson::StoreViaString(rfJValue, isFormat);
    ZxJson::WriteAllBytes(msPath, dump_str, isForceSave);
}
} // namespace ZQF::ZxJson