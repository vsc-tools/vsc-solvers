/*
 * Copyright 2019-2021 Matthew Ballance and contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Randomizer.cpp
 *
 *  Created on: Sep 26, 2021
 *      Author: mballance
 */

#include "dmgr/impl/DebugMacros.h"
#include "CommitFieldValueVisitor.h"
#include "Randomizer.h"
#include "SolveSetSwizzlerPartsel.h"
#include "SolveSpecBuilder.h"
#include "SolveSetSolveModelBuilder.h"
#include "SolverFactoryDefault.h"

namespace vsc {
namespace solvers {


Randomizer::Randomizer(
        dm::IContext        *ctxt,
		ISolverFactory		*solver_factory,
		IRandState 		    *randstate) :
				m_ctxt(ctxt), m_solver_factory(solver_factory), m_randstate(randstate) {
    DEBUG_INIT("Randomizer", ctxt->getDebugMgr());
}

Randomizer::~Randomizer() {
	// TODO Auto-generated destructor stub
}

bool Randomizer::randomize(
			const std::vector<dm::IModelField *>		&fields,
			const std::vector<dm::IModelConstraint *>	&constraints,
			bool									diagnose_failures) {
	bool ret = true;
	DEBUG_ENTER("randomize n_fields=%d n_constraints=%d",
			fields.size(),
			constraints.size());
	SolveSpecUP spec(SolveSpecBuilder(0).build(
			fields,
			constraints
			));

	DEBUG("%d solve-sets ; %d unconstrained",
			spec->solvesets().size(),
			spec->unconstrained().size());

	for (auto sset=spec->solvesets().begin();
			sset!=spec->solvesets().end(); sset++) {
		DEBUG("Solve Set: %d fields ; %d constraints",
				(*sset)->all_fields().size(),
				(*sset)->constraints().size());
		ISolverUP solver(m_solver_factory->createSolverInst(
            m_ctxt,
            sset->get())
        );

		// Build solve data for this solve set
		SolveSetSolveModelBuilder(solver.get()).build(sset->get());

		// First, ensure all constraints solve
		for (auto c_it=(*sset)->constraints().begin();
				c_it!=(*sset)->constraints().end(); c_it++) {
			solver->addAssume(*c_it);
		}
		for (auto c_it=(*sset)->soft_constraints().begin();
				c_it!=(*sset)->soft_constraints().end(); c_it++) {
			solver->addAssume(*c_it);
		}

		if (solver->isSAT()) {
			DEBUG("PASS: Initial try-solve for solveset");
			for (auto c_it=(*sset)->constraints().begin();
				c_it!=(*sset)->constraints().end(); c_it++) {
				solver->addAssert(*c_it);
			}
		} else {
			DEBUG("FAIL: Initial try-solve for solveset");

			ret = false;

			// TODO: Try backing off soft constraints
		}

		if (ret) {
			/*
			 */
			// Swizzle fields
			SolveSetSwizzlerPartsel(m_ctxt, m_randstate).swizzle(
					solver.get(),
					sset->get());

			// Ensure we're SAT
			if (!solver->isSAT()) {
				fprintf(stdout, "unsat post-swizzle\n");
			}

			for (auto f_it=(*sset)->rand_fields().begin();
					f_it!=(*sset)->rand_fields().end(); f_it++) {
				DEBUG("Commit %s", (*f_it)->name().c_str());
				CommitFieldValueVisitor(solver.get()).commit(*f_it);
			}
		} else {
			break;
		}
	}

	for (auto uc_it=spec->unconstrained().begin();
			uc_it!=spec->unconstrained().end(); uc_it++) {
		DEBUG("Randomize unconstrained field %s", (*uc_it)->name().c_str());
		m_randstate->randbits((*uc_it)->val());
	}

	DEBUG_LEAVE("randomize n_fields=%d n_constraints=%d ret=%d",
			fields.size(),
			constraints.size(),
			ret);
	return ret;
}

dmgr::IDebug            *Randomizer::m_dbg;

}
}

