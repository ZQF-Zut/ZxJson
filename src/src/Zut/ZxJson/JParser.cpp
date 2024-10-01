#include "JParser.h"
#include <cmath>
#include <cstring>


namespace ZQF::Zut::ZxJson
{
    JParser::JParser(const std::span<const char> spJson) : m_spJson{ spJson }
    {
        // skip utf-8 bom
        if (spJson[0] == '\xEF' && spJson[1] == '\xBB' && spJson[2] == '\xBF') { this->AddReadBytes(3); }
    }

    JParser::~JParser()
    {

    }

    auto JParser::Parse(JValue& rfJValue) -> bool
    {
        if (m_spJson.empty())
        {
            rfJValue.Assign(JNull_t{});
            return true;
        }

        this->SkipWhite();
        this->ParseValue(rfJValue);
        return this->GetReadBytes() == this->TotalBytes();
    }

    auto JParser::CurPtr() -> const char*
    {
        return m_spJson.data() + m_nReadBytes;
    }

    auto JParser::TotalBytes() const -> std::size_t
    {
        return m_spJson.size();
    }

    auto JParser::GetReadBytes() const -> std::size_t
    {
        return m_nReadBytes;
    }

    auto JParser::AddReadBytes(const std::size_t nCount) -> void
    {
        this->m_nReadBytes += nCount;
    }

    auto JParser::SkipWhite() -> char
    {
        while (true)
        {
            const auto tmp_char = this->CurToken();
            switch (tmp_char)
            {
            case '\t': case '\n': case '\r': case ' ':
                this->AddReadBytes();
                continue;
            }
            return tmp_char;
        }
    }

    auto JParser::NextToken() -> char
    {
        const auto tmp_char = this->SkipWhite();

        switch (tmp_char)
        {
        case '{': case '}': case '[': case ']':
        case '"': case ':': case 'n': case 't':
        case 'f': case '0': case '1': case '2':
        case '3': case '4': case '5': case '6':
        case '7': case '8': case '9': case '-':
            return tmp_char;
        case ',':
            this->AddReadBytes();
            return this->NextToken();
        default: throw std::runtime_error(std::string{ "ZxJson::JParser::NextToken(): not find token -> " }.append(1, tmp_char));
        }
    }

    auto JParser::CurToken() -> char
    {
        return m_spJson[m_nReadBytes];
    }

    auto JParser::ParseArray(JValue& rfJValue) -> void
    {
        assert(this->CurToken() == '[');

        auto& jarry = rfJValue.ToArray();
        this->AddReadBytes();

        while (this->NextToken() != ']')
        {
            JValue value;
            this->ParseValue(value);
            jarry.emplace_back(std::move(value));
        }

        this->AddReadBytes();
    }

    auto JParser::ParseObject(JValue& rfJValue) -> void
    {
        assert(this->CurToken() == '{');

        auto& obj = rfJValue.ToObject();
        this->AddReadBytes();

        std::string_view key;
        while (true)
        {
            switch (this->NextToken())
            {
            case '"': // parse key
            {
                this->AddReadBytes();

                const char* beg = this->CurPtr();
                const char* end = ::strchr(beg, '"');
                key = { beg, end };

                this->AddReadBytes((end - beg) + 1);
            }
            break;

            case ':': // check key : value
            {
                this->AddReadBytes();
                this->SkipWhite();

                JValue value;
                this->ParseValue(value);

                if (const auto [ite, is_insert_key] = obj.try_emplace({ key.data(), key.size() }); !is_insert_key)
                {
                    throw std::runtime_error(std::string{ "ZxJson::JParser::ParseObject(): same key! -> " }.append(key));
                }
                else
                {
                    ite->second = std::move(value);
                }
            }
            break;

            case '}': // end of object
            {
                this->AddReadBytes();
                return;
            }
            break;

            default:
                throw std::runtime_error("ZxJson::JParser::ParseObject(): error!");
            }
        }
    }

    auto JParser::ParseNumber(JValue& rfJValue) -> void
    {
        char* end = nullptr;
        const char* beg = this->CurPtr();
        const auto num_org = ::strtod(beg, &end);
        this->AddReadBytes(end - beg);

        const auto num_int = static_cast<uint64_t>(num_org);
        (num_org == static_cast<double>(num_int)) ? rfJValue.Assign(num_int) : rfJValue.Assign(num_org);
    }

    auto JParser::ParseString(JValue& rfJValue) -> void // NOLINT
    {
        auto fn_decode_unicode = [this](std::string& rfStr) {
            auto fn_read_code_point = [this]() -> uint16_t {
                auto fn_char2num = [](char cChar) -> uint8_t {
                    auto char_v = static_cast<uint8_t>(cChar);
                    if (char_v >= 0x61 && char_v <= 0x7a) { return char_v - 0x57; }      // a-z
                    else if (char_v >= 0x41 && char_v <= 0x5A) { return char_v - 0x37; } // A-Z
                    else if (char_v >= 0x30 && char_v <= 0x39) { return char_v - 0x30; } // 0-9
                    else { throw std::runtime_error("ZxJson::JParser::ParseString(): read code point error!"); }
                    };

                const char* code_point_str_ptr = this->CurPtr();
                uint8_t hex_0 = fn_char2num(code_point_str_ptr[0]);
                uint8_t hex_1 = fn_char2num(code_point_str_ptr[1]);
                uint8_t hex_2 = fn_char2num(code_point_str_ptr[2]);
                uint8_t hex_3 = fn_char2num(code_point_str_ptr[3]);
                uint16_t code_point = (hex_0 << 12) | (hex_1 << 8) | (hex_2 << 4) | (hex_3 << 0);
                this->AddReadBytes(4);
                return code_point;
                };

            assert(this->CurToken() == 'u');
            this->AddReadBytes();

            uint32_t code_point = fn_read_code_point();
            if (code_point >= 0xD800 && code_point <= 0xDBFF) // check surrogate pair leading range
            {
                // check '\u'
                if (this->CurPtr()[0] == '\\' && this->CurPtr()[1] == 'u')
                {
                    this->AddReadBytes(2);
                }
                else
                {
                    throw std::runtime_error("ZxJson::JParser::ParseString(): surrogate pair trailing error!");
                }

                // read trailing
                uint16_t code_point_trailing = fn_read_code_point();
                if (code_point_trailing >= 0xDC00 && code_point_trailing <= 0xDFFF) // trailing range
                {
                    code_point = (((code_point - 0xD800) << 10) | (code_point_trailing - 0xDC00)) + 0x10000;
                }
                else
                {
                    throw std::runtime_error(std::string{ "ZxJson::JParser::ParseString(): surrogate pair trailing range error! -> " }.append(std::to_string(code_point_trailing)));
                }
            }

            // encode to utf8
            if (code_point <= 0x7F)
            {
                rfStr.append(1, static_cast<char>(code_point & 0x7F));
            }
            else if (code_point <= 0x7FF)
            {
                rfStr.append(1, static_cast<char>(0xC0 | ((code_point >> 0x06) & 0xFF)));
                rfStr.append(1, static_cast<char>(0x80 | (code_point & 0x3F)));
            }
            else if (code_point <= 0xFFFF)
            {
                rfStr.append(1, static_cast<char>(0xE0 | ((code_point >> 0x0C) & 0xFF)));
                rfStr.append(1, static_cast<char>(0x80 | ((code_point >> 0x06) & 0x3F)));
                rfStr.append(1, static_cast<char>(0x80 | (code_point & 0x3F)));
            }
            else if (code_point <= 0x10FFFF)
            {
                rfStr.append(1, static_cast<char>(0xF0 | ((code_point >> 0x12) & 0xFF)));
                rfStr.append(1, static_cast<char>(0x80 | ((code_point >> 0x0C) & 0x3F)));
                rfStr.append(1, static_cast<char>(0x80 | ((code_point >> 0x06) & 0x3F)));
                rfStr.append(1, static_cast<char>(0x80 | (code_point & 0x3F)));
            }
            else
            {
                throw std::runtime_error(std::string{ "ZxJson::JParser::ParseString(): encode utf8 error! -> " }.append(std::to_string(code_point)));
            }
            };

        assert(this->CurToken() == '"');
        this->AddReadBytes();

        std::string str;
        while (true)
        {
            char token = this->CurToken();
            this->AddReadBytes();
            if (token == '\\')
            {
                switch (this->CurToken())
                {
                case '\\': token = '\\'; break;
                case '"': token = '\"'; break;
                case 'b': token = '\b'; break;
                case 'f': token = '\f'; break;
                case 'n': token = '\n'; break;
                case 'r': token = '\r'; break;
                case 't': token = '\t'; break;
                case 'u': token = 'u'; break;
                default: throw std::runtime_error(std::string{ "ZxJson::JParser::ParseString(): unknown escape character! -> " }.append(1, token));
                }

                if (token == 'u')
                {
                    fn_decode_unicode(str);
                }
                else
                {
                    str.append(1, token);
                    this->AddReadBytes();
                }
            }
            else if (token == '"') // end
            {
                break;
            }
            else
            {
                str.append(1, token);
            }
        }

        rfJValue.Assign(std::move(str));
    }

    auto JParser::ParseTrue(JValue& rfJValue) -> void
    {
        assert(this->CurToken() == 't');
        assert(::strncmp(this->CurPtr(), "true", 4) == 0);
        this->AddReadBytes(4);
        rfJValue.Assign(true);
    }

    auto JParser::ParseFalse(JValue& rfJValue) -> void
    {
        assert(this->CurToken() == 'f');
        assert(::strncmp(this->CurPtr(), "false", 5) == 0);
        this->AddReadBytes(5);
        rfJValue.Assign(false);
    }

    auto JParser::ParseNull(JValue& rfJValue) -> void
    {
        assert(this->CurToken() == 'n');
        assert(::strncmp(this->CurPtr(), "null", 4) == 0);
        this->AddReadBytes(4);
        rfJValue.Assign(JNull_t{});
    }

    auto JParser::ParseValue(JValue& rfJValue) -> void
    {
        const auto cur_token = this->CurToken();
        switch (cur_token)
        {
        case '{': ParseObject(rfJValue); break; // object
        case '[': ParseArray(rfJValue); break; // array
        case 't': ParseTrue(rfJValue); break; // true
        case 'f': ParseFalse(rfJValue); break; // false
        case 'n': ParseNull(rfJValue); break; // null
        case '"': ParseString(rfJValue); break; // string
        case '0':case '1':case '2':case '3':
        case '4':case '5':case '6':case '7':
        case '8':case '9':case '-': ParseNumber(rfJValue); break; // Number
        default: throw std::runtime_error(std::string{ "ZxJson::JParser::ParseValue(): token error! -> " }.append(1, cur_token));
        }
    }
} // namespace ZQF::Zut::ZxJson
