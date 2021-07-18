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
                // auto ids = apply_one(egraph, match.eclass, subset);

                // egraph.union_find(id, matche.eclass);
                // added.extend(ids);
            }
        }
        return added;
    }

    // virtual
    std::vector<Id> apply_one(EGraph<L, N> &egraph, Id eclass, SubSet subset)
    {

    }
};

template<class L, class N>
class Rewrite
{
public:
    size_t apply(EGraph<L, N> &egraph, const std::vector<SearchMatches> &matches)
    {
        return applier.apply_matches(egraph, matches).size();
    }

    std::vector<SearchMatches> search(EGraph<L, N> &egraph)
    {
        return searcher.search(egraph);
    }

    Applier<L, N> applier;
    // TODO:replace Arc by Searcher&
    Arc<L, N> searcher;
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
