#pragma once
#include <span>
#include <string_view>
#include <ZxJson/JValue.h>


namespace ZQF::ZxJson
{
    auto LoadViaMemory(const std::span<const char> spData) -> JValue;
    auto LoadViaFile(const std::string_view msPath) -> JValue;
    auto StoreViaMemory(const JValue& rfJValue, const bool isFormat) -> std::string;
    auto StoreViaFile(const std::string_view msPath, const JValue& rfJValue, const bool isFormat, const bool isForceSave) -> void;
} // namespace ZQF::ZxJson