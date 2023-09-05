/*
 * SolverUnconstrained.cpp
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
#include "SolverUnconstrained.h"


namespace vsc {
namespace solvers {


SolverUnconstrained::SolverUnconstrained(dmgr::IDebugMgr *dmgr) {
    DEBUG_INIT("vsc::solvers::SolverUnconstrained", dmgr);

}

SolverUnconstrained::~SolverUnconstrained() {

}

bool SolverUnconstrained::randomize(
        IRandState                              *randstate,
        const std::vector<dm::IModelFieldUP>    &root_fields,
        const RefPathSet                        &target_fields,
        const RefPathSet                        &fixed_fields) {
    DEBUG_ENTER("randomize");
    DEBUG_LEAVE("randomize");
    return true;
}

dmgr::IDebug *SolverUnconstrained::m_dbg = 0;

}
}
