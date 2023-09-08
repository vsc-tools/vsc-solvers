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
            const std::vector<dm::IModelFieldUP>        &root_fields,
            const RefPathSet                            &target_fields,
            const RefPathSet                            &fixed_fields,
            const RefPathSet                            &include_constraints,
            const RefPathSet                            &exclude_constraints,
			SolveFlags								    flags) {
    IFactory *factory = 0;
    std::vector<SolveSetUP>     solvesets;
    RefPathSet                  unconstrained;

    TaskBuildSolveSets(
        factory->getDebugMgr(),
        root_fields,
        target_fields,
        include_constraints,
        exclude_constraints).build(solvesets, unconstrained);

    if (!unconstrained.empty()) {
        RefPathSet fixed_fields;
        m_solver_unconstrained.randomize(
            randstate,
            root_fields,
            unconstrained,
            fixed_fields);
    }

    return true;
}

bool CompoundSolver::sat(
            const std::vector<dm::IModelFieldUP>        &root_fields,
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
