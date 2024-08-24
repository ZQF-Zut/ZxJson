#pragma once
#include <span>
#include <string_view>
#include <ZxJson/JValue.h>


namespace ZQF::ZxJson
{
    auto LoadViaMemory(std::span<const char> spData) -> JValue;
    auto LoadViaFile(const std::string_view msPath) -> JValue;
    auto StoreViaMemory(JValue& rfJValue, bool isFormat) -> std::string;
    auto StoreViaFile(const std::string_view msPath, JValue& rfJValue, bool isFormat, bool isForceSave) -> void;
} // namespace ZQF::ZxJson
