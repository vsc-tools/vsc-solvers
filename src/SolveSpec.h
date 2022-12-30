/*
 * SolveSpec.h
 *
 *  Created on: Oct 7, 2021
 *      Author: mballance
 */

#pragma once
#include <memory>
#include <vector>
#include "vsc/dm/IModelFieldVec.h"
#include "SolveSet.h"

namespace vsc {
namespace solvers {


class SolveSpec;
using SolveSpecUP = std::unique_ptr<SolveSpec>;

/**
 * Contains data specifying the solve problem
 * - Related fields
 * - Unconstrained fields
 * - ...
 */
class SolveSpec {
public:
	SolveSpec(
			const std::vector<SolveSet *>		&solvesets,
			const std::vector<dm::IModelField *>	&unconstrained,
			const std::vector<dm::IModelFieldVec *>	&unconstrained_sz_vec);

	virtual ~SolveSpec();

	const std::vector<SolveSetUP> &solvesets() const {
		return m_solvesets;
	}

	const std::vector<dm::IModelField *> &unconstrained() const {
		return m_unconstrained;
	}

	const std::vector<dm::IModelFieldVec *> &unconstrained_sz_vec() const {
		return m_unconstrained_sz_vec;
	}

private:
	std::vector<SolveSetUP>			m_solvesets;
	std::vector<dm::IModelField *>		m_unconstrained;
	std::vector<dm::IModelFieldVec *>	m_unconstrained_sz_vec;
};

}
}

