#include <ZxJson/JDoc.h>


namespace ZQF::ZxJson
{
	JDoc::JDoc()
	{

	};

	JDoc::JDoc(std::span<char> spData)
	{
		this->Load(spData);
	};

	JDoc::~JDoc()
	{

	};

	auto JDoc::GetJValue() -> JValue&
	{
		return m_JValue;
	}

	auto JDoc::GetJArray() -> JArray_t&
	{
		return m_JValue.Sure<JArray_t&>();
	}

	auto JDoc::GetJObject() -> JObject_t&
	{
		return m_JValue.Sure<JObject_t&>();
	}

	auto JDoc::Load(std::span<char> spData) -> bool
	{
		return JParser{ spData }.Parse(m_JValue);
	}

	auto JDoc::Dump(bool isFormat) const -> std::string
	{
		std::string json;
		m_JValue.Dump(json, isFormat, 0);
		return json;
	}
}