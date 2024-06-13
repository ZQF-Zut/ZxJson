#pragma once
#include <ZxJson/JParser.h>


namespace ZQF::ZxJson
{
	class JDoc
	{
	private:
		JValue m_JValue;

	public:
		JDoc();
		JDoc(std::span<char> spData);
		~JDoc();

	public:
		auto GetJValue() -> JValue&;
		auto GetJArray() -> JArray_t&;
		auto GetJObject() -> JObject_t&;
		auto Load(std::span<char> spData) -> bool;
		auto Dump(bool isFormat) const->std::string;
	};
}