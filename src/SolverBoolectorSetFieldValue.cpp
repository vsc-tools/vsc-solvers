/*
 * SolverBoolectorSetFieldValue.cpp
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
#include "vsc/dm/impl/ValRefInt.h"
#include "vsc/solvers/impl/TaskPath2ValRef.h"
#include "vsc/solvers/impl/TaskPath2Field.h"
#include "SolverBoolectorSetFieldValue.h"


namespace vsc {
namespace solvers {


SolverBoolectorSetFieldValue::SolverBoolectorSetFieldValue(
    dmgr::IDebugMgr     *dmgr,
    Btor                *btor,
    dm::IModelField     *root_field) : 
    m_btor(btor), m_root_field(root_field) {
    DEBUG_INIT("vsc::solvers::SolverBoolectorSetFieldValue", dmgr);
}

SolverBoolectorSetFieldValue::~SolverBoolectorSetFieldValue() {

}

void SolverBoolectorSetFieldValue::set(
        const std::vector<int32_t> &path, 
        struct BoolectorNode       *node) {
    DEBUG_ENTER("set");
    m_node = node;
    dm::ITypeField *field = TaskPath2Field(m_root_field).toField(path);
    DEBUG("Field: %s", field->name().c_str());
    m_val = TaskPath2ValRef(m_root_field).toMutVal(path);
    field->getDataType()->accept(m_this);
    DEBUG_LEAVE("set");
}

void SolverBoolectorSetFieldValue::visitDataTypeBool(dm::IDataTypeBool *t) {
    DEBUG_ENTER("visitDataTypeBool");

    DEBUG_LEAVE("visitDataTypeBool");
}

void SolverBoolectorSetFieldValue::visitDataTypeEnum(dm::IDataTypeEnum *t) {
    DEBUG_ENTER("visitDataTypeEnum");

    DEBUG_LEAVE("visitDataTypeEnum");
}

void SolverBoolectorSetFieldValue::visitDataTypeInt(dm::IDataTypeInt *t) {
    DEBUG_ENTER("visitDataTypeInt");

    const char *bits = boolector_get_bits(
        m_btor, 
        boolector_get_value(m_btor, m_node));
    DEBUG("bits: %s\n", bits);
    if (t->width() <= 64) {
        uint64_t val = 0;
        dm::ValRefInt val_i(m_val);

        for (uint32_t i=0; i<t->width() && bits[i]; i++) {
            val <<= 1;
            val |= (bits[i] == '1');
        }

        val_i.set_val(val);
    } else {
        // TODO:
    }
    boolector_free_bits(m_btor, bits);

    DEBUG_LEAVE("visitDataTypeInt");
}

dmgr::IDebug *SolverBoolectorSetFieldValue::m_dbg = 0;

}
}
