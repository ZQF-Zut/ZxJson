#include <ZxJson/JIO.h>
#include <ZxJson/JParser.h>
#include <ZxJson/Platform.h>


namespace ZQF::ZxJson
{
    auto LoadViaMemory(std::span<char> spData) -> JValue
    {
        JValue value;
        JParser{ spData }.Parse(value);
        return value;
    }

    auto LoadViaFile(const std::string_view msPath) -> JValue
    {
        auto [file_size, file_data] = ZxJson::ReadAllBytes(msPath);
        return ZxJson::LoadViaMemory(std::span{ file_data.get(), file_size });
    }

    auto StoreViaMemory(JValue& rfJValue, bool isFormat) -> std::string
    {
        std::string dump_str;
        rfJValue.Dump(dump_str, isFormat, 0);
        return dump_str;
    }

    auto StoreViaFile(const std::string_view msPath, JValue& rfJValue, bool isFormat, bool isForceSave) -> void
    {
        auto dump_str = ZxJson::StoreViaMemory(rfJValue, isFormat);
        ZxJson::WriteAllBytes(msPath, dump_str, isForceSave);
    }
} // namespace ZQF::ZxJson
