#pragma once
#include <span>
#include <memory>
#include <cstddef>
#include <string_view>

namespace ZQF::Zut::ZxJson::Plat
{
  auto ReadAllBytes(const std::string_view msPath) -> std::pair<std::size_t, std::unique_ptr<char[]>>; // NOLINT
  auto WriteAllBytes(const std::string_view msPath, const std::span<const char> spData, const bool isForceCreate) -> void;
} // namespace ZQF::Zut::ZxJson::Plat
