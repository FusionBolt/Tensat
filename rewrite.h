//
// Created by fusionbolt on 2021/7/17.
//

#ifndef TENSAT_REWRITE_H
#define TENSAT_REWRITE_H

#include "types.h"

template<class L, class N>
class Applier
{
public:
    std::vector<Id> apply_matches(EGraph<L, N> &egraph, const std::vector<SearchMatches> &matches)
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
class Searcher
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

    virtual std::vector<Var> vars() = 0;
};

template<class L, class N>
class Pattern;

template<class L, class N>
class Rewrite
{
public:
    // TODO:error
    Rewrite()
    {
        applier = new Pattern<L, N>();
    }

    size_t apply(EGraph<L, N> &egraph, const std::vector<SearchMatches> &matches)
    {
        return applier->apply_matches(egraph, matches).size();
    }

    std::vector<SearchMatches> search(EGraph<L, N> &egraph)
    {
        return searcher->search(egraph);
    }

    Applier<L, N> *applier;
    // TODO:replace SearcherArc by Searcher&
    Searcher<L, N> *searcher;
    std::string name;
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

#endif //TENSAT_REWRITE_H
