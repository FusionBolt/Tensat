#pragma once
#include "runner.h"
#include "pattern.h"

struct Lg : public Language
{
    //TODO:impl
    bool matches(Language &other) override
    {
        return true;
    }

    /// Runs a given function on each child `Id`.
    // TODO:const version
    void for_each(std::function<void(Id &)> f) override
    {

    }

    size_t size()
    {
        // TODO: call fold
        return 1;
    }

    bool operator==(const Language &other) const override
    {
        // TODO:error
        return false;
    }

    bool operator!=(const Language &other) const override
    {
        // TODO:error
        return true;
    }

    bool operator>(const Language &other) const override
    {
        return false;
    }

    bool operator<(const Language &other) const override
    {
        return true;
    }
};

template<typename L>
struct LgAna
{
    using Data = int;

    // EL -> e_graph language
    template<class EL, class N>
    static void pre_union(EGraph<EL, N> &egraph, Id id1, Id id2)
    {

    }
};