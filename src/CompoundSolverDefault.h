/*
 * CompoundSolverDefault.h
 *
 *  Created on: Mar 23, 2022
 *      Author: mballance
 */

#pragma once
#include "vsc/dm/IContext.h"
#include "vsc/solvers/ICompoundSolver.h"
#include "vsc/solvers/ISolverFactory.h"

namespace vsc {
namespace solvers {

class SolveSet;


class CompoundSolverDefault : public ICompoundSolver {
public:
	CompoundSolverDefault(
        dm::IContext        *ctxt,
        ISolverFactory      *solver_factory);

	virtual ~CompoundSolverDefault();

	virtual bool solve(
			IRandState								*randstate,
			const std::vector<dm::IModelField *>		&fields,
			const std::vector<dm::IModelConstraint *>	&constraints,
			SolveFlags								flags) override;

protected:
	bool solve_sset(
		SolveSet						*sset,
		ISolverFactory					*solver_f,
		IRandState						*randstate,
		SolveFlags						flags);

private:
	static dmgr::IDebug						*m_dbg;
	dm::IContext							*m_ctxt;
    ISolverFactory                          *m_solver_factory;
};

}
}

