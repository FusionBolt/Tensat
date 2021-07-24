//
// Created by fusionbolt on 2021/7/17.
//

#ifndef TENSAT_TYPES_H
#define TENSAT_TYPES_H

using Id = size_t;

struct Var
{

};

struct SubSet
{
    // TODO:small vector
    std::vector<std::pair<Var, Id>> vec;
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
//        Bind { node: L, i: Reg, out: Reg },
//        Compare { i: Reg, j: Reg },
//    }
};

#endif //TENSAT_TYPES_H
