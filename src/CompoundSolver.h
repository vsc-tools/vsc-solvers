/**
 * CompoundSolver.h
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
#pragma once
#include "dmgr/IDebugMgr.h"
#include "vsc/solvers/ISolverFactory.h"
#include "vsc/solvers/ICompoundSolver.h"
#include "SolverUnconstrained.h"

namespace vsc {
namespace solvers {

class CompoundSolver : public ICompoundSolver {
public:
    CompoundSolver(
        dmgr::IDebugMgr     *dmgr,
        ISolverFactory      *solver_f
    );

    virtual ~CompoundSolver();

	virtual bool randomize(
			IRandState								    *randstate,
            const std::vector<dm::IModelFieldUP>        &root_fields,
            const RefPathSet                            &target_fields,
            const RefPathSet                            &fixed_fields,
            const RefPathSet                            &include_constraints,
            const RefPathSet                            &exclude_constraints,
			SolveFlags								    flags) override;

	virtual bool sat(
            const std::vector<dm::IModelFieldUP>        &root_fields,
            const RefPathSet                            &target_fields,
            const RefPathSet                            &fixed_fields,
            const RefPathSet                            &include_constraints,
            const RefPathSet                            &exclude_constraints,
			SolveFlags								    flags) override;

private:
    static dmgr::IDebug                 *m_dbg;
    ISolverFactory                      *m_solver_f;
    SolverUnconstrained                 m_solver_unconstrained;

};

}
}


