#pragma once
#include <span>
#include <string>
#include <string_view>
#include <Zut/ZxJson/JValue.h>

namespace ZQF::Zut::ZxJson
{
  class JDoc
  {
  private:
    JValue m_JValue;

  public:
    JDoc() = default;
    ~JDoc();
    JDoc(const std::string_view msPath);

    JDoc(const JDoc&) = delete;
    JDoc(JDoc&&) noexcept = delete;
    auto operator=(const JDoc&) -> JDoc& = delete;
    auto operator=(JDoc&&) noexcept -> JDoc& = delete;

    auto GetJValue() -> JValue&;
    auto GetJArray() -> JArray_t&;
    auto GetJObject() -> JObject_t&;
    auto LoadViaMemory(const std::span<const char> spData) -> bool;
    auto LoadViaFile(const std::string_view msPath) -> bool;
    auto StoreViaMemory(bool isFormat) const -> std::string;
    auto StoreViaFile(const std::string_view msPath, const bool isFormat, const bool isForceSave) const -> void;
  };
} // namespace ZQF::Zut::ZxJson
