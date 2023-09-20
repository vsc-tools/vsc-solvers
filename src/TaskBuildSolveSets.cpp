/*
 * TaskBuildSolveSets.cpp
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
#include "vsc/dm/ITypeExprFieldRef.h"
#include "vsc/solvers/impl/RefPathConstraint.h"
#include "TaskBuildSolveSets.h"


namespace vsc {
namespace solvers {


TaskBuildSolveSets::TaskBuildSolveSets(
        dmgr::IDebugMgr                         *dmgr,
        dm::IModelField                         *root_field,
        const RefPathSet                        &target_fields,
        const RefPathSet                        &fixed_fields,
        const RefPathSet                        &include_constraints,
        const RefPathSet                        &exclude_constraints
        ) : m_phase(0), m_root_field(root_field), 
        m_target_fields(target_fields), m_fixed_fields(fixed_fields),
        m_include_constraints(include_constraints), 
        m_exclude_constraints(exclude_constraints) {
    DEBUG_INIT("vsc::solvers::TaskBuildSolveSets", dmgr);
}

TaskBuildSolveSets::~TaskBuildSolveSets() {

}

void TaskBuildSolveSets::build(
    std::vector<ISolveSetUP>        &solvesets,
    RefPathSet                      &unconstrained) {
    m_active_ss_idx = -1;
    m_constraint_depth = 0;
    m_unconstrained = &unconstrained;

    m_phase = 0; // Collect variable references from constraints
    m_root_field->getDataType()->accept(m_this);

    m_phase = 1; // Collect unreferenced random fields
    m_root_field->getDataType()->accept(m_this);

    // Now, move the active solvesets to the destination
    for (std::vector<SolveSetUP>::iterator
        it=m_solveset_l.begin();
        it!=m_solveset_l.end(); it++) {
        if (it->get()) {
            solvesets.push_back(ISolveSetUP(it->release()));
        }
    }
}

void TaskBuildSolveSets::visitTypeConstraintExpr(dm::ITypeConstraintExpr *c) {
    DEBUG_ENTER("visitTypeConstraintExpr");
    enterConstraint();
    VisitorBase::visitTypeConstraintExpr(c);
    leaveConstraint();
    DEBUG_LEAVE("visitTypeConstraintExpr");
}

void TaskBuildSolveSets::visitTypeConstraintIfElse(dm::ITypeConstraintIfElse *c) {
    DEBUG_ENTER("visitTypeConstraintIfElse");
    enterConstraint();
    VisitorBase::visitTypeConstraintIfElse(c);
    leaveConstraint();
    DEBUG_LEAVE("visitTypeConstraintIfElse");
}

void TaskBuildSolveSets::visitTypeConstraintImplies(dm::ITypeConstraintImplies *c) {
    DEBUG_ENTER("visitTypeConstraintImplies");
    enterConstraint();
    VisitorBase::visitTypeConstraintImplies(c);
    leaveConstraint();
    DEBUG_LEAVE("visitTypeConstraintImplies");
}

void TaskBuildSolveSets::visitTypeConstraintScope(dm::ITypeConstraintScope *c) {
    DEBUG_ENTER("visitTypeConstraintScope");
    for (uint32_t i=0; i<c->getConstraints().size(); i++) {
        m_constraint_path.push_back(i);
        c->getConstraints().at(i)->accept(m_this);
        m_constraint_path.pop_back();
    }
    DEBUG_LEAVE("visitTypeConstraintScope");
}

void TaskBuildSolveSets::visitDataTypeBool(dm::IDataTypeBool *t) {
    // TODO: need to be careful to only do this selectively
    if (m_phase == 1) {
        int32_t idx;
        if (!m_field_ss_m.find(m_field_path, idx)) {
            DEBUG("Adding as unconstrained");
            m_unconstrained->add(m_field_path);
        } else {
            DEBUG("Already referenced");
        }
    }
}

void TaskBuildSolveSets::visitDataTypeEnum(dm::IDataTypeEnum *t) {
    // TODO: need to be careful to only do this selectively
    if (m_phase == 1) {
        int32_t idx;
        if (!m_field_ss_m.find(m_field_path, idx)) {
            DEBUG("Adding as unconstrained");
            m_unconstrained->add(m_field_path);
        } else {
            DEBUG("Already referenced");
        }
    }
}

void TaskBuildSolveSets::visitDataTypeInt(dm::IDataTypeInt *t) {
    DEBUG_ENTER("visitDataTypeInt");

    // TODO: need to be careful to only do this selectively
    if (m_phase == 1) {
        int32_t idx;
        if (!m_field_ss_m.find(m_field_path, idx)) {
            DEBUG("Adding as unconstrained");
            m_unconstrained->add(m_field_path);
        } else {
            DEBUG("Already referenced");
        }
    }
    DEBUG_LEAVE("visitDataTypeInt");
}

void TaskBuildSolveSets::visitDataTypeStruct(dm::IDataTypeStruct *t) {
    DEBUG_ENTER("visitDataTypeStruct nFields=%d", t->getFields().size());
    for (uint32_t i=0; i<t->getFields().size(); i++) {
        m_field_path.push_back(i);
        t->getFields().at(i)->accept(m_this);
        m_field_path.pop_back();
    }
    if (m_phase == 0) {
        // Constraint paths start with the crossover index
        // between datatype and constraint hierarchy
        m_constraint_path.clear();
        m_constraint_path.push_back(m_field_path.size()+1);
        m_constraint_path.insert(
            m_constraint_path.begin()+m_constraint_path.size(),
            m_field_path.begin(),
            m_field_path.end());
        for (uint32_t i=0; i<t->getConstraints().size(); i++) {
            m_constraint_path.push_back(i);
            t->getConstraints().at(i)->accept(m_this);
            m_constraint_path.pop_back();
        }
        m_constraint_path.clear();
    }
    DEBUG_LEAVE("visitDataTypeStruct");
}

void TaskBuildSolveSets::visitTypeExprBin(dm::ITypeExprBin *e) {
    DEBUG_ENTER("visitTypeExprBin");
    e->lhs()->accept(m_this);
    e->rhs()->accept(m_this);
    DEBUG_LEAVE("visitTypeExprBin");
}

void TaskBuildSolveSets::visitTypeExprFieldRef(dm::ITypeExprFieldRef *e) {
    DEBUG_ENTER("visitTypeExprFieldRef");
    uint32_t sz = m_field_path.size();
    m_field_path.insert(
        m_field_path.end(), 
        e->getPath().begin(), 
        e->getPath().end());
    processFieldRef(m_field_path);
    m_field_path.resize(sz);
    DEBUG_LEAVE("visitTypeExprFieldRef");
}

void TaskBuildSolveSets::visitTypeFieldPhy(dm::ITypeFieldPhy *f) {
    DEBUG_ENTER("visitTypeFieldPhy %s (%s)", 
        f->name().c_str(),
        m_field_path.toString().c_str());

    m_field_s.push_back(f);
    f->getDataType()->accept(m_this);
    m_field_s.pop_back();


    DEBUG_LEAVE("visitTypeFieldPhy");
}

void TaskBuildSolveSets::processFieldRef(const RefPathField &ref) {
    DEBUG_ENTER("processFieldRef %s", ref.toString().c_str());
    int32_t ex_ss_idx;
    SolveSet *ex_ss;

    if (m_field_ss_m.find(ref, ex_ss_idx)) {
        ex_ss = m_solveset_l.at(ex_ss_idx).get();
        // We've previously processed this field.

        if (m_active_ss_idx == -1) {
            // This becomes our active solve-set, but nothing
            // else to do
            m_active_ss_idx = ex_ss_idx;
        } else if (m_active_ss_idx != ex_ss_idx) {
            // Need to combine two solvesets. Try to move the 
            // least data by merging the smaller into the
            // currently-larger
            int32_t src_idx, dst_idx;

            if (m_solveset_l.at(m_active_ss_idx)->size() > ex_ss->size()) {
                dst_idx = m_active_ss_idx;
                src_idx = ex_ss_idx;
            } else {
                dst_idx = ex_ss_idx;
                src_idx = m_active_ss_idx;
            }

            DEBUG("Merging randset %d <- %d", dst_idx, src_idx);

            m_solveset_l.at(dst_idx)->merge(m_solveset_l.at(src_idx).get());

            // Now, all field paths in the source must be updated to the dest
            for (RefPathMap<SolveSetFieldType>::iterator 
                it=m_solveset_l.at(src_idx)->getFields().begin(); 
                it.next(); ) {
                m_field_ss_m.add(it.path(), dst_idx, true);
            }

            // Finally, remove the source from the list and update the active set
            m_solveset_l.at(src_idx).reset();
            m_active_ss_idx = dst_idx;
        }
    } else {
        // Field not-yet seen
        if (m_active_ss_idx == -1) {
            DEBUG("Creating a new randset");
            m_active_ss_idx = m_solveset_l.size();
            m_solveset_l.push_back(SolveSetUP(new SolveSet()));
        }

        bool is_target = (m_target_fields.size() == 0 || m_target_fields.find(ref));
        bool is_fixed = ((m_target_fields.size() && !is_target) 
                            || (m_fixed_fields.size() && m_fixed_fields.find(ref)));

        // A field is a non-fixed non-target if either target or
        // fixed fields are specified and the field is not part of either
        if (is_fixed) {
            m_solveset_l.at(m_active_ss_idx)->addField(
                ref, SolveSetFieldType::Fixed);
        } else if (is_target) {
            m_solveset_l.at(m_active_ss_idx)->addField(
                ref, SolveSetFieldType::Target);
        } else {
            m_solveset_l.at(m_active_ss_idx)->addField(
                ref, SolveSetFieldType::NonTarget);
        }
        DEBUG("Add field to randset");
        m_field_ss_m.add(ref, m_active_ss_idx);
    }
    DEBUG_LEAVE("processFieldRef");
}

void TaskBuildSolveSets::enterConstraint() {
    m_constraint_depth++;
}

void TaskBuildSolveSets::leaveConstraint() {
    m_constraint_depth--;

    if (!m_constraint_depth && m_active_ss_idx != -1) {
        DEBUG("Add constraint: %s", 
            RefPathConstraint(m_constraint_path).toString().c_str());
        m_solveset_l.at(m_active_ss_idx)->addConstraint(m_constraint_path);
    }
}

dmgr::IDebug *TaskBuildSolveSets::m_dbg = 0;

}
}
