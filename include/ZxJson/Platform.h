#pragma once
#include <span>
#include <memory>
#include <string_view>


namespace ZQF::ZxJson::Private
{
    auto ReadAllBytes(const std::string_view msPath) -> std::pair<size_t, std::unique_ptr<char[]>>;
    auto WriteAllBytes(const std::string_view msPath, const std::span<char> spData, bool isForceCreate) -> void;
} // namespace ZQF::ZxJsonPrivate
