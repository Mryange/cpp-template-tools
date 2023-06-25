#include "traits.h"
/// TODO: 支持数组类型
// 创建析构函数
template <class T>
void destroy(char *data) {
    reinterpret_cast<T *>(data)->~T();
}
template <class... Ts>
struct Variant {
    int type = -1;
    char data[MaxSize<Ts...>::size] = {0};
    using destroy_func_t = void (*)(char *); // 析构函数类型
    // 析构函数 的数组
    constexpr static destroy_func_t destroy_func[] = {destroy<Ts>...};
    Variant(){};
    // 同类型的拷贝构造
    Variant(const Variant<Ts...> &rhs) {
        type = rhs.type;
        memcpy(data, rhs.data, MaxSize<Ts...>::size);
    }
    // 因为是构造函数，就不需要调用析构函数了
    template <typename T>
    Variant(T &&rhs) {
        type = Position<0, T, Ts...>::pos;
        *reinterpret_cast<T *>(data) = std::forward<T>(rhs);
    }
    // 同类型的拷贝
    void operator=(const Variant<Ts...> &rhs) {
        type = rhs.type;
        memcpy(data, rhs.data, MaxSize<Ts...>::size);
    }
    // 完美转发
    template <typename T>
    void operator=(T &&rhs) {
        if (type != -1) {
            destroy_func[type](data);
        }
        type = Position<0, T, Ts...>::pos;
        // 去掉带引用的类型
        using rm_ref = typename std::remove_reference<T>::type;
        memset(data, 0, sizeof(rm_ref));
        *reinterpret_cast<rm_ref *>(data) = std::forward<T>(rhs);
    }
    // 根据类型返回
    template <typename T>
    T &get() {
        return *reinterpret_cast<T *>(data);
    }
    // 根据下标返回
    template <int id>
    auto &get() {
        using T = typename Type_element<id, Ts...>::Type;
        return *reinterpret_cast<T *>(data);
    }
    // 析构
    ~Variant() {
        if (type != -1) {
            destroy_func[type](data);
        }
    }

    template <typename Func, typename Var, typename realtype>
    static auto _Visit_helper(Func &&func, Var &var) {
        return func(std::forward<Var>(var).template get<realtype>());
    }
    template <typename Func>
    auto Visit(Func &&func) {
        using arg0 = typename Type_element<0, Ts...>::Type;
        using Ret = typename get_ret_type<Func(arg0)>::type;
        using Vartype = Variant<Ts...>;
        using fn_type = Ret (*)(Func &&, Vartype &);
        constexpr static fn_type table[] = {_Visit_helper<Func, Vartype, Ts>...};
        return table[type](std::forward<Func>(func), *this);
    }
};

template <typename Func, typename Vs>
auto Visit(Func &&func, Vs &&var) {
    return std::forward<Vs>(var).Visit(std::forward<Func>(func));
}