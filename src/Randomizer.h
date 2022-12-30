/*
 * Randomizer.h
 *
 *  Created on: Sep 26, 2021
 *      Author: mballance
 */
#pragma once
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
			ISolverFactory		*solver_factory,
			IRandState			*randstate);

	virtual ~Randomizer();

	virtual bool randomize(
			const std::vector<dm::IModelField *>		&fields,
			const std::vector<dm::IModelConstraint *>	&constraints,
			bool									diagnose_failures
			) override;

private:
	ISolverFactory		*m_solver_factory;
	IRandState			*m_randstate;

};

}
}

