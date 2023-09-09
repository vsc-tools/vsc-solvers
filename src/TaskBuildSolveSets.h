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
#include "vsc/solvers/impl/RefPathMap.h"
#include "SolveSet.h"

namespace vsc {
namespace solvers {



class TaskBuildSolveSets : public virtual dm::VisitorBase {
public:
    TaskBuildSolveSets(
        dmgr::IDebugMgr                         *dmgr,
        dm::IModelField                         *root_field,
        const RefPathSet                        &target_fields,
        const RefPathSet                        &fixed_fields,
        const RefPathSet                        &include_constraints,
        const RefPathSet                        &exclude_constraints);

    virtual ~TaskBuildSolveSets();

    void build(
        std::vector<ISolveSetUP>        &solvesets,
        RefPathSet                      &unconstrained);

	virtual void visitDataTypeStruct(dm::IDataTypeStruct *t) override;

	virtual void visitTypeConstraintExpr(dm::ITypeConstraintExpr *c) override;

	virtual void visitTypeConstraintIfElse(dm::ITypeConstraintIfElse *c) override;

	virtual void visitTypeConstraintImplies(dm::ITypeConstraintImplies *c) override;

	virtual void visitTypeConstraintScope(dm::ITypeConstraintScope *c) override;

	virtual void visitTypeExprFieldRef(dm::ITypeExprFieldRef *e) override;

	virtual void visitTypeFieldPhy(dm::ITypeFieldPhy *f) override;

protected:

    void processFieldRef(const std::vector<int32_t> &ref);

    void enterConstraint();

    void leaveConstraint();

protected:
    static dmgr::IDebug                         *m_dbg;
    uint32_t                                    m_phase;
    dm::IModelField                             *m_root_field;
    const RefPathSet                            &m_target_fields;
    const RefPathSet                            &m_fixed_fields;
    const RefPathSet                            &m_include_constraints;
    const RefPathSet                            &m_exclude_constraints;
    std::vector<int32_t>                        m_field_path;
    std::vector<int32_t>                        m_constraint_path;

    RefPathMap<int32_t>                         m_field_ss_m;
    int32_t                                     m_active_ss_idx;
    int32_t                                     m_constraint_depth;
    std::vector<SolveSetUP>                     m_solveset_l;
    RefPathSet                                  *m_unconstrained;
};

}
}


