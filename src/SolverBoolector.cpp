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
#include "SolverBoolectorConstraintBuilder.h"
#include "SolverBoolectorFieldBuilder.h"
#include "SolverBoolectorSetFieldValue.h"


namespace vsc {
namespace solvers {


SolverBoolector::SolverBoolector(dmgr::IDebugMgr *dmgr) : 
    m_dmgr(dmgr), m_issat(false) {
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
    bool ret = true;
    DEBUG_ENTER("randomize");

    // Solve set will tell us what fields are:
    // - target
    // - have a fixed value

    SolverBoolectorFieldBuilder builder(m_dmgr, m_btor, root_field);
    for (RefPathMap<SolveSetFieldType>::iterator
        it=solveset->getFields().begin(); it.next(); ) {
        if (it.value() == SolveSetFieldType::Target ||
            it.value() == SolveSetFieldType::NonTarget) {
            // Create a solver variable to represent
            m_field_m.add(
                it.path(),
                builder.build(it.path(), false));
        } else {
            // Create a literal to represent
            m_field_m.add(
                it.path(),
                builder.build(it.path(), true));
        }
    }

    // - Create variables for the target fields
    // - Create constraints, opportunistically creating variables for non-target fields
    std::vector<BoolectorNode *> constraints;
    SolverBoolectorConstraintBuilder c_builder(m_dmgr, m_btor, m_field_m, root_field);
    for (RefPathSet::iterator
        it=solveset->getConstraints().begin(); it.next(); ) {
        BoolectorNode *c = c_builder.build(it.path());
        boolector_assert(m_btor, c);
        constraints.push_back(c);
    }

    // Assert all hard constraints

    // Solve
    int32_t result = boolector_sat(m_btor);

    ret = (result == BTOR_RESULT_SAT);
    DEBUG("issat: %d", ret);

//    boolector_saddo

    // TODO: swizzle result for randomness

    // Finally, fix the values of target fields
    if (ret) {
        for (RefPathMap<SolveSetFieldType>::iterator
            it=solveset->getFields().begin(); it.next(); ) {
            if (it.value() == SolveSetFieldType::Target) {
                // Assign value
                BoolectorNode *n = m_field_m.find(it.path());
                SolverBoolectorSetFieldValue(m_dmgr, m_btor, root_field).set(
                    it.path(),
                    n);

            }
        }
    }

    DEBUG_LEAVE("randomize");
    return ret;
}

dmgr::IDebug *SolverBoolector::m_dbg = 0;

}
}
