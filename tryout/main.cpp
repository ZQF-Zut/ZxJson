#include <print>
#include <iostream>
#include <cassert>
#include <chrono>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <Zut/ZxJson.h>

#define ZXTEST(MY_TEST_CONDITION)                                                                                                                                                                                                                              \
  if ((MY_TEST_CONDITION) == false)                                                                                                                                                                                                                            \
  {                                                                                                                                                                                                                                                            \
    std::println("assert failed -> line:{} -> file:{}", __LINE__, __FILE__);                                                                                                                                                                                   \
    std::abort();                                                                                                                                                                                                                                              \
  }                                                                                                                                                                                                                                                            \
  else                                                                                                                                                                                                                                                         \
  {                                                                                                                                                                                                                                                            \
    std::println("assert success -> line:{} -> file:{}", __LINE__, __FILE__);                                                                                                                                                                                  \
  }

#define ZXTEST_EXCEPTION(EXCEPTION_CODE)                                                                                                                                                                                                                       \
  try                                                                                                                                                                                                                                                          \
  {                                                                                                                                                                                                                                                            \
    EXCEPTION_CODE;                                                                                                                                                                                                                                            \
    std::println("exception failed -> line:{} -> file:{}", __LINE__, __FILE__);                                                                                                                                                                                \
    std::abort();                                                                                                                                                                                                                                              \
  }                                                                                                                                                                                                                                                            \
  catch (const std::exception&)                                                                                                                                                                                                                                \
  {                                                                                                                                                                                                                                                            \
    std::println("exception success -> line:{} -> file:{}", __LINE__, __FILE__);                                                                                                                                                                               \
  }

// # Utils #
namespace ZQF
{
  class ZxRecord
  {
  private:
    std::chrono::steady_clock::time_point m_tpBeg;
    std::chrono::steady_clock::time_point m_tpEnd;
    std::vector<std::chrono::duration<double, std::milli>> m_vcRecord;

  public:
    auto Beg() -> void;
    auto End() -> void;
    auto Log() -> void;
  };

  auto ZxRecord::Beg() -> void
  {
    m_tpBeg = std::chrono::steady_clock::now();
  }

  auto ZxRecord::End() -> void
  {
    m_tpEnd = std::chrono::steady_clock::now();
    m_vcRecord.emplace_back((m_tpEnd - m_tpBeg));
  }

  auto ZxRecord::Log() -> void
  {
    std::chrono::duration<double, std::milli> cout{};

    for (auto& dur : m_vcRecord)
    {
      cout += dur;
      std::println("{}", dur);
    }

    std::println("Avg:{}", cout / m_vcRecord.size());
  }
} // namespace ZQF

// # Utils #
[[maybe_unused]] static auto FetchFileData(const std::filesystem::path& phJson) -> std::string
{
  std::string data;
  std::ifstream ifs{ phJson };
  if (!ifs)
  {
    throw std::runtime_error(std::format("open file error: {}", phJson.string()));
  }
  ifs.seekg(0, std::ios::end);
  data.reserve(ifs.tellg());
  ifs.seekg(0, std::ios::beg);
  data.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
  return data;
}

// # Bench #
[[maybe_unused]] static auto BenchJsonParser() -> void
{
  ZQF::ZxRecord record;

  auto raw_data = ::FetchFileData(u8"test.json");
  for (size_t i = 0; i < 200; i++)
  {
    record.Beg();
    auto json_value = ZxJson::LoadViaMemory(raw_data);
    record.End();
  }

  record.Log();
}

[[maybe_unused]] static auto BenchJsonDumper() -> void
{
  ZQF::ZxRecord record;

  ZxJson::JDoc jdoc{ "test.json" };
  auto& json_value = jdoc.GetJValue();
  for (size_t i = 0; i < 200; i++)
  {
    record.Beg();
    auto dump_str = ZxJson::StoreViaMemory(json_value, true);
    dump_str[0] = {};
    record.End();
  }

  jdoc.StoreViaMemory(true);

  record.Log();
}

// # Test #
[[maybe_unused]] static auto TestJsonParser() -> void
{
  auto raw_data = ::FetchFileData(u8"1.json");
  auto json_value = ZxJson::LoadViaMemory(raw_data);
}

auto main() -> int
{
  try
  {
    // TestLoadStoreViaFile();
    // TestJsonParser();
    // TestJsonDumper();
    // TestJsonValue();
    // TestJsonParseRegularEscape();
    // TestJsonParseUnicodeEscape();
    // BenchJsonDumper();
    BenchJsonParser();
  }
  catch (const std::exception& err)
  {
    std::println(std::cerr, "std::exception: {}", err.what());
  }
}
