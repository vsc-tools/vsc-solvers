/*
 * Randomizer.h
 *
 *  Created on: Sep 26, 2021
 *      Author: mballance
 */
#pragma once
#include "vsc/dm/IContext.h"
#include "vsc/dm/IModelConstraint.h"
#include "vsc/solvers/ISolverFactory.h"
#include "vsc/solvers/IRandomizer.h"
#include "vsc/solvers/IRandState.h"
#include "RNG.h"

namespace vsc {
namespace solvers {


class Randomizer : public IRandomizer {
public:
	Randomizer(
            dm::IContext        *ctxt,
			ISolverFactory		*solver_factory,
			IRandState			*randstate);

	virtual ~Randomizer();

	virtual bool randomize(
			const std::vector<dm::IModelField *>		&fields,
			const std::vector<dm::IModelConstraint *>	&constraints,
			bool									diagnose_failures
			) override;

private:
    static dmgr::IDebug             *m_dbg;
    dm::IContext                    *m_ctxt;
	ISolverFactory		            *m_solver_factory;
	IRandState			            *m_randstate;

};

}
}

