#pragma once
#include <ranges>
#include <Zut/ZxJson/Reflex/Traits.h>


namespace ZQF::Zut::ZxJson::Reflex
{
    template<typename Field_Type, typename IO_Type>
    static auto WriteBase(Field_Type&& Field, IO_Type&& Writer) -> void
    {
        using io_type_t = std::decay_t<IO_Type>;
        using field_type_t = std::decay_t<Field_Type>;

        if constexpr (std::is_same_v<ZxJson::JValue, io_type_t>)
        {
            Writer.Assign(std::forward<Field_Type>(Field));
        }
        else if constexpr (std::is_same_v<ZxJson::JArray_t, io_type_t>)
        {
            Writer.reserve(Field.size());
            for (auto&& v : Field)
            {
                Writer.emplace_back(v);
            }
        }
        else if constexpr (std::is_same_v < ZxJson::JObject_t, io_type_t>)
        {
            for (auto&& v : Field)
            {
                Writer.try_emplace(v.first, v.second);
            }
        }
        else
        {
            static_assert(false, "ZQF::Zut::ZxJson::Reflect::_FieldWrite(): error type!");
        }
    }

    template<typename Field_Type>
    static auto Write(Field_Type&& Field, auto&& Name, auto&& Writer) -> void
    {
        using field_type_t = std::decay_t<Field_Type>;

        auto& jv = Writer[std::string{ Name }];

        if constexpr (Traits::is_arithmetic_v<field_type_t> || Traits::is_std_basic_string_v<field_type_t>)
        {
            WriteBase(std::forward<Field_Type>(Field), jv);
        }
        else if constexpr (Traits::is_iter_able<field_type_t>)
        {
            if constexpr (Traits::is_std_map_v<field_type_t> || Traits::is_std_unordered_map_v<field_type_t>)
            {
                WriteBase(std::forward<Field_Type>(Field), jv.ToObject());
            }
            else
            {
                WriteBase(std::forward<Field_Type>(Field), jv.ToArray());
            }
        }
        else
        {
            static_assert(false, "ZQF::Zut::ZxJson::Reflect::_FieldWrite(): error type!");
        }
    }

    template<typename Field_Type, typename IO_Type>
    static auto ReadBase(Field_Type&& Field, IO_Type&& Reader) -> void
    {
        using io_type_t = std::decay_t<IO_Type>;
        using field_type_t = std::decay_t<Field_Type>;

        if constexpr (std::is_same_v<ZxJson::JValue, io_type_t>)
        {
            if constexpr (std::is_integral_v<field_type_t>)
            {
                Field = Reader.GetNum<field_type_t>();
            }
            else if constexpr (std::is_floating_point_v<field_type_t>)
            {
                Field = Reader.GetFloat<field_type_t>();
            }
            else if constexpr (std::is_same_v<bool, field_type_t>)
            {
                Field = Reader.GetBool();
            }
            else if constexpr (Traits::is_std_basic_string_v<field_type_t>)
            {
                Field.assign(Reader.GetStrView());
            }
            else
            {
                static_assert(false, "ZQF::Zut::ZxJson::Reflect::_FieldWrite(): error type!");
            }
        }
        else if constexpr (std::is_same_v<ZxJson::JArray_t, io_type_t>)
        {
            using ele_type = field_type_t::value_type;

            if constexpr (Traits::is_std_array_v<field_type_t>)
            {
                for (auto&& [idx, jv] : std::views::enumerate(Reader))
                {
                    ele_type tmp;
                    ReadBase(tmp, jv);
                    Field[idx] = std::move(tmp);
                }
            }
            else
            {
                if constexpr (Traits::is_std_vector_v<field_type_t>)
                {
                    Field.reserve(Reader.size());
                }

                for (const auto& jv : Reader)
                {
                    ele_type tmp;
                    ReadBase(tmp, jv);
                    Field.emplace_back(std::move(tmp));
                }
            }
        }
        else if constexpr (std::is_same_v <ZxJson::JObject_t, io_type_t>)
        {
            using val_type = field_type_t::mapped_type;

            for (auto&& v : Reader)
            {
                val_type tmp;
                ReadBase(tmp, v.second);
                Field.try_emplace(v.first, std::move(tmp));
            }
        }
        else
        {
            static_assert(false, "ZQF::Zut::ZxJson::Reflect::_FieldWrite(): error type!");
        }
    }

    template<typename Field_Type>
    static auto Read(Field_Type& Field, std::string_view&& Name, const auto& Writer) -> void
    {
        using field_type_t = std::decay_t<Field_Type>;

        const auto& jv = Writer.At(Name);

        if constexpr (Traits::is_arithmetic_v<field_type_t> || Traits::is_std_basic_string_v<field_type_t>)
        {
            ReadBase(Field, jv);
        }
        else if constexpr (Traits::is_iter_able<field_type_t>)
        {
            if constexpr (Traits::is_std_map_v<field_type_t> || Traits::is_std_unordered_map_v<field_type_t>)
            {
                ReadBase(Field, jv.GetObject());
            }
            else
            {
                ReadBase(Field, jv.GetArray());
            }
        }
        else
        {
            static_assert(false, "ZQF::Zut::ZxJson::Reflect::_FieldWrite(): error type!");
        }
    }
}
