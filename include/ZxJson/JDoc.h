#pragma once
#include <ZxJson/JParser.h>

namespace ZQF::ZxJson
{
class JDoc
{
  private:
    JValue m_JValue;

  public:
    JDoc();
    JDoc(const JDoc&) = delete;
    JDoc(JDoc&&) noexcept = delete;
    auto operator=(const JDoc&) -> JDoc& = delete;
    auto operator=(JDoc&&) noexcept -> JDoc& = delete;
    JDoc(const std::string_view msPath);
    ~JDoc();

    auto GetJValue() -> JValue&;
    auto GetJArray() -> JArray_t&;
    auto GetJObject() -> JObject_t&;
    auto LoadViaFile(const std::string_view msPath) -> bool;
    auto LoadViaMemory(std::span<char> spData) -> bool;
    auto Dump(bool isFormat) const -> std::string;
};

auto LoadViaMemory(std::span<char> spData) -> JValue;
auto LoadViaFile(const std::string_view msPath) -> JValue;
auto StoreViaString(JValue& rfJValue, bool isFormat) -> std::string;
auto StoreViaFile(const std::string_view msPath, JValue& rfJValue, bool isFormat, bool isForceSave) -> void;
} // namespace ZQF::ZxJson