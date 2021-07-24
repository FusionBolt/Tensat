//
// Created by fusionbolt on 2021/7/24.
//

#ifndef TENSAT_MACHINE_H
#define TENSAT_MACHINE_H

#include "types.h"
#include "language.h"

struct Machine
{
    std::vector<Id> reg_;

    template<class L, class N, class Callable>
    void run(EGraph<L, N> &egraph, std::vector<Instruction<L>> &instructions,
             SubSet &subset, Callable &&yield_fn)
    {
        size_t count = 0;
        for (auto &&instruction : instructions)
        {
            if (instruction.is_bind)
            {
                auto bind = instruction.bind;
                // TODO:rust while return??
                return for_each_matching_node(egraph[reg(bind.i)], bind.node, [&](L &matched)
                {
                    // let instructions = instructions.as_slice
                    // [1, 2, 3] -> [2, 3] -> [2, 3] -> []
                    // TODO:maybe error
                    decltype(instructions) remaining_instructions(instructions.begin(), instructions.end() - count);
                    // TODO:maybe error
                    // reg_ [0:out.0 + 1]
                    reg_.resize(std::min(bind.out, reg_.size()));

                    // TODO:un finish
                    matched.for_each([&](Id id)
                                     { reg_.push_back(id); });
                    run(egraph, remaining_instructions, subset, yield_fn);
                });
            }
            else
            {
                // TODO:reg_
                auto compare = instruction.compare;
                if (egraph.find(reg(compare.i)) != egraph.find(reg(compare.j)))
                {
                    return;
                }
            }
        }
        yield_fn(*this, subset);
    }

    template<class L, class D, class Callable>
    void for_each_matching_node(EClass<L, D> &eclass, L &node, Callable &&f)
    {
        auto nodes = eclass.nodes();
        if (nodes.size() < 50)
        {
            for (auto &&n : nodes)
            {
                // todo
                if (node.matches(n))
                {
                    f(n);
                }
            }
        }
        else
        {
            // TODO:debug assert node.all == 0
            // TODO:debug assert eclass.nodes.windows(2) w[0] < w[1]

            // 二分查找，查找到的位置，或者说中间位置
            // 比如说 1 3 5，如果node是4，那返回2
            // nodes.binary_search(node)
            size_t start = 0;
            while (start > 0)
            {
                // TODO:why need -1
                if (nodes[start - 1] == node)
                {
                    start -= 1;
                }
                else
                {
                    break;
                }
            }
            // decltype(nodes) matching;
            for(auto n = nodes.begin() + start; n != nodes.end(); ++n)
            {
                if(node.matches(n))
                {
                    f(n);
                    // matching.push_back(n);
                }
            }
            // TODO:debug assert ref machines this fun
            // matching.for_each(f);
        }
    }

    Id reg(Reg r)
    {
        return reg_[r];
    }
};

template<class L>
struct Program
{
    std::vector<Instruction<L>> instructions;
    std::vector<RecExpr<L>> ground_terms;
    SubSet subset;

    std::vector<SubSet> run(EGraph<L, Analysis<L>> &egraph, Id eclass)
    {
        Machine machine;
        assert(machine.reg_.empty());
        for (auto &&expr : ground_terms)
        {
            if (auto[ok, id] = egraph.lookup_expr(expr); ok)
            {
                machine.reg_.push_back(id);
            }
            else
            {
                return {};
            }
        }
        machine.reg_.push_back(eclass);
        std::vector<SubSet> subsets;
        machine.run(egraph, instructions, subset, [&](Machine &machine, SubSet &subst)
        {
            auto subset_vec = subst.vec;
            for (auto &&v : subset_vec)
            {
                auto var = v.first;
                auto reg_id = v.second;
                // TODO:has some difference
                v = {var, machine.reg_[reg_id]};
            }
            // TODO:refactor constructor
            subsets.push_back(SubSet{subset_vec});
        });
        return subsets;
    }
};

#endif //TENSAT_MACHINE_H
