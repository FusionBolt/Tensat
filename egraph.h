//
// Created by fusionbolt on 2021/7/17.
//

#ifndef TENSAT_EGRAPH_H
#define TENSAT_EGRAPH_H

#include <set>
#include "types.h"

template<class L, class D>
struct EClass
{
    Id id;
    std::vector<std::pair<L, Id>> parents;
    std::vector<L> nodes_;
    D data;
    std::vector<L>& nodes()
    {
        return nodes_;
    }
};

struct UnionFind
{
    Id find(Id id)
    {
        // TODO:finish
        return 1;
    }

    Id find_mut(Id id)
    {
        // TODO:finish
        return 1;
    }
};

// language, analysis
template<class L, class N>
class EGraph
{
public:
    std::pair<bool, Id> union_(Id id1, Id id2)
    {
        auto i1 = find_mut(id1);
        auto i2 = find_mut(id2);
        if(i1 == i2)
        {
            return {i1, false};
        }
        auto class1_parents = classes_[i1].parents.size();
        auto class2_parents = classes_[i2].parents.size();
        if(class1_parents < class2_parents)
        {
            // TODO:may be has problem
            std::swap(i1, i2);
        }

        N::pre_union(*this, i1, i2);
    }

    Id find_mut(Id id)
    {
        return union_find.find_mut(id);
    }
    void rebuild()
    {

    }

    std::map<Id, EClass<L, typename N::Data>> classes()
    {
        return classes_;
    }

    size_t size()
    {
        // TOOD:error
        return classes_.size();
    }
    std::map<Id, EClass<L, typename N::Data>> classes_;
    std::map<L, std::set<Id>> classes_by_op_;
    UnionFind union_find;
    std::map<L, Id> memo;

    std::map<L, std::set<Id>> &classes_by_op()
    {
        return classes_by_op_;
    }
    Id find(Id id)
    {
        return union_find.find(id);
    }
    // TODO:what is this
    // TODO:may be has some error?
    std::pair<bool, Id> lookup(L& enode)
    {
        enode.update_children([&](auto &id){ return find(id); });
        if(auto id = memo.find(enode);id != memo.end())
        {
            return {true, this->find(id->second)};
        }
        else
        {
            return {false, {}};
        }
    }

    std::pair<bool, Id> lookup_expr(RecExpr<L> &expr)
    {
        auto nodes = expr.nodes;
        std::vector<Id> new_ids(nodes.size());
        for(auto &&node : nodes)
        {
            // TODO: should copy and should constraint callable type, should pass id ref
            node.map_children([&](auto &id){ return new_ids[id]; });
            // auto id = lookup(node);
            // TODO:optional, should be same?
            if(auto [ok, id] = lookup(node); ok)
            {
                new_ids.push_back(id);
            }
        }
    }

    EClass<L, typename N::Data>& operator[](Id id)
    {
        auto i = find(id);
        if(classes_.find(id) != classes_.end())
        {
            return classes_[id];
        }
        else
        {
            throw std::runtime_error("Invalid id" + std::to_string(id));
        }
    }
};

#endif //TENSAT_EGRAPH_H
