//
// Created by fusionbolt on 2021/7/25.
//

#ifndef TENSAT_RUNNER_H
#define TENSAT_RUNNER_H
#include <iostream>
#include <vector>
#include <map>
#include "egraph.h"
#include "pattern.h"
#include "rewrite.h"
#include "index_map.h"
#include "extractor.h"
#include "types.h"

// TODO:const and ref, refactor
// TODO:type, access
// TODO:type name alias

struct Stopper
{
    bool is_stop() const noexcept { return is_stop_; }

    bool is_stop_ = false;
    std::string stop_reason_;
};

struct Limit
{
    uint64_t time_limit_;
    uint64_t size_limit_;
    uint64_t iterations_limit_;
    void check_limits(uint64_t time, uint64_t node_size, uint64_t iterations_limit)
    {
        // TODO:finish
        if(time > time_limit_)
        {

        }
        if(node_size > size_limit_)
        {

        }
        if(iterations_limit > iterations_limit_)
        {

        }
    }
};

template<class IterData>
struct Iterator
{

};

// TODO:IterData default is ()
template<class L, class N = Analysis<L>, class IterData = void>
class Runner
{
public:
    void run()
    {
        // check_rules();
        egraph.rebuild();
        while (!stopper_.is_stop())
        {
            run_single(rules_);
        }
    }

    void try_start()
    {
        // TODO:what is it
    }
    Iterator<IterData> run_single(std::vector<Rewrite<L, N>> &rules)
    {
        check_limits();
        try_start();
        // TODO:is not
        auto egraph_nodes_after_hooks = egraph_nodes;
        auto egraph_classes_after_hooks = egraph_classes;
        size_t i = iterations.size();
        // TODO:只需要规则就能get？？
        auto matches = get_matches(rules);
        auto applied = apply_rewrite(egraph, rules, matches);
        egraph.rebuild();
        // TODO:scheduler的不同实现，具体实现更改can_stop行为
        auto can_be_saturated = applied.is_empty()
                                && scheduler.can_stop(i)
                                && (egraph_nodes == egraph_nodes_after_hooks)
                                && (egraph_classes == egraph_classes_after_hooks);
        return Iterator<IterData>{};
    }

    std::vector<std::vector<SearchMatches>> get_matches(std::vector<Rewrite<L, N>> &rules)
    {
        std::vector<std::vector<SearchMatches>> matches;
        for (auto &&rule : rules)
        {
            auto ms = scheduler.search_rewrite(rule, egraph);
            matches.push_back(ms);
            check_limits();
        }
        return matches;
    }

    IndexMap<int, int, int> apply_rewrite(EGraph<L, N> &egraph, std::vector<Rewrite<L, N>> &rules,
                                          std::vector<std::vector<SearchMatches>> &matches)
    {
        auto applied = DefaultIndexMap();
        for (size_t i = 0; i < rules.size(); ++i)
        {
            auto &rw = rules[i];
            auto &match = matches[i];
            auto actual_matched = scheduler.apply_rewrite(egraph, rw, match);
            if (actual_matched > 0)
            {
                // 存在的话就增加，否则新建这个条目
                if(auto count = applied.get_mut(rw.name()))
                {
                    count += actual_matched;
                }
                else
                {
                    applied.insert(rw.name(), actual_matched);
                }
            }
            check_limits();
        }
        return applied;
    }

    void check_limits()
    {
        // TODO:change param
        limit_.check_limits(0, 0, 0);
    }

    Limit limit_;
    Stopper stopper_;
    std::vector<Rewrite<L, N>> rules_;
    EGraph<L, N> egraph;
    RewriteScheduler<L, N> scheduler;
    size_t egraph_classes;
    size_t egraph_nodes;
    std::vector<Iterator<IterData>> iterations;
};
// TODO:抽象出一个run_with_check_limit
#endif //TENSAT_RUNNER_H
