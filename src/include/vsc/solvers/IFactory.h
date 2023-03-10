
#pragma once
#include "dmgr/IDebugMgr.h"
#include "vsc/solvers/ICompoundSolver.h"
#include "vsc/solvers/IRandState.h"

namespace vsc {
namespace solvers {

class IFactory {

public:
	virtual ~IFactory() { }

	virtual void init(dmgr::IDebugMgr *dmgr) = 0;

    virtual dmgr::IDebugMgr *getDebugMgr() = 0;

	virtual ICompoundSolver *mkCompoundSolver(dm::IContext *ctxt) = 0;

    virtual IRandState *mkRandState(const std::string &seed) = 0;


};


}
}

