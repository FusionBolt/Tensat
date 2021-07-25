#include "runner.h"
#include "pattern.h"

struct Lg : public Language
{
    //TODO:impl
    bool matches(Language &other) override
    {
        return true;
    }

    /// Runs a given function on each child `Id`.
    // TODO:const version
    void for_each(std::function<void(Id &)> f) override
    {

    }

    bool operator==(const Language &other) const override
    {
        // TODO:error
        return false;
    }

    bool operator!=(const Language &other) const override
    {
        // TODO:error
        return true;
    }

    bool operator>(const Language &other) const override
    {
        return false;
    }

    bool operator<(const Language &other) const override
    {
        return true;
    }
};

template<typename L>
struct LgAna
{
    using Data = int;

    // EL -> e_graph language
    template<class EL, class N>
    static void pre_union(EGraph<EL, N> &egraph, Id id1, Id id2)
    {

    }
};

std::vector<Rewrite<Lg, LgAna<Lg>>> rules()
{
    // name
    // lhs -> rhs
    Searcher<Lg, LgAna<Lg>> *searcher = new Pattern<Lg, LgAna<Lg>>();
    Applier<Lg, LgAna<Lg>> *applier = new Pattern<Lg, LgAna<Lg>>();
    std::vector<Rewrite<Lg, LgAna<Lg>>> rules;
    // TODO:if rule has (if cond), should support
    // TODO:if <=> need 2 rewrite(=> only 1)
    rules.emplace_back("rule A", searcher, applier);
}

int main()
{
    Runner<int, Analysis<int>> runner;
    runner.run();
    Extractor<int, Analysis<int>, int> extractor({}, {});
}
