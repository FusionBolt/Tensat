//
// Created by fusionbolt on 2021/7/26.
//

#ifndef TENSAT_ECLASS_H
#define TENSAT_ECLASS_H

#include <vector>
#include "types.h"

template<class L, class D>
struct EClass
{
    // TODO:impl range for and iterator
    Id id_;
    std::vector<L> nodes_;
    D data_;
    std::vector<std::pair<L, Id>> parents_;

    // TODO:should be remove
    EClass() = default;

    EClass(Id id, const std::vector<L> &nodes, D data, const std::vector<std::pair<L, Id>> &parents)
    : id_(id), nodes_(nodes), data_(data), parents_(parents)
    {}

    [[nodiscard]] bool is_empty() const noexcept
    {
        return nodes_.empty();
    }

    [[nodiscard]] size_t size() const noexcept
    {
        return nodes_.size();
    }

    std::vector<L> leaves()
    {
        // TODO:condition push back
        std::vector<L> result;
        for(auto &&n : nodes_)
        {
            if(n.is_leaf())
            {
                result.push_back(n);
            }
        }
        return result;
    }


    // TODO:has many like this, may be a design problem, should refactor
    std::vector<std::pair<L, Id>> &parents()
    {
        return parents_;
    }

    std::vector<std::pair<L, Id>> parents() const
    {
        return parents_;
    }
    void assert_unique_leaves()
    {
        auto leaves = this->leaves();
        for(auto &&first : leaves)
        {
            assert_s(
                    leaves.all([&](L& l) {return l == first;}),
                    "different leaves in eclass:" + std::to_string(id_)
                    );
        }
    }

    std::vector<L>& nodes()
    {
        return nodes_;
    }

    // TODO:is good?
    std::vector<L> nodes() const
    {
        return nodes_;
    }
};

#endif //TENSAT_ECLASS_H
