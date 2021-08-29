#pragma once

#include <functional>
#include <memory>
#include "types.h"

struct Language
{
    // require methods
    virtual bool matches(Language &other) = 0;
    virtual std::vector<Id> children() const = 0;
    virtual std::vector<Id> &children() = 0;

    // provided methods
    // TODO:const version
    virtual void for_each(std::function<void(Id)> f) const = 0;
    virtual void for_each(std::function<void(Id&)> f) = 0;

    virtual std::string display_op() const = 0;

    // result<self, string>
    virtual std::shared_ptr<Language> from_op_str(const std::string &str, const std::vector<Id> &children) = 0;

    virtual size_t size() const = 0;

    virtual bool is_leaf() const = 0;
    /// Runs a given function to replace the children.
    template<class Callable>
    void update_children(Callable &&f)
    {
        // TODO:const version, ref is true?
        for_each([&](Id id)
                 { id = f(id); return id; });
    }

    /// Creates a new enode with children determined by the given function.
    template<class Callable>
    Language &map_children(Callable &&f)
    {
        update_children(std::forward<Callable &&>(f));
        return *this;
    }


    /// TODO:fold

    // TODO:to_recexpr
    bool all(std::function<bool(Id)> f)
    {
        // TODO:should test correctness
        auto acc = true;
        for_each([&](Id &id)
                 { acc = acc && f(id); });
        return acc;
    }

    //TODO:friendly?
    virtual bool operator==(const Language &other) const = 0;

    virtual bool operator!=(const Language &other) const = 0;

    virtual bool operator>(const Language &other) const = 0;

    virtual bool operator<(const Language &other) const = 0;
};

// TODO:virtual??
template<class L>
struct Analysis
{
    // TODO:error
    using Data = int;

    // TODO:concept
    // N -> analysis N
    template<class N>
    static void pre_union(EGraph<L, N> &egraph, Id id1, Id id2)
    {

    }

    // TODO:concept
    template<class N>
    static void make(EGraph<L, N> &egraph, L& node)
    {

    }
};
