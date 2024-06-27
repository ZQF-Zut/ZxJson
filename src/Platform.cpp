#include <ZxJson/Platform.h>
#include <span>
#include <format>

#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <cstdio>
#endif


namespace ZQF::ZxJson
{
#ifdef _WIN32
    static auto PathUtf8ToWide(const std::string_view msPath, wchar_t* wpBuffer) -> void
    {
        const auto char_count = static_cast<size_t>(::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, msPath.data(), static_cast<int>(msPath.size()), nullptr, 0));
        if (char_count >= MAX_PATH)
        {
            throw std::runtime_error(std::format("ZxJson::PathUtf8ToWide(): file path too loog. msPath: {}", msPath));
        }
        const auto char_count_real = static_cast<size_t>(::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, msPath.data(), static_cast<int>(msPath.size()), wpBuffer, static_cast<int>(char_count)));
        wpBuffer[char_count_real] = {};
    }

    auto ReadAllBytes(const std::string_view msPath) -> std::pair<size_t, std::unique_ptr<char[]>>
    {
        wchar_t path_buffer[MAX_PATH];
        PathUtf8ToWide(msPath, path_buffer); // NOLINT

        const HANDLE hfile = ::CreateFileW(path_buffer, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr); // NOLINT
        if (hfile == INVALID_HANDLE_VALUE)
        {
            throw std::runtime_error(std::format("ZxJson::RreadAllBytes(): open file error!. msPath: {}", msPath));
        }

        auto file_size = static_cast<size_t>(::GetFileSize(hfile, nullptr));

        auto file_buffer = std::make_unique_for_overwrite<char[]>(file_size);

        DWORD read{};
        (void)::ReadFile(hfile, file_buffer.get(), static_cast<DWORD>(file_size), &read, nullptr);

        ::CloseHandle(hfile);

        return { file_size, std::move(file_buffer) };
    }

    auto WriteAllBytes(const std::string_view msPath, const std::span<char> spData, bool isForceCreate) -> void
    {
        wchar_t path_buffer[MAX_PATH];
        PathUtf8ToWide(msPath, path_buffer); // NOLINT

        const HANDLE hfile = ::CreateFileW(path_buffer, GENERIC_WRITE, FILE_SHARE_READ, nullptr, isForceCreate ? CREATE_ALWAYS : CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr); // NOLINT
        if (hfile == INVALID_HANDLE_VALUE)
        {
            throw std::runtime_error(std::format("ZxJson::WriteAllBytes(): create file error!. msPath: {}", msPath));
        }

        DWORD write{};
        (void)::WriteFile(hfile, spData.data(), static_cast<DWORD>(spData.size_bytes()), &write, nullptr);

        ::CloseHandle(hfile);
    }
#else
    auto ReadAllBytes(const std::string_view msPath) -> std::pair<size_t, std::unique_ptr<char[]>>
    {
        FILE* data_fp = fopen(msPath.data(), "rb");
        if (data_fp == nullptr)
        {
            throw std::runtime_error(std::format("ZxJson::RreadAllBytes(): open file error!. msPath: {}", msPath));
        }

        size_t file_size{};
        ::fseek(data_fp, 0, SEEK_END);
        file_size = ::ftell(data_fp);
        ::fseek(data_fp, 0, SEEK_SET);

        auto buffer = std::make_unique_for_overwrite<char[]>(file_size);

        ::fread(buffer.get(), 1, file_size, data_fp);
        ::fclose(data_fp);

        return { file_size, std::move(buffer) };
    }

    auto WriteAllBytes(const std::string_view msPath, const std::span<char> spData, bool isForceCreate) -> void
    {
        if (!isForceCreate)
        {
            // to do
            throw std::runtime_error(std::format("ZxJson::WriteAllBytes(): file exists!. msPath: {}", msPath));
        }

        FILE* data_fp = fopen(msPath.data(), "wb");
        if (data_fp == nullptr)
        {
            throw std::runtime_error(std::format("ZxJson::WriteAllBytes(): create file error!. msPath: {}", msPath));
        }
        ::fwrite(spData.data(), 1, spData.size_bytes(), data_fp);
        ::fclose(data_fp);
    }
#endif
} // namespace ZQF::ZxJson
