//
// Created by fusionbolt on 2021/7/24.
//

#ifndef TENSAT_UTIL_H
#define TENSAT_UTIL_H

#include <optional>

namespace craft
{
    template<class T, template<class L> class Container>
    std::optional<T> find(const Container<T> &container, const T &v)
    {
        if (auto v_p = std::find(container.begin(), container.end(), v); v_p != container.end())
        {
            return v_p;
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
}
#endif //TENSAT_UTIL_H
