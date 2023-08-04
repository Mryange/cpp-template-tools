#pragma once
#include <utility>
template<typename T>
struct Function_ref;

template<typename Ret,typename ...Args>
struct Function_ref<Ret(Args...)>{
    using Type = Ret(*)(void * fnptr,Args...args);
    void * fnptr{nullptr};
    Type callfn_withtype;
    template<typename Fn>
    static Ret call_fn(void *fnptr,Args ... args){
        auto real_fn = *reinterpret_cast<Fn*>(fnptr);
        return real_fn(std::forward<Args>(args)...);
    }
    Function_ref() = default;
    Function_ref(std::nullptr_t){};
    template <typename Func>
    Function_ref(Func &&func) {
        using type = std::remove_reference_t<Func>;
        if constexpr (std::is_same_v<type, Function_ref<Ret(Args...)>>) {
            this->fnptr = func.fnptr;
            this->callfn_withtype = func.callfn_withtype;
        } else {
            callfn_withtype = call_fn<type>;
            fnptr = reinterpret_cast<void *>(&func);
        }
    }
    Ret operator()(Args ... args) const {
        return callfn_withtype(fnptr,std::forward<Args>(args)...);
    };
    operator bool () const {return fnptr;}
};
