#pragma once
template <typename... Ty>
struct Tuple;
template <>
struct Tuple<> {
    template <class... Ty>
    bool equal(const Tuple<> &rhs) const {
        return true;
    }
};
template <typename Ty1, typename... Ty2>
struct Tuple<Ty1, Ty2...> : Tuple<Ty2...> {
    Ty1 val;
    using Base = Tuple<Ty2...>;
    Tuple() {
    }
    Tuple(Ty1 v, Ty2... args) :
        val(v), Base(args...) {
    }
    Base &getBase() {
        return *this;
    }
    const Base &getBase() const {
        return *this;
    }
    template <class... Ty>
    bool equal(const Tuple<Ty...> &rhs) const {
        return this->val == rhs.val && Base::equal(rhs.getBase());
    }
};

template <int idx, typename _Tuple>
struct Tuple_element {
    using Type = typename Tuple_element<idx - 1, typename _Tuple::Base>::Type;
};
template <typename _Tuple>
struct Tuple_element<0, _Tuple> {
    using Type = _Tuple;
};

template <int idx, typename _Tuple>
constexpr auto &Get(_Tuple &t) {
    using Type = typename Tuple_element<idx, _Tuple>::Type;
    return static_cast<Type &>(t).val;
}

template <typename... Ty1, typename... Ty2>
bool operator==(const Tuple<Ty1...> &L, const Tuple<Ty2...> &R) {
    return L.equal(R);
}
