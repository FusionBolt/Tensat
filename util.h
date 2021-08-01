//
// Created by fusionbolt on 2021/7/24.
//

#ifndef TENSAT_UTIL_H
#define TENSAT_UTIL_H

#include <optional>
#include <variant>

namespace craft
{
    template<template<class ...> class Container, class T>
    std::optional<T> find(const Container<T> &container, const T &v)
    {
        if (auto v_p = std::find(container.begin(), container.end(), v); v_p != container.end())
        {
            return *v_p;
        }
        else
        {
            return std::nullopt;
        }
    }

    template<class T, template<class L> class Container, class Callable>
    void for_each(const Container<T> &container, Callable &&f)
    {
        std::for_each(container.begin(), container.end(), f);
    }

    template<class T, class ...Args>
    std::optional<T> get(const std::variant<Args...> &v)
    {
        try
        {
            return std::get<T>(v);
        }
        catch (const std::runtime_error &e)
        {
            return std::nullopt;
        }
    }

    void assert_s(bool cond, const std::string &str)
    {
        if(!cond)
        {
            throw std::runtime_error(str);
        }
    }

    // TODO:changed this, only useful for vector and list
    // 顺序迭代器？还有一些类似的操作也是这样
    template<template<class...> class Container, class T>
    std::optional<T> last(const Container<T> &container)
    {
        if(container.empty())
        {
            return std::nullopt;
        }
        else
        {
            return container[container.size() - 1];
        }
    }
    // TODO:return what? new container or a Functional Base?
    template<class R, class T, template<class...> class Container, class Callable>
    Container<R> map(const Container<T> &container, Callable &&f)
    {

    }


// trash, I hope they will be helpful
//    template<class T>
//    struct FBase
//    {
//        // callable -> function<FBase<R>(T&&)>
//        template<class R, class Callable>
//        virtual FBase map(Callable &&f) = 0;
//
//        virtual T fold() = 0;
//    };

// or
//    template<class T, template<class...> class Container>
//    struct FContainer
//    {
//        Container<T> &container_;
//        FContainer(Container<T> &container) : container_(container)
//        {
//
//        }
//
//        // map()
//        // fold()
//    };


//    template<class T, template<class...> class Container, class Callable>
//    Container<T> filter(const Container<T> &container, Callable &&cond)
//    {
//        // TODO:only run by vector
//        Container<T> result;
//        for_each(container, [&](auto &&v){
//            if(cond(v))
//            {
//                result.push_back(v);
//            }
//        });
//    }
}
#endif //TENSAT_UTIL_H
