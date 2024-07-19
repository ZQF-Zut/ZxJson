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
        auto operator=(const JDoc&)->JDoc & = delete;
        auto operator=(JDoc&&) noexcept -> JDoc & = delete;

        auto GetJValue() -> JValue&;
        auto GetJArray() -> JArray_t&;
        auto GetJObject() -> JObject_t&;
        auto LoadViaMemory(std::span<char> spData) -> bool;
        auto LoadViaFile(const std::string_view msPath) -> bool;
        auto StoreViaMemory(bool isFormat) const->std::string;
        auto StoreViaFile(const std::string_view msPath, const bool isFormat, const bool isForceSave) const -> void;
    };
} // namespace ZQF::ZxJson
