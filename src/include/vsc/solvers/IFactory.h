
#pragma once
#include "dmgr/IDebugMgr.h"
#include "vsc/solvers/ICompoundSolver.h"

namespace vsc {
namespace solvers {

class IFactory {

public:
	virtual ~IFactory() { }

	virtual void init(dmgr::IDebugMgr *dmgr) = 0;

    virtual dmgr::IDebugMgr *getDebugMgr() = 0;

	virtual ICompoundSolver *mkCompoundSolver(dm::IContext *ctxt) = 0;


};


}
}

