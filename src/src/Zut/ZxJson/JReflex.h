#pragma once
#include <Zut/ZxJson/JValue.h>
#include <Zut/ZxJson/Reflex/Reflex.h>
#include <Zut/ZxJson/Reflex/Field.h>

namespace ZQF::Zut::ZxJson
{
  // clang-format off
    template<typename Object_Type>
    static auto ReflexStore(Object_Type&& Object) -> ZxJson::JValue
    {
      ZxJson::JValue json_value;
      Reflex::ZxReflex::FotEach
      (
        std::forward<Object_Type>(Object),
        [&json_value](auto&& name, auto& value)
        {
          Reflex::Field::Write(std::forward<decltype(value)>(value), std::forward<decltype(name)>(name), json_value);
        }
      );

      return json_value;
    }

    template<typename Object_Type>
    static auto ReflexLoad(Object_Type& Object, ZxJson::JValue&& JsonValue) -> void // NOLINT
    {
      Reflex::ZxReflex::FotEach
      (
        Object,
        [&JsonValue](auto&& name, auto& value)
        {
          Reflex::Field::Read(std::forward<decltype(value)>(value), std::forward<decltype(name)>(name), JsonValue);
        }
      );
    }

  // clang-format on
} // namespace ZQF::Zut::ZxJson
