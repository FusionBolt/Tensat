#pragma once

#include <set>
#include "types.h"
#include "unionfind.h"
#include "eclass.h"
#include "util.h"

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
        auto class1_parents = classes_[i1].parents().size();
        auto class2_parents = classes_[i2].parents().size();
        if(class1_parents < class2_parents)
        {
            // TODO:may be has problem
            std::swap(i1, i2);
        }

        N::pre_union(*this, i1, i2);
    }

    Id find_mut(Id id)
    {
        return union_find.find(id);
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
    std::optional<Id> lookup(L& enode)
    {
        enode.update_children([&](auto &id){ return find(id); });
        if(auto id = memo.find(enode);id != memo.end())
        {
            return this->find(id->second);
        }
        else
        {
            return std::nullopt;
        }
    }

    std::optional<Id> lookup_expr(RecExpr<L> &expr)
    {
        auto nodes = expr.nodes;
        std::vector<Id> new_ids(nodes.size());
        for(auto &&node : nodes)
        {
            // TODO: should copy and should constraint callable type, should pass id_ ref
            node.map_children([&](auto &id){ return new_ids[id]; });
            if(auto id = lookup(node))
            {
                new_ids.push_back(id.value());
            }
        }
        return craft::last(new_ids);
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
            throw std::runtime_error("Invalid id_" + std::to_string(id));
        }
    }

    Id add_expr(RecExpr<L> &expr)
    {
        std::vector<Id> new_ids(expr.size());
        for(auto &&node : expr)
        {
            // TODO:what is this?
            auto n = node.map_children([&](Id i){
                return new_ids[i];
            });
            new_ids.push_back(add(n));
        }
        // TODO:impl a last
        // unwrap
        return new_ids[new_ids.size() - 1];
    }

    Id add(L &enode)
    {
        // TODO:not sure
        if(auto id = lookup(enode))
        {
            return id;
        }
        auto id = union_find.make_set();
        EClass<L, typename N::Data> new_class(id, enode.nodes(), N::make(*this, enode), {});

        // add this enode to the parent lists of its children
        // alloc id in this
        enode.for_each([&](Id child) {
            *this[child].parents().emplace_back({enode, id});
        });

        // TODO:don't know what is this, in egg has comment: "is this needed?"
        pending.emplace_back({enode, id});
        classes_.insert(id, new_class);
        N::modify(*this, id);
        return id;
    }

    std::map<Id, EClass<L, typename N::Data>> classes_;
    std::map<L, std::set<Id>> classes_by_op_;
    UnionFind union_find;
    std::map<L, Id> memo;
    std::vector<std::pair<L, Id>> pending;
};


