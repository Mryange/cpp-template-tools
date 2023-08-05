#pragma once
#include "Tuple.h"
#include "traits.h"
template <int _Nx>
struct _MyPh {};

constexpr _MyPh<0> _0;
constexpr _MyPh<1> _1;
constexpr _MyPh<2> _2;
constexpr _MyPh<3> _3;
constexpr _MyPh<4> _4;

template <typename T>
constexpr bool is_MyPh = false;

template <int N>
constexpr bool is_MyPh<_MyPh<N>> = true;

template <typename T>
constexpr int get_MyPh = -1;

template <int N>
constexpr int get_MyPh<_MyPh<N>> = N;

template <typename F, typename... Args>
struct bind_ {
    template <typename Func, typename... Ts>
    bind_(Func &&f, Ts &&...args) :
        _func{std::forward<Func>(f)}, _tuple{std::forward<Ts>(args)...} {};

    template <typename... callArgs>
    auto operator()(callArgs &&...args) {
        using List = Make_Int_List<0, sizeof...(Args) - 1>;
        auto call_t = Tuple<callArgs...>{std::forward<callArgs>(args)...};

        return call(List{}, call_t);
    }
    template <typename T, int... S>
    auto call(Int_List<S...>, T &call_t) {
        return _func(get<S>(call_t)...);
    }
    template <int N>
    auto get(auto &call_t) {
        auto t = Get<N>(_tuple);
        using type = decltype(t);
        if constexpr (is_MyPh<type>) {
            constexpr int idx = get_MyPh<type>;
            return Get<idx>(call_t);
        } else {
            return t;
        }
    }
    Tuple<Args...> _tuple;
    F _func;
};

template <typename F, typename... Args>
auto Bind(F &&f, Args &&...args) {
    return bind_<std::decay_t<F>, std::decay_t<Args>...>(std::forward<F>(f), std::forward<Args>(args)...);
}