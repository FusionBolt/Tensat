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
        for(auto &&[p_var, p_id] : vec_)
        {
            if(p_var == var)
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

    void push_back(const L& l)
    {
        nodes.push_back(l);
    }
};

using Reg = uint32_t;

template<class L>
struct Instruction
{
    bool is_bind = true;
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
    Bind bind;
    Compare compare;
    // TODO:replace enum with variant
//    enum Instruction<L> {
//        Bind { node: L, i: Reg, out_: Reg },
//        Compare { i: Reg, j: Reg },
//    }
};

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

#endif //TENSAT_TYPES_H
