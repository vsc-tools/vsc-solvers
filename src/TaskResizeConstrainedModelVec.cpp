
#include "TaskResizeConstrainedModelVec.h"
#include "SolveSpecBuilder.h"

namespace vsc {
namespace solvers {



TaskResizeConstrainedModelVec::TaskResizeConstrainedModelVec(
        dm::IContext        *ctxt,
        ISolverFactory      *solver_f) : m_ctxt(ctxt), m_solver_f(solver_f) { 
}

TaskResizeConstrainedModelVec::~TaskResizeConstrainedModelVec() {

}

bool TaskResizeConstrainedModelVec::resize(SolveSet *solve_s) {
    m_fixed_size_c.clear();

    // Collect vectors with referenced size
    for (std::vector<dm::IModelConstraint *>::const_iterator
        it=solve_s->constraints().begin();
        it!=solve_s->constraints().end(); it++) {
        (*it)->accept(m_this);
    }

    // Build a new solve-set
    if (m_sizeref_vec_l.size() > 0) {
        SolveSpecUP solve_s(SolveSpecBuilder(m_ctxt).build(m_sizeref_vec_l, m_fixed_size_c));

//        for ()
    }


    return m_sizeref_vec_l.size() > 0;
}

void TaskResizeConstrainedModelVec::visitModelExprFieldRef(dm::IModelExprFieldRef *f) {
    if (f->field()->isFlagSet(dm::ModelFieldFlag::VecSize)) {
        // Add the vector to our list if it isn't there already
        dm::IModelFieldVec *vec = f->field()->getParentT<dm::IModelFieldVec>();
        if (m_sizeref_vec_s.insert(vec).second) {
            m_sizeref_vec_l.push_back(vec);

            // If this vector has no factory, then the size
            // is restricted to being less-equal to the 
            // current size
            if (!vec->getElemFactory()) {
                dm::IModelVal *size_v = m_ctxt->mkModelVal();
                size_v->set_val_u(vec->getSize(), 32);
                m_fixed_size_c.push_back(dm::IModelConstraintUP(
                    m_ctxt->mkModelConstraintExpr(
                        m_ctxt->mkModelExprBin(
                            m_ctxt->mkModelExprFieldRef(vec->getSizeRef()),
                            dm::BinOp::Le,
                            m_ctxt->mkModelExprVal(size_v)
                        )
                    )
                ));
            }

        }
    }
}

}
}

