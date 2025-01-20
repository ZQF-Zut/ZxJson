#pragma once
#include <set>
#include <map>
#include <span>
#include <list>
#include <deque>
#include <array>
#include <cstdint>
#include <forward_list>
#include <unordered_set>
#include <unordered_map>


namespace ZQF::Zut::ZxJson::Traits
{
    template <typename T>
    concept is_iter_able = requires(T & t)
    {
        *std::begin(t);
        std::begin(t) != std::end(t);
        ++std::declval<decltype(std::begin(t))&>();
    };

    template <typename T>
    concept is_span_able = requires(T & t)
    {
        std::span{ t };
    };

    template <typename T>
    concept is_sequence_container = requires(T & t)
    {
        t.data();
        t.size();
    };


    template<typename T>
    constexpr bool is_arithmetic_v = std::is_arithmetic_v<T>;


    // std::basic_string
    template <typename T>
    struct is_std_basic_string : std::false_type {};

    template <typename... ArgsT>
    struct is_std_basic_string <std::basic_string<ArgsT...>> : std::true_type {};

    template <typename T>
    constexpr bool is_std_basic_string_v = is_std_basic_string<T>::value;

    // std::array
    template <typename T>
    struct is_std_array : std::false_type {};

    template <typename T, size_t N>
    struct is_std_array <std::array<T, N>> : std::true_type {};

    template <typename T>
    constexpr bool is_std_array_v = is_std_array<T>::value;

    // std::vector
    template <typename T>
    struct is_std_vector : std::false_type {};

    template <typename... ArgsT>
    struct is_std_vector <std::vector<ArgsT...>> : std::true_type {};

    template <typename T>
    constexpr bool is_std_vector_v = is_std_vector<T>::value;

    // std::deque
    template <typename T>
    struct is_std_deque : std::false_type {};

    template <typename... ArgsT>
    struct is_std_deque <std::deque<ArgsT...>> : std::true_type {};

    template <typename T>
    constexpr bool is_std_deque_v = is_std_deque<T>::value;

    // std::list
    template <typename T>
    struct is_std_list : std::false_type {};

    template <typename... ArgsT>
    struct is_std_list <std::list<ArgsT...>> : std::true_type {};

    template <typename T>
    constexpr bool is_std_list_v = is_std_list<T>::value;

    // std::forward_list
    template <typename T>
    struct is_std_forward_list : std::false_type {};

    template <typename... ArgsT>
    struct is_std_forward_list <std::forward_list<ArgsT...>> : std::true_type {};

    template <typename T>
    constexpr bool is_std_forward_list_v = is_std_forward_list<T>::value;

    // std::map
    template <typename T>
    struct is_std_map : std::false_type {};

    template <typename... ArgsT>
    struct is_std_map <std::map<ArgsT...>> : std::true_type {};

    template <typename T>
    constexpr bool is_std_map_v = is_std_map<T>::value;

    // std::unordered_map
    template <typename T>
    struct is_std_unordered_map : std::false_type {};

    template <typename... ArgsT>
    struct is_std_unordered_map <std::unordered_map<ArgsT...>> : std::true_type {};

    template <typename T>
    constexpr bool is_std_unordered_map_v = is_std_unordered_map<T>::value;

    // std::set
    template <typename T>
    struct is_std_set : std::false_type {};

    template <typename... ArgsT>
    struct is_std_set <std::set<ArgsT...>> : std::true_type {};

    template <typename T>
    constexpr bool is_std_set_v = is_std_set<T>::value;

    // std::unordered_set
    template <typename T>
    struct is_std_unordered_set : std::false_type {};

    template <typename... ArgsT>
    struct is_std_unordered_set <std::unordered_set<ArgsT...>> : std::true_type {};

    template <typename T>
    constexpr bool is_std_unordered_set_v = is_std_unordered_set<T>::value;

    // std::span
    template <typename T>
    struct is_std_span : std::false_type {};

    template <typename T, std::size_t N>
    struct is_std_span <std::span<T, N>> : std::true_type {};

    template <typename T>
    constexpr bool is_std_span_v = is_std_span<T>::value;
}
