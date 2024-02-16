/*
 * SolverBoolectorConstraintBuilder.cpp
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
#include "vsc/dm/ITypeExprFieldRef.h"
#include "vsc/dm/ITypeExprVal.h"
#include "vsc/dm/impl/ValRefBool.h"
#include "vsc/dm/impl/ValRefInt.h"
#include "vsc/solvers/impl/RefPathPtrMap.h"
#include "vsc/solvers/impl/RefPathField.h"
#include "vsc/solvers/impl/TaskPath2Constraint.h"
#include "vsc/solvers/impl/TaskPath2Field.h"
#include "SolverBoolectorConstraintBuilder.h"


namespace vsc {
namespace solvers {


SolverBoolectorConstraintBuilder::SolverBoolectorConstraintBuilder(
    dmgr::IDebugMgr                         *dmgr,
    Btor                                    *btor,
    const RefPathPtrMap<BoolectorNode>      &field_m,
    dm::IModelField                         *root_field) :
    m_btor(btor), m_field_m(field_m), m_root_field(root_field),
    m_expr_sz_down(0), m_dt_mode(DataTypeMode::Literal) {
    DEBUG_INIT("vsc::solvers::SolverBoolectorConstraintBuilder", dmgr);
}

SolverBoolectorConstraintBuilder::~SolverBoolectorConstraintBuilder() {

}

BoolectorNode *SolverBoolectorConstraintBuilder::build(const std::vector<int32_t> &path) {
    DEBUG_ENTER("build");
    m_expr = {0, false};

    int32_t constraint_offset = *(path.begin());

    // 
    m_path_prefix.clear();
    m_path_prefix.insert(
        m_path_prefix.begin(),
        path.begin()+1,
        path.begin()+constraint_offset);

    // Now, locate and build the constraint
    dm::ITypeConstraint *c = TaskPath2Constraint(m_root_field).toConstraint(path);

    c->accept(m_this);

    DEBUG_LEAVE("build");
    return m_expr.first;
}

void SolverBoolectorConstraintBuilder::visitDataTypeBool(dm::IDataTypeBool *t) {
    DEBUG_ENTER("visitDataTypeBool");
    if (m_dt_mode == DataTypeMode::Literal) {
        dm::ValRefBool val(m_val);

        m_expr = {boolector_const(
            m_btor,
            val.get_val()?"1":"0"),
            false};
    } else if (m_dt_mode == DataTypeMode::RefSign) {
        m_expr.second = false;
    }
    DEBUG_LEAVE("visitDataTypeBool");
}

void SolverBoolectorConstraintBuilder::visitDataTypeEnum(dm::IDataTypeEnum *t) {
    if (m_dt_mode == DataTypeMode::Literal) {
    } else if (m_dt_mode == DataTypeMode::RefSign) {
    }
}

void SolverBoolectorConstraintBuilder::visitDataTypeInt(dm::IDataTypeInt *t) {
    DEBUG_ENTER("visitDataTypeInt");
    if (m_dt_mode == DataTypeMode::Literal) {
        dm::ValRefInt val(m_val);

        if (val.bits() <= 64) {
            char tmp[32];
            sprintf(tmp, "%llx", val.get_val_u());
            m_expr = {
                boolector_consth(
                    m_btor,
                    boolector_bitvec_sort(m_btor, val.bits()),
                    tmp),
                t->isSigned()
            };
        } else {
            // TODO:
        }
    } else if (m_dt_mode == DataTypeMode::RefSign) {
        m_expr.second = t->isSigned();
    }
    DEBUG_LEAVE("visitDataTypeInt");
}

void SolverBoolectorConstraintBuilder::visitTypeConstraintExpr(dm::ITypeConstraintExpr *c) {
    DEBUG_ENTER("visitTypeConstraintExpr");
    c->expr()->accept(m_this);
    DEBUG_LEAVE("visitTypeConstraintExpr");
}

void SolverBoolectorConstraintBuilder::visitTypeConstraintIfElse(dm::ITypeConstraintIfElse *c) { 

}

void SolverBoolectorConstraintBuilder::visitTypeConstraintImplies(dm::ITypeConstraintImplies *c) { 

}

void SolverBoolectorConstraintBuilder::visitTypeConstraintScope(dm::ITypeConstraintScope *c) { 
    VisitorBase::visitTypeConstraintScope(c);
}

void SolverBoolectorConstraintBuilder::visitTypeConstraintUnique(dm::ITypeConstraintUnique *c) { 

}

void SolverBoolectorConstraintBuilder::visitTypeExprBin(dm::ITypeExprBin *e) { 
    DEBUG_ENTER("visitTypeExprBin");
    m_expr = {0, false};
    e->lhs()->accept(m_this);
    ExprT lhs = m_expr;

    m_expr = {0, false};
    e->rhs()->accept(m_this);
    ExprT rhs = m_expr;

    bool is_signed = lhs.second && rhs.second;

    // Prepare operands
    switch (e->op()) {
        case dm::BinOp::LogAnd:
        case dm::BinOp::LogOr: 
        case dm::BinOp::LogXor: { // Operands must be boolean
            lhs = booleanize(lhs);
            rhs = booleanize(rhs);
        } break;

        default: { // Operands must be same sized
            lhs = maxsize(lhs, rhs);
            rhs = maxsize(rhs, lhs);

        } break;
    }

    m_expr.second = is_signed;
    switch (e->op()) {
	    case dm::BinOp::Eq:
            m_expr.first = boolector_eq(
                m_btor,
                lhs.first,
                rhs.first);
        case dm::BinOp::Ne:
            m_expr.first = boolector_ne(
                m_btor,
                lhs.first,
                rhs.first);
            break;
        case dm::BinOp::Gt:
            if (is_signed) {
                m_expr.first = boolector_sgt(
                    m_btor,
                    lhs.first,
                    rhs.first);
            } else {
                m_expr.first = boolector_ugt(
                    m_btor,
                    lhs.first,
                    rhs.first);
            }
            break;
        case dm::BinOp::Ge:
            if (is_signed) {
                m_expr.first = boolector_sgte(
                    m_btor,
                    lhs.first,
                    rhs.first);
            } else {
                m_expr.first = boolector_ugte(
                    m_btor,
                    lhs.first,
                    rhs.first);
            }
            break;
        case dm::BinOp::Lt:
            if (is_signed) {
                m_expr.first = boolector_slt(
                    m_btor,
                    lhs.first,
                    rhs.first);
            } else {
                m_expr.first = boolector_ult(
                    m_btor,
                    lhs.first,
                    rhs.first);
            }
            break;
        case dm::BinOp::Le:
            if (is_signed) {
                m_expr.first = boolector_slte(
                    m_btor,
                    lhs.first,
                    rhs.first);
            } else {
                m_expr.first = boolector_ulte(
                    m_btor,
                    lhs.first,
                    rhs.first);
            }
            break;
            m_expr.first = boolector_add(
                m_btor,
                lhs.first,
                rhs.first);
            break;
        case dm::BinOp::Sub:
            m_expr.first = boolector_sub(
                m_btor,
                lhs.first,
                rhs.first);
            break;
        case dm::BinOp::Div:
            if (is_signed) {
                m_expr.first = boolector_sdiv(
                    m_btor,
                    lhs.first,
                    rhs.first);
            } else {
                m_expr.first = boolector_udiv(
                    m_btor,
                    lhs.first,
                    rhs.first);
            }
            break;
        case dm::BinOp::Mul:
            m_expr.first = boolector_mul(
                m_btor,
                lhs.first,
                rhs.first);
            break;
        case dm::BinOp::Mod:
            if (is_signed) {
                m_expr.first = boolector_smod(
                    m_btor,
                    lhs.first,
                    rhs.first);
            } else {
                m_expr.first = boolector_umulo(
                    m_btor,
                    lhs.first,
                    rhs.first);
            }
            break;
        case dm::BinOp::BinAnd:
            m_expr.first = boolector_and(
                m_btor,
                lhs.first,
                rhs.first);
            break;
        case dm::BinOp::BinOr:
            m_expr.first = boolector_or(
                m_btor,
                lhs.first,
                rhs.first);
            break;
        case dm::BinOp::BinXor:
            m_expr.first = boolector_xor(
                m_btor,
                lhs.first,
                rhs.first);
            break;
        case dm::BinOp::LogAnd:
            m_expr.first = boolector_and(
                m_btor,
                lhs.first,
                rhs.first);
            break;
        case dm::BinOp::LogOr:
            m_expr.first = boolector_or(
                m_btor,
                lhs.first,
                rhs.first);
            break;
        case dm::BinOp::LogXor:
            m_expr.first = boolector_xor(
                m_btor,
                lhs.first,
                rhs.first);
            break;
        case dm::BinOp::Sll:
            m_expr.first = boolector_sll(
                m_btor,
                lhs.first,
                rhs.first);
            break;
        case dm::BinOp::Srl:
            m_expr.first = boolector_srl(
                m_btor,
                lhs.first,
                rhs.first);
            break;
    }

    DEBUG_LEAVE("visitTypeExprBin");
}

void SolverBoolectorConstraintBuilder::visitTypeExprFieldRef(dm::ITypeExprFieldRef *e) { 
#ifdef UNDEFINED
    DEBUG_ENTER("visitTypeExprFieldRef path.size=%d prefix=%s", 
        e->getPath().size(),
        RefPathField(m_path_prefix).toString().c_str());
    int32_t prefix_sz = m_path_prefix.size();
    m_path_prefix.insert(
        m_path_prefix.end(),
        e->getPath().begin(),
        e->getPath().end()
    );

    m_expr.first = m_field_m.find(m_path_prefix);
    DEBUG("node @ %s: %p", RefPathField(m_path_prefix).toString().c_str(), m_expr.first);

    DataTypeMode dt_mode = m_dt_mode;
    m_dt_mode = DataTypeMode::RefSign;
    TaskPath2Field(m_root_field).toField(m_path_prefix)->getDataType()->accept(m_this);
    m_dt_mode = dt_mode;

    m_path_prefix.resize(prefix_sz);
    DEBUG_LEAVE("visitTypeExprFieldRef");
#endif /* UNDEFINED */
}

void SolverBoolectorConstraintBuilder::visitTypeExprRangelist(dm::ITypeExprRangelist *e) { 

}

void SolverBoolectorConstraintBuilder::visitTypeExprVal(dm::ITypeExprVal *e) { 
    DEBUG_ENTER("visitTypeExprVal");
    m_val = e->val();
    e->val().type()->accept(m_this);

    DEBUG_LEAVE("visitTypeExprVal");
}

SolverBoolectorConstraintBuilder::ExprT SolverBoolectorConstraintBuilder::booleanize(const ExprT &expr) {
    int32_t expr_sz = boolector_bitvec_sort_get_width(
        m_btor,
        boolector_get_sort(m_btor, expr.first));
    
    if (expr_sz != 1) {
        return {
            boolector_ne(
                m_btor,
                expr.first,
                boolector_zero(m_btor, boolector_get_sort(m_btor, expr.first))),
            expr.second};
    } else {
        return expr;
    }
}

SolverBoolectorConstraintBuilder::ExprT SolverBoolectorConstraintBuilder::maxsize(const ExprT &expr, const ExprT &other) {
    int32_t expr_sz = boolector_bitvec_sort_get_width(
        m_btor,
        boolector_get_sort(m_btor, expr.first));
    int32_t other_sz = boolector_bitvec_sort_get_width(
        m_btor,
        boolector_get_sort(m_btor, other.first));
    bool is_signed = (expr.second && other.second);
    int32_t max = (other_sz>m_expr_sz_down)?other_sz:m_expr_sz_down;

    if (max > expr_sz) {
        if (is_signed) {
            return { boolector_sext( m_btor, expr.first, (max-expr_sz)), is_signed };
        } else {
            return { boolector_uext( m_btor, expr.first, (max-expr_sz)), is_signed };
        }
    } else {
        return expr;
    }
}

dmgr::IDebug *SolverBoolectorConstraintBuilder::m_dbg = 0;

}
}
