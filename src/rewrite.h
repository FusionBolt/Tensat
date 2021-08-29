#pragma once



#include <utility>
#include "types.h"

template<class L, class N>
struct Base
{
    virtual std::vector<Var> vars() = 0;
};

template<class L, class N>
class Applier : public Base<L, N>
{
public:
    virtual std::vector<Id> apply_matches(EGraph<L, N> &egraph, const std::vector<SearchMatches> &matches)
    {
        std::vector<Id> added;
        for (auto &&match : matches)
        {
            for (auto &&subset : match.subsets)
            {
                auto ids = apply_one(egraph, match.eclass, subset);
                for (auto &&id : ids)
                {
                    if(auto &&[to_id, do_something] = egraph.union_(id, match.eclass); do_something)
                    {
                        added.push_back(to_id);
                    }
                }
            }
        }
        return added;
    }

    // virtual
    virtual std::vector<Id> apply_one(EGraph<L, N> &egraph, Id eclass, SubSet subset) = 0;
};

template<class L, class N>
class Searcher : public Base<L, N>
{
public:
    virtual std::vector<SearchMatches> search(EGraph<L, N> &egraph)
    {
        // TODO:filter_map
        auto graph_classes = egraph.classes();
        std::vector<SearchMatches> result;
        for(auto &&[id, v] : graph_classes)
        {
            if(auto &&[ok, matches] = search_eclass(egraph, id); ok)
            {
                result.push_back(matches);
            }
        }
        return result;
    }

    virtual std::pair<bool, SearchMatches> search_eclass(EGraph<L, N> &egraph, Id eclass) = 0;
};

template<class L, class N>
class Rewrite
{
public:
    Rewrite(std::string name, Searcher<L, N> *searcher, Applier<L, N> *applier)
    : name_(std::move(name)), searcher_(searcher), applier_(applier)
    {
        auto bound_vars = searcher->vars();
        for(auto &&v : applier->vars())
        {
            if(std::find(bound_vars.begin(), bound_vars.end(), v) == bound_vars.end())
            {
                throw std::runtime_error("Rewrite" + name + "refers to unbound var" + v);
            }
        }
    }

    size_t apply(EGraph<L, N> &egraph, const std::vector<SearchMatches> &matches)
    {
        return applier_->apply_matches(egraph, matches).size();
    }

    std::vector<SearchMatches> search(EGraph<L, N> &egraph)
    {
        return searcher_->search(egraph);
    }

    std::string name()
    {
        return name_;
    }
    Applier<L, N> *applier_;
    // TODO:replace SearcherArc by Searcher&
    Searcher<L, N> *searcher_;
    std::string name_;
};

// interface
template<class L, class N>
class RewriteScheduler
{
public:
    bool can_stop(size_t i)
    {
        return true;
    }

    // TODO:iterator i
    std::vector<SearchMatches> search_rewrite(Rewrite<L, N> &rewrite, EGraph<L, N> &egraph)
    {
        return rewrite.search(egraph);
    }

    size_t apply_rewrite(EGraph<L, N> &egraph, Rewrite<L, N> &rewrite, const std::vector<SearchMatches> &matches)
    {
        return rewrite.apply(egraph, matches);
    }
};


