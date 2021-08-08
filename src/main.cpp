#include <variant>
#include "runner.h"
#include "pattern.h"
#include "lg.h"
#include "language.h"

struct Conv
{
    int data;
    int kernel;
};
struct Matmul
{
    int data;
};

struct Add
{
    int v;
};

struct Ops : public Language
{
    enum ir_type
    {
        conv,
        add,
        matmul
    };

    template<class Op>
    Ops(ir_type type, const Op &op)
    {
        type_ = type;
        value_ = op;
        return op;
    }
    ir_type type_;
    std::variant<Conv, Add, Matmul> value_;
};

template<class L>
class OpsAnalysis
{

};

template<class L>
PatternAst<L> get_rec_expr_list()
{
    PatternAst<L> expr;
    ENodeOrVar<L> v;
    v.enode_ = Conv{4, 3};
    expr.push_back(v);
}

// TODO:a rule set class
template<class L, class N>
void append_rule(std::vector<Rewrite<L, N>> &rules, const PatternAst<L> &l, PatternAst<L> &r)
{
    // rewrite: lhs -> rhs
    Searcher<L, N> *searcher = new Pattern<L, N>(l);
    Applier<L, N> *applier = new Pattern<L, N>(r);
    rules.emplace_back("rule", searcher, applier);
}

std::vector<Rewrite<Ops, OpsAnalysis<Ops>>> rules()
{
    std::vector<Rewrite<Ops, OpsAnalysis<Ops>>> rule_set;
    append_rule(rule_set, RecExpr<L>);
}

int main()
{
    auto rs = rules();
    Runner<int, Analysis<int>> runner;
    runner.run();
    Extractor<int, Analysis<int>, int> extractor({}, {});
}
