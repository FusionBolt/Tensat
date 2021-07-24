//
// Created by fusionbolt on 2021/7/17.
//

#ifndef TENSAT_PATTERN_H
#define TENSAT_PATTERN_H

#include "types.h"
#include "egraph.h"
#include "machine.h"
#include "language.h"
#include "rewrite.h"

template<class L>
struct ENodeOrVar
{
    L enode;
    Var var;
    bool is_enode_ = true;
    bool is_enode() { return is_enode_; }
};

template<class L>
using PatternAst = RecExpr<ENodeOrVar<L>>;


// TODO:不适合继承
// 这里模板类型接口可能有问题
template<class L, class N>
struct Pattern : Searcher<L, Analysis<L>>
{
    PatternAst<L> ast;
    Program<L> program;
    std::vector<SearchMatches> search(EGraph<L, N> &egraph) override
    {
        auto &nodes = ast.nodes;
        // TODO:为什么这里是last
        auto e = nodes[nodes.size() - 1];
        // TODO:node与var的区别
        if (e.is_enode())
        {
            auto key = e.enode;
            if(auto cls = egraph.classes_by_op; cls.find(key) != cls.end())
            {
                auto ids = cls[key];
                // filter_map
                std::vector<SearchMatches> result;
                for(auto &&id : ids)
                {
                    if(auto &&[ok, matches] = search_eclass(egraph, id); ok)
                    {
                        result.push_back(matches);
                    }
                }
                return result;
            }
            else
            {
                return {};
            }
        }
        else
        {
            // TODO:这里能否正确调用子类的search_eclass
            Searcher<L, Analysis<L>>::search(egraph);
        }
    }

    std::pair<bool, SearchMatches> search_eclass(EGraph<L, N> &egraph, Id eclass)
    {
        auto subsets = program.run(egraph, eclass);
        // TODO:refactor
        if(subsets.empty())
        {
            return {false, {}};
        }
        else
        {
            return {true, SearchMatches{eclass, subsets}};
        }
    }

    std::vector<Var> vars()
    {

    }
};


#endif //TENSAT_PATTERN_H
