
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