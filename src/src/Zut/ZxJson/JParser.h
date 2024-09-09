#pragma once
#include <span>
#include <Zut/ZxJson/JValue.h>


namespace ZQF::Zut::ZxJson
{
    class JParser
    {
    private:
        std::size_t m_nReadBytes{};
        std::span<const char> m_spJson;

    public:
        JParser(const std::span<const char> spJson);
        ~JParser();
        JParser(const JParser&) = delete;
        JParser(JParser&&) noexcept = delete;

    public:
        auto operator=(const JParser&)->JParser & = delete;
        auto operator=(JParser&&) noexcept -> JParser & = delete;

    public:
        auto Parse(JValue& rfJValue) -> bool;

    private:
        auto CurPtr() -> const char*;
        auto GetReadBytes() const -> std::size_t;
        auto AddReadBytes(const std::size_t nCount = 1) -> void;
        auto TotalBytes() const -> std::size_t;

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
} // namespace ZQF::Zut::ZxJson
