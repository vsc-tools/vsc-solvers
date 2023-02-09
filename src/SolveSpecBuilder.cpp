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
 * SolveSpecBuilder.cpp
 *
 *  Created on: Oct 7, 2021
 *      Author: mballance
 */
#include "dmgr/impl/DebugMacros.h"
#include "SolveSpecBuilder.h"
#include "vsc/dm/impl/TaskResolveModelExprFieldRef.h"

namespace vsc {
namespace solvers {


SolveSpecBuilder::SolveSpecBuilder(dm::IContext *ctx) : 
	m_ctx(ctx),
	m_pass(0), 
	m_flags(SolveSetFlag::NoFlags),
	m_active_solveset(0) {
    DEBUG_INIT("SolveSpecBuilder", ctx->getDebugMgr());
}

SolveSpecBuilder::~SolveSpecBuilder() {
	// TODO Auto-generated destructor stub
}

SolveSpec *SolveSpecBuilder::build(
			const std::vector<dm::IModelField *>		&fields,
			const std::vector<dm::IModelConstraint *>	&constraints
			) {
	DEBUG_ENTER("build %d fields %d constraints", fields.size(), constraints.size());

	m_pass = 0;
	for (std::vector<dm::IModelField *>::const_iterator
			it=fields.begin();
			it!=fields.end(); it++) {
		(*it)->accept(this);
	}

	m_pass = 1;
	for (auto it=fields.begin(); it!=fields.end(); it++) {
		(*it)->accept(this);
	}

	for (auto it=constraints.begin(); it!=constraints.end(); it++) {
		DEBUG_ENTER("Visiting constraint");
		(*it)->accept(this);
		DEBUG_LEAVE("Visiting constraint");
	}

	std::vector<SolveSet *> solvesets;
	for (auto it=m_solveset_l.begin(); it!=m_solveset_l.end(); it++) {
		if (it->get()) {
			solvesets.push_back(it->release());
		}
	}

	std::vector<dm::IModelField *> unconstrained;
	std::vector<dm::IModelFieldVec *> unconstrained_sz_vec;

	for (auto it=m_unconstrained_l.begin(); it!=m_unconstrained_l.end(); it++) {
		if (*it) {
			if ((*it)->isFlagSet(dm::ModelFieldFlag::VecSize)) {
				unconstrained_sz_vec.push_back((*it)->getParentT<dm::IModelFieldVec>());
			} else if ((*it)->isFlagSet(dm::ModelFieldFlag::UsedRand)) {
				unconstrained.push_back(*it);
			}
		}
	}

	SolveSpec *spec = new SolveSpec(
			solvesets,
			unconstrained,
			unconstrained_sz_vec);

	DEBUG_LEAVE("build");
	return spec;
}

SolveSpec *SolveSpecBuilder::build(
			const std::vector<dm::IModelField *>		&fields,
			const std::vector<dm::IModelConstraintUP>	&constraints
			) {
	DEBUG_ENTER("build %d fields %d constraints", fields.size(), constraints.size());

	m_pass = 0;
	for (std::vector<dm::IModelField *>::const_iterator
			it=fields.begin();
			it!=fields.end(); it++) {
		(*it)->accept(this);
	}

	m_pass = 1;
	for (auto it=fields.begin(); it!=fields.end(); it++) {
		(*it)->accept(this);
	}

	for (std::vector<dm::IModelConstraintUP>::const_iterator
		it=constraints.begin(); 
		it!=constraints.end(); it++) {
		DEBUG_ENTER("Visiting constraint");
		it->get()->accept(this);
		DEBUG_LEAVE("Visiting constraint");
	}

	std::vector<SolveSet *> solvesets;
	for (auto it=m_solveset_l.begin(); it!=m_solveset_l.end(); it++) {
		if (it->get()) {
			solvesets.push_back(it->release());
		}
	}

	std::vector<dm::IModelField *> unconstrained;
	std::vector<dm::IModelFieldVec *> unconstrained_sz_vec;

	for (auto it=m_unconstrained_l.begin(); it!=m_unconstrained_l.end(); it++) {
		if (*it) {
			if ((*it)->isFlagSet(dm::ModelFieldFlag::VecSize)) {
				unconstrained_sz_vec.push_back((*it)->getParentT<dm::IModelFieldVec>());
			} else {
				unconstrained.push_back(*it);
			}
		}
	}

	SolveSpec *spec = new SolveSpec(
			solvesets,
			unconstrained,
			unconstrained_sz_vec);

	DEBUG_LEAVE("build");
	return spec;
}

void SolveSpecBuilder::visitDataTypeEnum(dm::IDataTypeEnum *t) {
	DEBUG_ENTER("visitDataTypeEnum");

	if (m_pass == 0) {
		// Save the field
		dm::IModelField *field = m_field_s.back();
		auto it = m_unconstrained_m.find(field);

		if (it == m_unconstrained_m.end()) {
			DEBUG("Add field %s to unconstrained set", field->name().c_str());
			m_unconstrained_m.insert({field, m_unconstrained_l.size()});
			m_unconstrained_l.push_back(field);
		}
	}

	DEBUG_LEAVE("visitDataTypeEnum");
}

void SolveSpecBuilder::visitDataTypeInt(dm::IDataTypeInt *t) {
	DEBUG_ENTER("visitDataTypeInt");

	if (m_pass == 0) {
		// Save the field
		dm::IModelField *field = m_field_s.back();
		auto it = m_unconstrained_m.find(field);

		if (it == m_unconstrained_m.end()) {
			DEBUG("Add field %s to unconstrained set", field->name().c_str());
			m_unconstrained_m.insert({field, m_unconstrained_l.size()});
			m_unconstrained_l.push_back(field);
		}
	}

	DEBUG_LEAVE("visitDataTypeInt");
}

void SolveSpecBuilder::visitDataTypeStruct(dm::IDataTypeStruct *t) {
	// Don't recurse to look 
}

void SolveSpecBuilder::visitModelConstraintExpr(dm::IModelConstraintExpr *c) {
	DEBUG_ENTER("visitModelConstraintExpr pass=%d", m_pass);
	constraint_enter(c);
	dm::VisitorBase::visitModelConstraintExpr(c);
	constraint_leave(c);
	DEBUG_LEAVE("visitModelConstraintExpr pass=%d", m_pass);
}

void SolveSpecBuilder::visitModelConstraintForeach(dm::IModelConstraintForeach *c) {
	DEBUG_ENTER("visitModelConstraintForeach");
	constraint_enter(c);

	m_flags |= SolveSetFlag::HaveForeach;

	dm::VisitorBase::visitModelConstraintForeach(c);

	constraint_leave(c);
	DEBUG_LEAVE("visitModelConstraintForeach");
}

void SolveSpecBuilder::visitModelConstraintIfElse(dm::IModelConstraintIfElse *c) {
	DEBUG_ENTER("visitModelConstraintIf");
	constraint_enter(c);
	dm::VisitorBase::visitModelConstraintIfElse(c);
	constraint_leave(c);
	DEBUG_LEAVE("visitModelConstraintIf");
}

void SolveSpecBuilder::visitModelConstraintImplies(dm::IModelConstraintImplies *c) {
	DEBUG_ENTER("visitModelConstraintImplies");
	constraint_enter(c);
	dm::VisitorBase::visitModelConstraintImplies(c);
	constraint_leave(c);
	DEBUG_LEAVE("visitModelConstraintImplies");
}

void SolveSpecBuilder::visitModelExprFieldRef(dm::IModelExprFieldRef *e) {
	DEBUG_ENTER("visitModelExprFieldRef");
	if (m_pass == 0) {
		// In pass 0, we're collecting fields
		e->field()->accept(this);
	} else {
		// In pass 1, we're evaluating dependencies
		process_fieldref(e->field());
	}
	DEBUG_LEAVE("visitModelExprFieldRef");
}

void SolveSpecBuilder::visitModelExprIndexedFieldRef(dm::IModelExprIndexedFieldRef *e) {
	DEBUG_ENTER("visitModelExprIndexedFieldRef");
	dm::IModelField *field = dm::TaskResolveModelExprFieldRef(m_ctx).resolve(0, e);

	// We ignore constraints via null field references
	if (!field) {
		DEBUG_LEAVE("visitModelExprIndexedFieldRef: null reference");
		return;
	}

	if (m_pass == 0) {
		// In pass 0, we're collecting fields
		field->accept(this);
	} else {
		// In pass 1, we're evaluating dependencies
		process_fieldref(field);
	}
	DEBUG_LEAVE("visitModelExprIndexedFieldRef");
}

void SolveSpecBuilder::visitModelField(dm::IModelField *f) {
	DEBUG_ENTER("visitModelField %s", f->name().c_str());
	m_field_s.push_back(f);
	if (m_pass == 0) {
		if (f->getDataType()) {
			f->getDataType()->accept(this);
		}
		for (std::vector<dm::IModelField *>::const_iterator
				it=f->getFields().begin();
				it!=f->getFields().end(); it++) {
			(*it)->accept(m_this);
		}
	} else {
		dm::VisitorBase::visitModelField(f);
	}
	m_field_s.pop_back();
	DEBUG_LEAVE("visitModelField %s", f->name().c_str());
}

void SolveSpecBuilder::constraint_enter(dm::IModelConstraint *c) {
	DEBUG_ENTER("constraint_enter %d", m_constraint_s.size());
	m_constraint_s.push_back(c);
	DEBUG_LEAVE("constraint_enter %d", m_constraint_s.size());
}

void SolveSpecBuilder::constraint_leave(dm::IModelConstraint *c) {
	DEBUG_ENTER("constraint_leave pass=%d %d", m_pass, m_constraint_s.size());
	m_constraint_s.pop_back();

	if (m_pass == 1 && m_constraint_s.size() == 0) {
		if (m_active_solveset) {
			m_active_solveset->add_constraint(c);
			m_active_solveset->setFlags(m_flags);
		}
		m_flags = SolveSetFlag::NoFlags;
	}
	DEBUG_LEAVE("constraint_leave pass=%d %d", m_pass, m_constraint_s.size());
}

void SolveSpecBuilder::process_fieldref(dm::IModelField *f) {
	DEBUG_ENTER("process_fieldref %s", f->name().c_str());
	std::unordered_map<dm::IModelField *, SolveSet *>::const_iterator f_it;

	if ((f_it=m_solveset_field_m.find(f)) != m_solveset_field_m.end()) {
		// This field is part of an existing solveset
		DEBUG("In an existing solve set");

		if (!m_active_solveset) {
			// Accept this solveset as the active one
			DEBUG("No active solve set, so take this one");
			m_active_solveset = f_it->second;
		} else if (m_active_solveset != f_it->second) {
			// We have an active solveset which is not the same
			// as the one containing this field. We need to
			// merge the two sets
			DEBUG("Must merge this solve-set with the active one");

			// Relink fields into the new consolidated solveset
			for (std::vector<dm::IModelField *>::const_iterator
					it=f_it->second->all_fields().begin();
					it!=f_it->second->all_fields().end(); it++) {
				// Update the solveset this field is mapped to
				auto it_e = m_solveset_field_m.find(*it);
				it_e->second = m_active_solveset;
			}

			// Merge data into the active solveset
			m_active_solveset->merge(f_it->second);

			// Now, remove the old solveset
			auto solveset_i = m_solveset_m.find(f_it->second);
			m_solveset_l[solveset_i->second] = 0;
		} else {
			DEBUG("Existing solve-set and active are the same");
		}
	} else {
		// The field isn't current managed by a solveset
		DEBUG("Not in an existing solve set (active=%p)", m_active_solveset);

		if (!m_active_solveset) {
			m_active_solveset = new SolveSet(m_ctx->getDebugMgr());
			m_solveset_m.insert({m_active_solveset, m_solveset_l.size()});
			m_solveset_l.push_back(SolveSetUP(m_active_solveset));
		}

		// Remove the field from the unconstrained set
		auto uc_it = m_unconstrained_m.find(f);
		if (uc_it != m_unconstrained_m.end()) {
			m_unconstrained_l.at(uc_it->second) = 0;
		}

		m_active_solveset->add_field(f);
		m_solveset_field_m.insert({f, m_active_solveset});
	}

	DEBUG_LEAVE("process_fieldref %s", f->name().c_str());
}

dmgr::IDebug *SolveSpecBuilder::m_dbg = 0;
}
}
