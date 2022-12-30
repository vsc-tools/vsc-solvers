
#pragma once
#include <set>
#include <vector>
#include "vsc/dm/IContext.h"
#include "vsc/solvers/ISolverFactory.h"
#include "vsc/dm/impl/VisitorBase.h"

namespace vsc {
namespace solvers {


class TaskResizeConstrainedModelVec : public dm::VisitorBase {
public:
    TaskResizeConstrainedModelVec(
        dm::IContext        *ctxt,
        ISolverFactory      *solver_f);

    virtual ~TaskResizeConstrainedModelVec();

    bool resize(SolveSet *solve_s);

	virtual void visitModelExprFieldRef(dm::IModelExprFieldRef *f) override;

protected:
    dm::IContext                            *m_ctxt;
    ISolverFactory                          *m_solver_f;
    std::set<dm::IModelFieldVec *>          m_sizeref_vec_s;
    std::vector<dm::IModelField *>          m_sizeref_vec_l;
    std::vector<dm::IModelConstraintUP>     m_fixed_size_c;

};

}
}
