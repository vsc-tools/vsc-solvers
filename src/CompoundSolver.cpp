/*
 * CompoundSolver.cpp
 *
 * Copyright 2023 Matthew Ballance and Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may 
 * not use this file except in compliance with the License.  
 * You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 *
 * Created on:
 *     Author:
 */
#include "dmgr/impl/DebugMacros.h"
#include "CompoundSolver.h"
#include "TaskBuildSolveSets.h"


namespace vsc {
namespace solvers {


CompoundSolver::CompoundSolver(
    dmgr::IDebugMgr         *dmgr,
    ISolverFactory          *solver_f) : 
        m_dmgr(dmgr), m_solver_f(solver_f), 
        m_solver_unconstrained(dmgr) {
    DEBUG_INIT("vsc::solvers::CompoundSolver", dmgr);
}

CompoundSolver::~CompoundSolver() {

}

bool CompoundSolver::randomize(
			IRandState								    *randstate,
            dm::IModelField                             *root_field,
            const RefPathSet                            &target_fields,
            const RefPathSet                            &fixed_fields,
            const RefPathSet                            &include_constraints,
            const RefPathSet                            &exclude_constraints,
			SolveFlags								    flags) {
    std::vector<ISolveSetUP>    solvesets;
    RefPathSet                  unconstrained;

    TaskBuildSolveSets(
        m_dmgr,
        root_field,
        target_fields,
        fixed_fields,
        include_constraints,
        exclude_constraints).build(solvesets, unconstrained);

    // First, randomize any unconstrained fields
    if (!unconstrained.empty()) {
        RefPathSet fixed_fields;
        m_solver_unconstrained.randomize(
            randstate,
            root_field,
            unconstrained);
    }

    // Now, move on
    for (std::vector<ISolveSetUP>::const_iterator
        it=solvesets.begin();
        it!=solvesets.end(); it++) {
        ISolverUP solver(m_solver_f->mkSolver(it->get()));
        /*
        if (!solver->randomize(
            randstate,
            root_fields,
            (*it)->getFields(),

        ))
         */
    }

    return true;
}

bool CompoundSolver::sat(
            dm::IModelField                             *root_field,
            const RefPathSet                            &target_fields,
            const RefPathSet                            &fixed_fields,
            const RefPathSet                            &include_constraints,
            const RefPathSet                            &exclude_constraints,
			SolveFlags								    flags) {
    return true;
}

dmgr::IDebug *CompoundSolver::m_dbg = 0;

}
}
