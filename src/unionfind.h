#pragma once

#include <vector>
#include <cassert>
#include "types.h"
// TODO:what's this??
struct UnionFind
{
    // TODO:replace with a optimized union find
    std::vector<Id> parents_;

    Id make_set()
    {
        // TODO:atoi
        auto id = parents_.size();
        parents_.push_back(id);
        return id;
    }

    Id parent(Id query) const
    {
        // TODO:what??
        return parents_[query];
    }

    Id &parent_ref(Id query)
    {
        return parents_[query];
    }

    Id find(Id current) const
    {
        while(current != parent(current))
        {
            current = parent(current);
        }
        return current;
    }

    Id find(Id current)
    {
        while(current != parent(current))
        {
            auto grandparent = parent(parent(current));
            parent_ref(current) = grandparent;
            current = grandparent;
        }
        return current;
    }

    Id union_(Id root1, Id root2)
    {
        assert(root1 == parent(root1));
        assert(root2 == parent(root2));
        assert(root1 != root2);
        parent_ref(root2) = root1;
        return root1;
    }
};

