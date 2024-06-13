#include <ZxJson/JValue.h>


namespace ZQF::ZxJson
{
	auto JValue::Dump(std::string& wsText, bool isFormat, size_t nIndent) const ->void
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
			for (auto ch : *(std::get<std::unique_ptr<JString_t>>(m_Data)))
			{
				switch (ch)
				{
				case '\n': ch = 'n'; break;
				case '\r': ch = 'r'; break;
				case '\b': ch = 'b'; break;
				case '\t': ch = 't'; break;
				case '\f': ch = 'f'; break;
				case '\"': ch = '"'; break;
				case '\\': ch = '\\'; break;
				default: {
					wsText.append(1, ch);
					continue;
				}
				}

				wsText.append(1, '\\');
				wsText.append(1, ch);
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
}

