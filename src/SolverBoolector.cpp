/*
 * SolverBoolector.cpp
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
#include "boolector/boolector.h"
#include "dmgr/impl/DebugMacros.h"
#include "SolverBoolector.h"


namespace vsc {
namespace solvers {


SolverBoolector::SolverBoolector(dmgr::IDebugMgr *dmgr) {
    DEBUG_INIT("vsc::solvers::SolverBoolector", dmgr);

	m_btor = boolector_new();
	boolector_set_opt(m_btor, BTOR_OPT_INCREMENTAL, 1);
	boolector_set_opt(m_btor, BTOR_OPT_MODEL_GEN, 1);
}

SolverBoolector::~SolverBoolector() {
	boolector_release_all(m_btor);
	boolector_delete(m_btor);
}

bool SolverBoolector::randomize(
        IRandState                              *randstate,
        dm::IModelField                         *root_field,
        ISolveSet                               *solveset) {
    DEBUG_ENTER("randomize");

    // Solve set will tell us what fields are:
    // - target
    // - have a fixed value

    for (RefPathMap<SolveSetFieldType>::iterator
        it=solveset->getFields().begin(); it.next(); ) {

    }
    // - Create variables for the target fields
    // - Create constraints, opportunistically creating variables for non-target fields

    DEBUG_LEAVE("randomize");
    return true;
}

dmgr::IDebug *SolverBoolector::m_dbg = 0;

}
}
