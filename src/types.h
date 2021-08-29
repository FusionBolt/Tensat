#pragma once

#include <optional>

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

    Id push_back(const L &node)
    {
        check_node_valid();
        nodes.push_back(node);
        return nodes.size() - 1;
    }

    void check_node_valid(const L &node)
    {
        node.all([&](Id id){
            assert(id < nodes.size());
        });
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
    void set_enode(const L& enode)
    {
        enode_ = enode;
        is_enode_ = true;
    }
    void set_var(const Var& var)
    {
        var_ = var;
        is_enode_ = false;
    }
};

template<class L>
using PatternAst = RecExpr<ENodeOrVar<L>>;
