/*
 * SolveSetSolveModelBuilder.h
 *
 *  Created on: Oct 23, 2021
 *      Author: mballance
 */

#pragma once
#include "vsc/solvers/ISolver.h"
#include "SolveSet.h"
#include "vsc/dm/impl/VisitorBase.h"

namespace vsc {
namespace solvers {


/**
 * Builds solver data for the fields and constraints in the
 * specified solve set
 */
class SolveSetSolveModelBuilder : public dm::VisitorBase {
public:
	SolveSetSolveModelBuilder(ISolver *solver);

	virtual ~SolveSetSolveModelBuilder();

	void build(SolveSet *sset);

private:
	ISolver						*m_solver;

};

}
}

