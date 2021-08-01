//
// Created by fusionbolt on 2021/7/17.
//

#ifndef TENSAT_TYPES_H
#define TENSAT_TYPES_H

using Id = size_t;

using Var = std::string;

struct SubSet
{
    // TODO:small vector
    std::vector<std::pair<Var, Id>> vec_;

    std::optional<Id> insert(const Var &var, Id id)
    {
        // TODO:refactor
        for (auto &&[p_var, p_id] : vec_)
        {
            if (p_var == var)
            {
                auto tmp_id = p_id;
                p_id = id;
                return tmp_id;
            }
        }
        vec_.emplace_back(var, id);
        return std::nullopt;
    }
};

class SearchMatches
{
public:
    Id eclass;
    std::vector<SubSet> subsets;
};

template<class L>
struct RecExpr
{
    std::vector<L> nodes;

    size_t size()
    {
        return nodes.size();
    }

    void push_back(const L &l)
    {
        nodes.push_back(l);
    }

    L operator[](Id id)
    {
        return nodes[static_cast<size_t>(id)];
    }
};

using Reg = uint32_t;

template<class L>
struct Instruction
{
    struct Bind
    {
        L node;
        Reg i;
        Reg out;
    };
    struct Compare
    {
        Reg i;
        Reg j;
    };

    Instruction(Bind bind) : bind_(bind)
    {}

    Instruction(Compare compare) : compare_(compare)
    {}

    bool is_bind = true;

    Bind bind_;
    Compare compare_;
    // TODO:replace enum with variant
//    enum Instruction<L> {
//        Bind { node: L, i: Reg, out: Reg },
//        Compare { i: Reg, j: Reg },
//    }
};

template<class L>
struct ENodeOrVar
{
    L enode_;
    Var var_;
    // TODO:some good solution??
    bool is_enode_ = true;
    bool is_enode() { return is_enode_; }
    L& enode()
    {
        return enode_;
    }
};

template<class L>
using PatternAst = RecExpr<ENodeOrVar<L>>;

#endif //TENSAT_TYPES_H
