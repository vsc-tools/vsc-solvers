/*
 * SolveSetSwizzlerPartsel.h
 *
 *  Created on: Oct 24, 2021
 *      Author: mballance
 */

#pragma once
#include "vsc/dm/IContext.h"
#include "vsc/solvers/ISolver.h"
#include "vsc/solvers/IRandState.h"
#include "SolveSet.h"

namespace vsc {
namespace solvers {


class SolveSetSwizzlerPartsel {
public:
	SolveSetSwizzlerPartsel(
        dm::IContext        *ctxt,
        IRandState          *rand_state);

	virtual ~SolveSetSwizzlerPartsel();

	void swizzle(
			ISolver		*solver,
			SolveSet	*sset);

private:
	void swizzle_field_l(
			const std::vector<dm::IModelField *>	&fields);

	void swizzle_field(
			dm::IModelField			*f,
			std::vector<dm::IModelConstraintUP>	&constraints);

	void create_rand_domain_constraint(
			dm::IModelField			*f,
			std::vector<dm::IModelConstraintUP>	&constraints);

private:
    static dmgr::IDebug     *m_dbg;
    dm::IContext            *m_ctxt;
	IRandState				*m_randstate;
	ISolver					*m_solver;
	SolveSet				*m_sset;

};

}
}


