
#pragma once
#include "vsc/dm/IContext.h"
#include "vsc/dm/impl/VisitorBase.h"
#include "SolveSet.h"

namespace vsc {
namespace solvers {

class TaskSizeArraysExpandForeach : public virtual dm::VisitorBase {
public:

    TaskSizeArraysExpandForeach(dm::IContext *ctxt);

    virtual ~TaskSizeArraysExpandForeach();

    void apply(SolveSet *sset);

private:
    dm::IContext                    *m_ctxt;


};

}
}
