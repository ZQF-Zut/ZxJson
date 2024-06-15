#pragma once
#include <span>
#include <ZxJson/JValue.h>

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
    JParser& operator=(const JParser&) = delete;
    JParser& operator=(JParser&&) noexcept = delete;

    auto Parse(JValue& rfJValue) -> bool;

  private:
    auto CurPtr() -> const char*;
    auto GetReadBytes() const -> size_t;
    auto AddReadBytes(size_t nCount = 1) -> void;
    auto TotalBytes() const -> size_t;

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
