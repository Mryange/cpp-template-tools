
#pragma once
#include <type_traits>

/// TODO: 不要使用std的内容，全部手写
// Position<Ts...> 从 Ts... 找到 T类型的下标
template <int id, typename U, typename T, typename... Ts>
struct Position {
    constexpr static int pos = std::is_same_v<U, T> ? id : Position<id + 1, U, Ts...>::pos;
};
template <int id, typename U, typename T>
struct Position<id, U, T> {
    constexpr static int pos = id;
};

// MaxSize<Ts...>  Ts... 最大的一个
template <typename T, typename... Ts>
struct MaxSize {
    constexpr static int size = sizeof(T) > MaxSize<Ts...>::size ? sizeof(T) : MaxSize<Ts...>::size;
};
template <typename T>
struct MaxSize<T> {
    constexpr static int size = sizeof(T);
};

// Type_element<idx,Ts...> 返回下标为 idx 的类型
template <int idx, typename T, typename... Ts>
struct Type_element {
    using Type = typename Type_element<idx - 1, Ts...>::Type;
};
template <typename T, typename... Ts>
struct Type_element<0, T, Ts...> {
    using Type = T;
};

template <typename Fn, typename... Args>
struct get_ret_type;
template <typename Fn, typename... Args>
struct get_ret_type<Fn(Args...)> {
    using type = decltype(std::declval<Fn>()(std::declval<Args>()...));
};
template <typename T, typename... Ts>
struct is_all_trivial {
    static constexpr bool value = std::is_trivial_v<T> && is_all_trivial<Ts...>::value;
};
template <typename T>
struct is_all_trivial<T> {
    static constexpr bool value = std::is_trivial_v<T>;
};

template <typename... Ts>
constexpr bool is_all_trivial_v = is_all_trivial<Ts...>::value;

template <int... data>
struct Int_List {
    template <int N>
    using push_back = Int_List<data..., N>;
};

template <int L, int R, typename = void>
struct _Make_Int_List {
    using L_list = typename _Make_Int_List<L, R - 1>::ret;
    using ret = typename L_list::template push_back<R>;
};

template <int L, int R>
struct _Make_Int_List<L, R, typename std::enable_if_t<L == R, void>> {
    using ret = Int_List<L>;
};

template <int L, int R>
using Make_Int_List = typename _Make_Int_List<L, R>::ret;
