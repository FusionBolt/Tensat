//
// Created by fusionbolt on 2021/7/24.
//

#ifndef TENSAT_LANGUAGE_H
#define TENSAT_LANGUAGE_H

struct Language
{
    virtual bool matches(Language &other) = 0;

    /// Runs a given function on each child `Id`.
    // TODO:const version
    virtual void for_each(std::function<void(Id)> f) = 0;

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

    bool all(std::function<bool(Id)> f)
    {
        // TODO:should test correctness
        auto acc = true;
        for_each([&](Id id){ acc = acc && f(id);});
        return acc;
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
