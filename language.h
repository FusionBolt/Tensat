//
// Created by fusionbolt on 2021/7/24.
//

#ifndef TENSAT_LANGUAGE_H
#define TENSAT_LANGUAGE_H

struct Language
{
    /// Runs a given function to replace the children.
    template<class Callable>
    void update_children(Callable&& f)
    {
        for_each_mut([&](auto &id){ id = f(id);});
    }

    /// Creates a new enode with children determined by the given function.
    template<class Callable>
    Language& map_children(Callable&& f)
    {
        update_children(std::forward<Callable&&>(f));
    }
};

template<class L>
struct Analysis
{
    // TODO:error
    using Data = int;
    static void pre_union(EGraph<L, Analysis<L>>& egraph, Id id1, Id id2)
    {

    }
};

#endif //TENSAT_LANGUAGE_H
