//
// Created by fusionbolt on 2021/7/17.
//

#ifndef TENSAT_PATTERN_H
#define TENSAT_PATTERN_H

#include "types.h"
#include "egraph.h"
#include "language.h"
#include "machine.h"
#include "rewrite.h"

// TODO:不适合继承
// 这里模板类型接口可能有问题
template<class L, class N>
struct Pattern : public Searcher<L, N>, public Applier<L, N>
{
    //TODO:this constructor error
    Pattern()
    {

    }
    Pattern(const PatternAst<L>& ast) : ast_(ast)
    {

    }
    PatternAst<L> ast_;
    Program<L> program_;
    std::vector<Id> apply_one(EGraph<L, N> &egraph, Id eclass, SubSet subset)
    {
        return {};
    }

    std::vector<SearchMatches> search(EGraph<L, N> &egraph) override
    {
        auto &nodes = ast_.nodes;
        // TODO:为什么这里是last
        auto e = nodes[nodes.size() - 1];
        // TODO:node与var的区别
        if (e.is_enode())
        {
            auto key = e.enode;
            if(auto cls = egraph.classes_by_op(); cls.find(key) != cls.end())
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
            return this->Searcher<L, N>::search(egraph);
        }
        // will not reach
        assert(false);
    }

    std::pair<bool, SearchMatches> search_eclass(EGraph<L, N> &egraph, Id eclass) override
    {
        auto subsets = program_.run(egraph, eclass);
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

    std::vector<Var> vars() override
    {
        std::vector<Var> result;
        for(auto n : ast_.nodes)
        {
            if (!n.is_enode_)
            {
                auto var = n.var;
                if(std::find(result.begin(), result.end(), var) != result.end())
                {
                    result.push_back(var);
                }
            }
        }
        return result;
    }

    void from_str()
    {

    }

    void from(const PatternAst<L>& ast)
    {
        ast_ = ast;
        program_ = Compiler<L>(ast).compile();
    }
};


#endif //TENSAT_PATTERN_H
