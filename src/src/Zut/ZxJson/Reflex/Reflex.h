#pragma once
#include <tuple>
#include <string_view>
#include <source_location>

namespace ZQF::Zut::ZxJson::Reflex
{
  class ZxReflex
  {
  private:
    struct AnyType
    {
      template <typename T>
      operator T(){};
    };

    template <typename Object_Type>
    struct StaticWarp
    {
      inline static std::decay_t<Object_Type> value; // NOLINT
    };

    template <typename T>
    static consteval auto CountMember(auto&&... Args) -> std::size_t
    {
      if constexpr (!requires { T{ Args... }; })
      {
        return sizeof...(Args) - 1;
      }
      else
      {
        return CountMember<T>(Args..., AnyType{});
      }
    }

    template <auto... Vs>
    static consteval auto GetFieldSigNameImp() -> std::string_view
    {
#if __cpp_lib_source_location >= 201907L
      return std::source_location::current().function_name();
#elif defined(_MSC_VER)
      return __FUNCSIG__;
#else
      return __PRETTY_FUNCTION__;
#endif
    }

    template <class... Vs>
    static consteval auto GetFieldSigNameImp() -> std::string_view
    {
#if __cpp_lib_source_location >= 201907L
      return std::source_location::current().function_name();
#elif defined(_MSC_VER)
      return __FUNCSIG__;
#else
      return __PRETTY_FUNCTION__;
#endif
    }

    template <typename Object_Type, std::size_t IDX>
    static consteval auto GetFieldSigName() -> std::string_view
    {
      return ZxReflex::GetFieldSigNameImp<std::get<IDX>(ZxReflex::GetTuple(StaticWarp<Object_Type>::value))>();
    }

    template <typename Object_Type>
    static constexpr auto GetTuple(Object_Type&& Object) -> auto // NOLINT
    {
      constexpr auto field_num{ ZxReflex::CountMember<std::decay_t<Object_Type>>() };

      if constexpr (field_num == 0)
      {
        static_assert(false, "ZxReflex::GetTuple(): zero field.");
      }
      else if constexpr (field_num == 1)
      {
        auto&& [_1] = Object;
        return std::tuple{ &_1 };
      }
      else if constexpr (field_num == 2)
      {
        auto&& [_1, _2] = Object;
        return std::tuple{ &_1, &_2 };
      }
      else if constexpr (field_num == 3)
      {
        auto&& [_1, _2, _3] = Object;
        return std::tuple{ &_1, &_2, &_3 };
      }
      else if constexpr (field_num == 4)
      {
        auto&& [_1, _2, _3, _4] = Object;
        return std::tuple{ &_1, &_2, &_3, &_4 };
      }
      else if constexpr (field_num == 5)
      {
        auto&& [_1, _2, _3, _4, _5] = Object;
        return std::tuple{ &_1, &_2, &_3, &_4, &_5 };
      }
      else if constexpr (field_num == 6)
      {
        auto&& [_1, _2, _3, _4, _5, _6] = Object;
        return std::tuple{ &_1, &_2, &_3, &_4, &_5, &_6 };
      }
      else if constexpr (field_num == 7)
      {
        auto&& [_1, _2, _3, _4, _5, _6, _7] = Object;
        return std::tuple{ &_1, &_2, &_3, &_4, &_5, &_6, &_7 };
      }
      else if constexpr (field_num == 8)
      {
        auto&& [_1, _2, _3, _4, _5, _6, _7, _8] = Object;
        return std::tuple{ &_1, &_2, &_3, &_4, &_5, &_6, &_7, &_8 };
      }
      else if constexpr (field_num == 9)
      {
        auto&& [_1, _2, _3, _4, _5, _6, _7, _8, _9] = Object;
        return std::tuple{ &_1, &_2, &_3, &_4, &_5, &_6, &_7, &_8, &_9 };
      }
      else if constexpr (field_num == 10)
      {
        auto&& [_1, _2, _3, _4, _5, _6, _7, _8, _9, _10] = Object;
        return std::tuple{ &_1, &_2, &_3, &_4, &_5, &_6, &_7, &_8, &_9, &_10 };
      }
      else if constexpr (field_num == 11)
      {
        auto&& [_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11] = Object;
        return std::tuple{ &_1, &_2, &_3, &_4, &_5, &_6, &_7, &_8, &_9, &_10, &_11 };
      }
      else if constexpr (field_num == 12)
      {
        auto&& [_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12] = Object;
        return std::tuple{ &_1, &_2, &_3, &_4, &_5, &_6, &_7, &_8, &_9, &_10, &_11, &_12 };
      }
      else if constexpr (field_num == 13)
      {
        auto&& [_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13] = Object;
        return std::tuple{ &_1, &_2, &_3, &_4, &_5, &_6, &_7, &_8, &_9, &_10, &_11, &_12, &_13 };
      }
      else if constexpr (field_num == 14)
      {
        auto&& [_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14] = Object;
        return std::tuple{ &_1, &_2, &_3, &_4, &_5, &_6, &_7, &_8, &_9, &_10, &_11, &_12, &_13, &_14 };
      }
      else if constexpr (field_num == 15)
      {
        auto&& [_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15] = Object;
        return std::tuple{ &_1, &_2, &_3, &_4, &_5, &_6, &_7, &_8, &_9, &_10, &_11, &_12, &_13, &_14, &_15 };
      }
      else if constexpr (field_num == 16)
      {
        auto&& [_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16] = Object;
        return std::tuple{ &_1, &_2, &_3, &_4, &_5, &_6, &_7, &_8, &_9, &_10, &_11, &_12, &_13, &_14, &_15, &_16 };
      }
      else
      {
        static_assert(false, "ZxReflex::GetTuple(): max field count.");
      }
    }

  public:
    template <typename Object_Type>
    static consteval auto GetFieldCount() -> std::size_t
    {
      return std::tuple_size_v<decltype(ZxReflex::GetTuple(StaticWarp<Object_Type>::value))>;
    }

    template <typename Object_Type, std::size_t IDX>
    static consteval auto GetFieldName() -> std::string_view
    {
      constexpr auto symbol_name = ZxReflex::GetFieldSigName<Object_Type, IDX>();
#if __clang__
      constexpr auto field_name_beg{ symbol_name.find("[Vs = <&value.") + 14 };
      constexpr auto field_name_end{ symbol_name.find(">]", field_name_beg) };
      constexpr auto filed_name_size{ field_name_end - field_name_beg };
      return symbol_name.substr(field_name_beg, filed_name_size);
#elif __GNUC__
      constexpr auto field_name_beg{ symbol_name.find("::", symbol_name.find("&>::value.") + 11) + 2 };
      constexpr auto field_name_end{ symbol_name.find(")]") };
      constexpr auto filed_name_size{ field_name_end - field_name_beg };
      return symbol_name.substr(field_name_beg, filed_name_size);
#elif _MSC_VER
      constexpr auto field_name_beg{ symbol_name.find("&value->") + 8 };
      constexpr auto field_name_end{ symbol_name.find('>', field_name_beg) };
      constexpr auto filed_name_size{ field_name_end - field_name_beg };
      return symbol_name.substr(field_name_beg, filed_name_size);
#else
      static_assert(false, "ZxReflect::GetFieldName(): unknown compiler.");
#endif
    }

    template <typename Object_Type>
    static constexpr auto FotEach(Object_Type&& Object, auto&& Functor) -> void // NOLINT
    {
      auto tuple = ZxReflex::GetTuple(Object);
      [&Functor, &tuple]<auto... Ns>(std::index_sequence<Ns...>) { (Functor(ZxReflex::GetFieldName<Object_Type, Ns>(), *std::get<Ns>(tuple)), ...); }(std::make_index_sequence<ZxReflex::GetFieldCount<Object_Type>()>{});
    }

    template <typename Object_Type, std::size_t IDX>
    using GetFieldType = std::remove_pointer_t<std::decay_t<decltype(std::get<IDX>(ZxReflex::GetTuple(StaticWarp<Object_Type>::value)))>>;
  };
} // namespace ZQF::Zut::ZxJson::Reflex
