#include <iostream>
#include <vector>
struct Any {
    Any() :
        ptr{0} {
    }
    template <typename T>
    Any(const T &t) :
        ptr{new Data<T>(t)} {};
    Any(const Any &rhs) {
        ptr = rhs.ptr->clone();
    }
    Any &operator=(const Any &rhs) {
        if (ptr) delete ptr;
        ptr = rhs.ptr->clone();
        return *this;
    }
    Any(Any &&rhs) noexcept {
        ptr = rhs.ptr;
        rhs.ptr = 0;
    }
    Any &operator=(Any &&rhs) noexcept {
        if (ptr) delete ptr;
        ptr = rhs.ptr;
        rhs.ptr = 0;
        return *this;
    }
    struct Base {
        virtual Base *clone() = 0;
        virtual std::ostream &print(std::ostream &out) = 0;
        virtual ~Base(){};
    };
    template <typename T>
    struct Data : Base {
        T data;
        Data(const T &t) :
            data(t) {
        }
        virtual Base *clone() {
            return new Data<T>(data);
        }
        virtual std::ostream &print(std::ostream &out) {
            out << data;
            return out;
        }
        ~Data<T>() = default;
    };
    Base *ptr;
    template <typename T>
    T &get_data() {
        return ((Data<T> *)ptr)->data;
    }
    ~Any() {
        if (ptr) delete ptr;
    }
};
inline std::ostream &operator<<(std::ostream &out, const Any &oth) {
    oth.ptr->print(out);
    return out;
}
template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
    out << "[";
    for (auto it = v.begin(); it != v.end(); it++) {
        if (it != v.begin()) out << ",";
        out << *it;
    }
    out << "]";
    return out;
}
