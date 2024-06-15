#include <ZxJson/JValue.h>

namespace ZQF::ZxJson
{
auto JValue::Dump(std::string& wsText, bool isFormat, size_t nIndent) const -> void // NOLINT
{
    if (std::holds_alternative<JNull_t>(m_Data))
    {
        wsText.append("null");
    }
    else if (std::holds_alternative<JBool_t>(m_Data))
    {
        wsText.append((std::get<JBool_t>(m_Data)) ? "true" : "false");
    }
    else if (std::holds_alternative<JInt_t>(m_Data))
    {
        wsText.append(std::to_string(std::get<JInt_t>(m_Data)));
    }
    else if (std::holds_alternative<JDouble_t>(m_Data))
    {
        wsText.append(std::to_string(std::get<JDouble_t>(m_Data)));
    }
    else if (std::holds_alternative<std::unique_ptr<JString_t>>(m_Data))
    {
        wsText.append(1, '\"');
        for (auto char_v : *(std::get<std::unique_ptr<JString_t>>(m_Data)))
        {
            switch (char_v)
            {
            case '\n': char_v = 'n'; break;
            case '\r': char_v = 'r'; break;
            case '\b': char_v = 'b'; break;
            case '\t': char_v = 't'; break;
            case '\f': char_v = 'f'; break;
            case '\"': char_v = '"'; break;
            case '\\': char_v = '\\'; break;
            default: wsText.append(1, char_v); continue;
            }

            wsText.append(1, '\\');
            wsText.append(1, char_v);
        }
        wsText.append(1, '\"');
    }
    else if (std::holds_alternative<std::unique_ptr<JArray_t>>(m_Data))
    {
        wsText.append(1, '[');

        nIndent++;
        for (const auto& value : *(std::get<std::unique_ptr<JArray_t>>(m_Data)))
        {
            if (isFormat)
            {
                wsText.append(1, '\n');
                wsText.append(nIndent, '\t');
            }

            value.Dump(wsText, isFormat, nIndent);
            wsText.append(1, ',');
        }
        nIndent--;

        // if not null array
        if (wsText.back() == ',')
        {
            wsText.pop_back();
            if (isFormat)
            {
                wsText.append(1, '\n');
                wsText.append(nIndent, '\t');
            }
        }

        wsText.append(1, ']');
    }
    else if (std::holds_alternative<std::unique_ptr<JObject_t>>(m_Data))
    {
        wsText.append(1, '{');

        nIndent++;
        {
            for (const auto& value : *(std::get<std::unique_ptr<JObject_t>>(m_Data)))
            {
                if (isFormat)
                {
                    wsText.append(1, '\n');
                    wsText.append(nIndent, '\t');
                }

                wsText.append(1, '\"');
                wsText.append(value.first);
                wsText.append(1, '\"');
                wsText.append(1, ':');

                if (isFormat)
                {
                    wsText.append(1, ' ');
                }

                value.second.Dump(wsText, isFormat, nIndent);
                wsText.append(1, ',');
            }
        }
        nIndent--;

        // if not null object
        if (wsText.back() == ',')
        {
            wsText.pop_back();
            if (isFormat)
            {
                wsText.append(1, '\n');
                wsText.append(nIndent, '\t');
            }
        }

        wsText.append(1, '}');
    }
}
} // namespace ZQF::ZxJson
