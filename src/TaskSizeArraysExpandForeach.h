
#pragma once
#include "vsc/dm/IContext.h"
#include "vsc/dm/impl/VisitorBase.h"
#include "SolveSet.h"

namespace vsc {
namespace solvers {

namespace dm {

class TaskSizeArraysExpandForeach : public virtual VisitorBase {
public:

    TaskSizeArraysExpandForeach(IContext *ctxt);

    virtual ~TaskSizeArraysExpandForeach();

    void apply(SolveSet *sset);

private:
    IContext                    *m_ctxt;


};

}
}
}
};

}
}
}
