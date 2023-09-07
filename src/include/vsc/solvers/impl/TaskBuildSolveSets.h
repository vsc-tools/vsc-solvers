/**
 * TaskBuildSolveSets.h
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
#include <vector>
#include "vsc/dm/IModelField.h"
#include "vsc/dm/impl/VisitorBase.h"
#include "vsc/solvers/IFactory.h"
#include "vsc/solvers/ISolveSet.h"
#include "vsc/solvers/impl/RefPathSet.h"

namespace vsc {
namespace solvers {



class TaskBuildSolveSets : public virtual dm::VisitorBase {
public:
    TaskBuildSolveSets(
        IFactory                                *factory,
        const std::vector<dm::IModelFieldUP>    &root_fields,
        const RefPathSet                        &target_fields,
        const RefPathSet                        &include_constraints,
        const RefPathSet                        &exclude_constraints
    ) : m_factory(factory), m_phase(0), m_root_fields(root_fields),
        m_target_fields(target_fields),
        m_include_constraints(include_constraints),
        m_exclude_constraints(exclude_constraints) { }

    virtual ~TaskBuildSolveSets() { }

    void build(
        std::vector<ISolveSetUP>        &solvesets,
        RefPathSet                      &unconstrained) {

        m_solvesets = &solvesets;
        m_unconstrained = &unconstrained;

        m_phase = 0; // Collect fields
        for (uint32_t i=0; i<m_root_fields.size(); i++) {
            m_path.push_back(i);
            m_root_fields.at(i)->getDataType()->accept(m_this);
            m_path.pop_back();
        }
    }

	virtual void visitDataTypeStruct(dm::IDataTypeStruct *t) override {
        for (uint32_t i=0; i<t->getFields().size(); i++) {
            m_path.push_back(i);
            t->getFields().at(i)->accept(m_this);
            m_path.pop_back();
        }
        for (uint32_t i=0; i<t->getConstraints().size(); i++) {
            m_path.push_back(i);
            t->getConstraints().at(i)->accept(m_this);
            m_path.pop_back();
        }
    }

	virtual void visitTypeFieldPhy(dm::ITypeFieldPhy *f) override {
        switch (m_phase) {
        case 0: { // Collecting declared fields
            m_unconstrained->add(m_path);
        } break;
        }
    }

protected:
    IFactory                                    *m_factory;
    uint32_t                                    m_phase;
    const std::vector<dm::IModelFieldUP>        &m_root_fields;
    const RefPathSet                            &m_target_fields;
    const RefPathSet                            &m_include_constraints;
    const RefPathSet                            &m_exclude_constraints;
    std::vector<int32_t>                        m_path;
    std::vector<ISolveSetUP>                    *m_solvesets;
    RefPathSet                                  *m_unconstrained;

};

}
}


