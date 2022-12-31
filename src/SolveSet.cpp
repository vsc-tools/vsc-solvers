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
 * SolveSet.cpp
 *
 *  Created on: Oct 7, 2021
 *      Author: mballance
 */

#include "dmgr/impl/DebugMacros.h"
#include "SolveSet.h"

namespace vsc {
namespace solvers {


SolveSet::SolveSet(dmgr::IDebugMgr *dmgr) : m_flags(SolveSetFlag::NoFlags) {
    DEBUG_INIT("SolveSet", dmgr);
}

SolveSet::~SolveSet() {
	// TODO Auto-generated destructor stub
}

void SolveSet::add_field(dm::IModelField *f) {
	DEBUG_ENTER("add_field %s", f->name().c_str());
	if (m_field_s.find(f) == m_field_s.end()) {
		m_all_fields.push_back(f);
		if (f->isFlagSet(dm::ModelFieldFlag::UsedRand)) {
			DEBUG("  Field is marked used-rand");
			m_rand_fields.push_back(f);
		}
		// If this is the size of a vector, save the vec
		if (f->isFlagSet(dm::ModelFieldFlag::VecSize)) {
			m_constrained_sz_vec.push_back(
					dynamic_cast<dm::IModelFieldVec *>(f->getParent()));
		}
		m_field_s.insert(f);
	}
	DEBUG_LEAVE("add_field %s", f->name().c_str());
}

void SolveSet::add_constraint(dm::IModelConstraint *c) {
	if (m_constraint_s.find(c) == m_constraint_s.end()) {
		m_constraints.push_back(c);
		m_constraint_s.insert(c);
	}
}

void SolveSet::add_soft_constraint(dm::IModelConstraintSoft *c) {
	if (m_soft_constraint_s.find(c) == m_soft_constraint_s.end()) {
		m_soft_constraints.push_back(c);
		m_soft_constraint_s.insert(c);
	}
}

void SolveSet::merge(SolveSet *src) {
	for (auto it=src->m_all_fields.begin();
			it!=src->m_all_fields.end(); it++) {
		add_field(*it);
	}
	for (auto it=src->m_constraints.begin();
			it!=src->m_constraints.end(); it++) {
		add_constraint(*it);
	}
	for (auto it=src->m_soft_constraints.begin();
			it!=src->m_soft_constraints.end(); it++) {
		add_soft_constraint(*it);
	}
	m_flags |= src->m_flags;
}

dmgr::IDebug *SolveSet::m_dbg = 0;

}
}
