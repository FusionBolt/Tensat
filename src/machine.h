#pragma once



#include <algorithm>
#include <vector>
#include "types.h"
#include "language.h"
#include "index_map.h"
#include "util.h"

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
                auto bind = instruction.bind_;
                // TODO:rust while return??
                return for_each_matching_node(egraph[reg(bind.i)], bind.node, [&](L &matched)
                {
                    // let instructions_ = instructions_.as_slice
                    // [1, 2, 3] -> [2, 3] -> [2, 3] -> []
                    // TODO:maybe error 原本是迭代器，现在改成count，由于是递归，肯定会出问题
                    std::vector<Instruction<L>> remaining_instructions(instructions.begin(),
                                                                       instructions.end() - count);
                    // TODO:maybe error
                    // reg_ [0:out.0 + 1]
                    reg_.resize(std::min(static_cast<size_t>(bind.out), reg_.size()));

                    // TODO:un finish
                    matched.for_each([&](Id id)
                                     { reg_.push_back(id); });
                    run(egraph, remaining_instructions, subset, yield_fn);
                });
            }
            else
            {
                // TODO:reg_
                auto compare = instruction.compare_;
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
            for (auto n = nodes.begin() + start; n != nodes.end(); ++n)
            {
                if (node.matches(*n))
                {
                    f(*n);
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
struct Program;

template<class L>
struct Todo
{
    Reg reg_;
    bool is_ground_;
    size_t loc_;
    ENodeOrVar<L> pattern_;
};

// TODO:last is hash builder
using VarToReg = IndexMap<Var, Reg, int>;

// TODO:binary heap
//template<class T>
//using TodoList = std::vector<Todo<T>>;
template<class T>
struct TodoList
{
    std::optional<Todo<T>> pop()
    {
        if (list_.empty())
        {
            return std::nullopt;
        }
        else
        {
            auto last = list_[list_.size() - 1];
            list_.erase(list_.end() - 1);
            return last;
        }
    }

    std::vector<Todo<T>> list_;

    void push_back(Todo<T> &&todo)
    {
        list_.push_back(todo);
    }
//    template<class... Args>
//    T &emplace_back(Args &&... args)
//    {
//        return list_.emplace_back(std::forward<Args>(args)...);
//    }
};

template<class L>
struct Compiler
{
    Compiler(const PatternAst<L> &pattern, VarToReg v2r = {}, TodoList<L> todo = {}, Reg out = 0)
            : pattern_(pattern), v2r_(v2r), todo_(todo)
    {

    }

    std::pair<std::vector<RecExpr<L>>, std::vector<Reg>> get_ground_terms()
    {
        // get ground terms
        std::vector<bool> is_ground(pattern_.size());
        for (size_t i = 0; i < pattern_.size(); ++i)
        {
            if (pattern_[i].is_enode_)
            {
                auto &node = pattern_[i].enode();
                is_ground[i] = node.all([&](Id c)
                                        { return is_ground[c]; });
            }
        }
        auto ground_locs = get_ground_locs(is_ground);

        std::vector<RecExpr<L>> ground_terms;
        // TODO:some difference
        for (size_t i = 0; i < ground_locs.size(); ++i)
        {
            auto &&r = ground_locs[i];
            ground_terms.push_back(build_ground_terms(i));
        }
        append_todo(is_ground);
        return {ground_terms, ground_locs};
    }

    void append_todo(std::vector<bool> &is_ground)
    {
        auto pattern_last_index = pattern_.size() - 1;
        // TODO:change this
        todo_.push_back(Todo<L>{out_, is_ground[pattern_last_index], pattern_last_index, pattern_[pattern_last_index]});
        out_ += 1;
    }

    std::vector<Instruction<L>> get_instructions(const std::vector<Reg> &ground_locs)
    {
        std::vector<Instruction<L>> instructions;
        while (auto _todo = todo_.pop())
        {
            // TODO:fix this,replace by
            // pattern is ENodeOrVar
            auto todo = _todo.value();
            auto i = todo.reg_;
            auto pattern = todo.pattern_;
            auto loc = todo.loc_;
            if (!pattern.is_enode_)
            {
                auto v = pattern.var_;
                if (auto j = v2r_[v])
                {
                    instructions.emplace_back(typename Instruction<L>::Compare{i, j.value()});
                }
                else
                {
                    v2r_.insert(v, i);
                }
            }
            else
            {
                auto node = pattern.enode_;
                if (auto j = craft::find(ground_locs, Reg(loc)))
                {
                    instructions.emplace_back(typename Instruction<L>::Compare{i, j.value()});
                    continue;
                }
                auto out = out_;
                out_ += node.size();
                size_t id = 0;
                node.for_each([&](Id child)
                                {
                                    auto r = Reg(out + id);
                                    // TODO:must add
                                    // todo_.emplace_back(r, is_ground[child],child, pattern_[child]);
                                    id += 1;
                                });
                node.map_children([](auto v)
                                  { return 0; });
                instructions.emplace_back(typename Instruction<L>::Bind{node, i, out});
            }
        }
    }

    SubSet get_subset()
    {
        SubSet subst;
        // TODO:impl subst range for
//        for(auto s : v2r_)
//        {
//            // TODO:some diff
//            subst.insert(s.k, s.v);
//        }
        return subst;
    }

    Program<L> compile()
    {
        auto &&[ground_terms, ground_locs] = get_ground_terms();
        auto instructions = get_instructions(ground_locs);
        auto subst = get_subset();
        return Program<L>(instructions, ground_terms, subst);
    }

    RecExpr<L> build_ground_terms(size_t loc)
    {
        RecExpr<L> expr;
        return _build_ground_terms(loc, expr);
    }

    RecExpr<L> _build_ground_terms(size_t loc, RecExpr<L> &expr)
    {
        // copy
        if (auto _node = pattern_[loc]; _node.is_enode_)
        {
            // TODO:refactor
            auto node = _node.enode_;
            node.update_children([&](Id c)
                                 {
                                     _build_ground_terms(c, expr);
                                     // TODO:has difference
                                     return (expr.size() - 1);
                                 });
            expr.push_back(node);
        }
        else
        {
            throw std::runtime_error("could only build ground terms");
        }
    }

    std::vector<Reg> get_ground_locs(const std::vector<bool> &is_gound)
    {

    }

    PatternAst<L> pattern_;
    VarToReg v2r_;
    TodoList<L> todo_;
    Reg out_;
};

template<class L>
struct Program
{
    std::vector<Instruction<L>> instructions_;
    std::vector<RecExpr<L>> ground_terms_;
    SubSet subset_;

    //TODO:remove this when impl create/parse rule
    Program() = default;

    Program(const std::vector<Instruction<L>> &instruction,
            const std::vector<RecExpr<L>> &ground_term,
            const SubSet &subset)
            : instructions_(instruction), ground_terms_(ground_term), subset_(subset)
    {
    }

    template<class N>
    std::vector<SubSet> run(EGraph<L, N> &egraph, Id eclass)
    {
        Machine machine;
        assert(machine.reg_.empty());
        for (auto &&expr : ground_terms_)
        {
            if (auto id = egraph.lookup_expr(expr))
            {
                machine.reg_.push_back(id.value());
            }
            else
            {
                return {};
            }
        }
        machine.reg_.push_back(eclass);
        std::vector<SubSet> subsets;
        machine.run(egraph, instructions_, subset_, [&](Machine &machine, SubSet &subst)
        {
            auto subset_vec = subst.vec_;
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


