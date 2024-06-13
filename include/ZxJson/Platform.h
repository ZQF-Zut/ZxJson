#pragma once
#include <span>
#include <string>
#include <memory>


namespace ZQF::ZxJson
{
	auto RreadAllBytes(const std::string_view msPath) -> std::pair<size_t, std::unique_ptr<char[]>>;
	auto WriteAllBytes(const std::string_view msPath, const std::span<char> spData, bool isForceCreate) -> void;
}