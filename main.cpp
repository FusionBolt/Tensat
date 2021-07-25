#include "runner.h"
#include "pattern.h"

struct Lg
{

};
template<typename L>
struct LgAna
{

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
