/*
 * SolverFactoryDefault.h
 *
 *  Created on: Oct 23, 2021
 *      Author: mballance
 */

#pragma once
#include "vsc/dm/IContext.h"
#include "vsc/solvers/ISolverFactory.h"

namespace vsc {
namespace solvers {


class SolverFactoryDefault : public ISolverFactory {
public:
	SolverFactoryDefault();

	virtual ~SolverFactoryDefault();

    void init(dm::IContext *ctxt) { m_ctxt = ctxt; }

	virtual ISolver *createSolverInst(
        dm::IContext        *ctxt,
        SolveSet            *sset) override;

private:
    dm::IContext                        *m_ctxt;

};

}
}

