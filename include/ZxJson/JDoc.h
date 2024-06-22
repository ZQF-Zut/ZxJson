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
    ~JDoc();
    JDoc(const std::string_view msPath);

    JDoc(const JDoc&) = delete;
    JDoc(JDoc&&) noexcept = delete;
    auto operator=(const JDoc&) -> JDoc& = delete;
    auto operator=(JDoc&&) noexcept -> JDoc& = delete;

    auto GetJValue() -> JValue&;
    auto GetJArray() -> JArray_t&;
    auto GetJObject() -> JObject_t&;
    auto LoadViaMemory(std::span<char> spData) -> bool;
    auto LoadViaFile(const std::string_view msPath) -> bool;
    auto StoreViaMemory(bool isFormat) const -> std::string;                                         // NOLINT
    auto StoreViaFile(const std::string_view msPath, bool isFormat, bool isForceSave) const -> void; // NOLINT
};
} // namespace ZQF::ZxJson