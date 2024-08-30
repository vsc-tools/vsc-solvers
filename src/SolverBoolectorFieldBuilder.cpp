/*
 * SolverBoolectorFieldBuilder.cpp
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
#include "vsc/dm/impl/ValRefBool.h"
#include "vsc/dm/impl/ValRefInt.h"
#include "vsc/solvers/impl/TaskPath2Field.h"
#include "SolverBoolectorFieldBuilder.h"


namespace vsc {
namespace solvers {


SolverBoolectorFieldBuilder::SolverBoolectorFieldBuilder(
    dmgr::IDebugMgr         *dmgr,
    Btor                    *btor,
    vsc::dm::IModelField    *root_field) : m_btor(btor), 
        m_root_field(root_field), m_is_fixed(false) {
    DEBUG_INIT("vsc::solvers::SolverBoolectorFieldBuilder", dmgr);


}

SolverBoolectorFieldBuilder::~SolverBoolectorFieldBuilder() {

}

BoolectorNode *SolverBoolectorFieldBuilder::build(
        const std::vector<int32_t>  &path,
        bool                        is_fixed) {
    DEBUG_ENTER("build");
    m_node = 0;
    m_is_fixed = is_fixed;

    // Resolve to a field that we can visit
    dm::ITypeField *field = TaskPath2Field(m_root_field).toField(path);
    field->accept(m_this);

    DEBUG_LEAVE("build");
    return m_node;
}

void SolverBoolectorFieldBuilder::visitDataTypeBool(dm::IDataTypeBool *t) {
    DEBUG_ENTER("visitDataTypeBool");
    if (m_is_fixed) {
        // Create a single-bit constant
//        dm::ValRefBool val(m_field->getInit());
// TODO:
        dm::ValRefBool val;
        m_node = boolector_const(
            m_btor,
            val.get_val()?"1":"0");
    } else {
        // Create a single-bit variable
        m_node = boolector_var(m_btor, get_sort(1), 0);
    }

    DEBUG_LEAVE("visitDataTypeBool");
}

void SolverBoolectorFieldBuilder::visitDataTypeEnum(dm::IDataTypeEnum *t) {
    DEBUG_ENTER("visitDataTypeEnum");
    if (m_is_fixed) {

    } else {

    }
    DEBUG_LEAVE("visitDataTypeEnum");
}

void SolverBoolectorFieldBuilder::visitDataTypeInt(dm::IDataTypeInt *t) {
    DEBUG_ENTER("visitDataTypeInt");
    if (m_is_fixed) {
        if (t->width() <= 64) {
//            dm::ValRefInt val(m_field->getInit());
            // TODO:
            dm::ValRefInt val;
            char tmp[32];
            sprintf(tmp, "0x%llx", val.get_val_u());
            m_node = boolector_consth(
                m_btor,
                get_sort(t->width()),
                tmp);
        } else {
//            char *tmp = alloca(t->width()+1);
            // TODO:
        }
    } else {
        m_node = boolector_var(m_btor, get_sort(t->width()), 0);
    }
    DEBUG_LEAVE("visitDataTypeInt");
}

void SolverBoolectorFieldBuilder::visitTypeFieldPhy(dm::ITypeFieldPhy *f) {
    DEBUG_ENTER("visitTypeFieldPhy");
    m_field = f;
    f->getDataType()->accept(m_this);
    DEBUG_LEAVE("visitTypeFieldPhy");
}

struct BoolectorAnonymous *SolverBoolectorFieldBuilder::get_sort(int32_t width) {
    return boolector_bitvec_sort(m_btor, width);
};

dmgr::IDebug *SolverBoolectorFieldBuilder::m_dbg = 0;

}
}
