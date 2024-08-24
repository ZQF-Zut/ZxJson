#pragma once
#include <span>
#include <memory>
#include <cstdint>
#include <string_view>


namespace ZQF::ZxJson::Private
{
    auto ReadAllBytes(const std::string_view msPath) -> std::pair<std::size_t, std::unique_ptr<char[]>>;
    auto WriteAllBytes(const std::string_view msPath, const std::span<const char> spData, const bool isForceCreate) -> void;
} // namespace ZQF::ZxJsonPrivate
