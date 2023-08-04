#pragma once
#include <utility>
template <typename Ret, typename... Args>
struct Function;

template <typename Ret, typename... Args>
struct Function<Ret(Args...)> {
    struct Base {
        virtual Base *clone() = 0;
        virtual Ret operator()(Args &&...args) = 0;
        virtual ~Base(){};
    };
    template <typename T>
    struct Data : Base {
        T func;
        Data(const T &t) :
            func(t) {
        }
        virtual Base *clone() {
            return new Data<T>(func);
        }
        virtual Ret operator()(Args &&...args) {
            return func(std::forward<Args>(args)...);
        }
        virtual ~Data() = default;
    };
    Base *ptr;
    Function() :
        ptr{nullptr} {
    }
    template <typename T>
    Function(T t) :
        ptr{new Data<T>(t)} {};
    Function(const Function &rhs) {
        ptr = rhs.ptr->clone();
    }
    Function &operator=(const Function &rhs) {
        if (ptr) delete ptr;
        ptr = rhs.ptr->clone();
        return *this;
    }
    Function(Function &&rhs) noexcept {
        ptr = rhs.ptr;
        rhs.ptr = nullptr;
    }
    Function &operator=(Function &&rhs) noexcept {
        if (ptr) delete ptr;
        ptr = rhs.ptr;
        rhs.ptr = nullptr;
        return *this;
    }
    Ret operator()(Args... args) {
        return (*ptr)(std::forward<Args>(args)...);
    }
    ~Function() {
        if (ptr) delete ptr;
    }
};