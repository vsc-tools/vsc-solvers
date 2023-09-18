/**
 * SolverBoolectorConstraintBuilder.h
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
#include "dmgr/IDebugMgr.h"
#include "vsc/dm/IModelField.h"
#include "vsc/dm/impl/VisitorBase.h"
#include "vsc/solvers/impl/RefPathConstraint.h"

struct BoolectorNode;
struct Btor;

namespace vsc {
namespace solvers {

template <class T> class RefPathPtrMap;



class SolverBoolectorConstraintBuilder : public dm::VisitorBase {
public:
    SolverBoolectorConstraintBuilder(
        dmgr::IDebugMgr                             *dmgr,
        struct Btor                                 *btor,
        const RefPathPtrMap<struct BoolectorNode>   &field_m,
        dm::IModelField                             *root_field
    );

    virtual ~SolverBoolectorConstraintBuilder();

    struct BoolectorNode *build(const std::vector<int32_t> &path);

	virtual void visitDataTypeBool(dm::IDataTypeBool *t) override;

	virtual void visitDataTypeEnum(dm::IDataTypeEnum *t) override;

	virtual void visitDataTypeInt(dm::IDataTypeInt *t) override;

	virtual void visitTypeConstraintExpr(dm::ITypeConstraintExpr *c) override;

	virtual void visitTypeConstraintIfElse(dm::ITypeConstraintIfElse *c) override;

	virtual void visitTypeConstraintImplies(dm::ITypeConstraintImplies *c) override;

	virtual void visitTypeConstraintScope(dm::ITypeConstraintScope *c) override;

	virtual void visitTypeConstraintUnique(dm::ITypeConstraintUnique *c) override;

	virtual void visitTypeExprBin(dm::ITypeExprBin *e) override;

	virtual void visitTypeExprFieldRef(dm::ITypeExprFieldRef *e) override;

	virtual void visitTypeExprRangelist(dm::ITypeExprRangelist *e) override;

	virtual void visitTypeExprVal(dm::ITypeExprVal *e) override;

protected:
    using ExprT=std::pair<struct BoolectorNode *, bool>;

    enum class DataTypeMode {
        Literal,
        RefSign
    };

protected:
    ExprT booleanize(const ExprT &);

    ExprT maxsize(const ExprT &expr, const ExprT &other);

private:
    static dmgr::IDebug                             *m_dbg;
    struct Btor                                     *m_btor;
    const RefPathPtrMap<struct BoolectorNode>       &m_field_m;
    dm::IModelField                                 *m_root_field;
    std::vector<int32_t>                            m_path_prefix;
    int32_t                                         m_expr_sz_down;
    ExprT                                           m_expr;
    dm::ValRef                                      m_val;
    DataTypeMode                                    m_dt_mode;
};

}
}


