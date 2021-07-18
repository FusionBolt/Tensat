//
// Created by fusionbolt on 2021/7/17.
//

#ifndef TENSAT_TYPES_H
#define TENSAT_TYPES_H

using Id = size_t;

struct SubSet
{

};

class SearchMatches
{
public:
    Id eclass;
    std::vector<SubSet> subsets;
};


#endif //TENSAT_TYPES_H
