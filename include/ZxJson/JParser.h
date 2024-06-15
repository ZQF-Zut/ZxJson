#pragma once
#include <ZxJson/JValue.h>
#include <span>

namespace ZQF::ZxJson
{
class JParser
{
  private:
    size_t m_nReadBytes{};
    std::span<const char> m_spJson;

  public:
    JParser(std::span<const char> spJson);
    ~JParser();
    JParser(const JParser&) = delete;
    JParser(JParser&&) noexcept = delete;
    auto operator=(const JParser&) -> JParser& = delete;
    auto operator=(JParser&&) noexcept -> JParser& = delete;

    auto Parse(JValue& rfJValue) -> bool;

  private:
    auto CurPtr() -> const char*;
    auto GetReadBytes() const -> size_t; // NOLINT
    auto AddReadBytes(size_t nCount = 1) -> void;
    auto TotalBytes() const -> size_t; // NOLINT

    auto SkipWhite() -> char;
    auto NextToken() -> char;
    auto CurToken() -> char;

    auto ParseArray(JValue& rfJValue) -> void;
    auto ParseObject(JValue& rfJValue) -> void;
    auto ParseNumber(JValue& rfJValue) -> void;
    auto ParseString(JValue& rfJValue) -> void;
    auto ParseTrue(JValue& rfJValue) -> void;
    auto ParseFalse(JValue& rfJValue) -> void;
    auto ParseNull(JValue& rfJValue) -> void;
    auto ParseValue(JValue& rfJValue) -> void;
};
} // namespace ZQF::ZxJson
