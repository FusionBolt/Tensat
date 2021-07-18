//
// Created by fusionbolt on 2021/7/17.
//

#ifndef TENSAT_EGRAPH_H
#define TENSAT_EGRAPH_H

#include <set>
#include "types.h"

class EClass
{

};

// language, analysis
template<class L, class N>
class EGraph
{
public:
    void union_find(Id id1, Id id2)
    {

    }

    void rebuild()
    {

    }

    std::map<Id, EClass> classes()
    {
        return classes_;
    }

    size_t size()
    {
        // TOOD:error
        return classes_.size();
    }
    std::map<Id, EClass> classes_;

    std::map<L, std::set<Id>> classes_by_op;
};

#endif //TENSAT_EGRAPH_H
