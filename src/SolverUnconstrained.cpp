/*
 * SolverUnconstrained.cpp
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
#include "vsc/dm/impl/ValRefBool.h"
#include "vsc/dm/impl/ValRefInt.h"
#include "vsc/dm/impl/ValRefStruct.h"
#include "vsc/solvers/impl/TaskPath2ValRef.h"
#include "SolverUnconstrained.h"


namespace vsc {
namespace solvers {


SolverUnconstrained::SolverUnconstrained(dmgr::IDebugMgr *dmgr) {
    DEBUG_INIT("vsc::solvers::SolverUnconstrained", dmgr);

}

SolverUnconstrained::~SolverUnconstrained() {

}

bool SolverUnconstrained::randomize(
        IRandState                              *randstate,
        const std::vector<dm::IModelFieldUP>    &root_fields,
        const RefPathSet                        &target_fields,
        const RefPathSet                        &fixed_fields) {
    DEBUG_ENTER("randomize");
    m_randstate = randstate;
    for (RefPathSet::iterator it=target_fields.begin(); it.next(); ) {
        m_it = it.path().begin();
        m_it_end = it.path().end();
        dm::IDataType *field_t = root_fields.at(*m_it)->getDataType();
        m_val = root_fields.at(*m_it)->getMutVal();

        m_it++;
        DEBUG("--> randomize field");
        field_t->accept(m_this);
        DEBUG("<-- randomize field");
    }
    DEBUG_LEAVE("randomize");

    return true;
}

void SolverUnconstrained::visitDataTypeBool(dm::IDataTypeBool *t) {
    dm::ValRefBool val_b(m_val);
    val_b.set_val(m_randstate->randint32(0, 1));
}

void SolverUnconstrained::visitDataTypeEnum(dm::IDataTypeEnum *t) {

}

void SolverUnconstrained::visitDataTypeInt(dm::IDataTypeInt *t) {
    dm::ValRefInt val_i(m_val);
    val_i.set_val(m_randstate->rand_ui64());
    /*
    if (t->getWidth() == 64) {
        dm::ValRefInt val_i(m_val);
        val_i.set_val(m_randstate->rand_ui64());
    } else if (t->getWidth() < 64) {
        dm::ValRefInt val_i(m_val);
    } else {
        // Greater than 64

    }
     */
}

void SolverUnconstrained::visitDataTypeStruct(dm::IDataTypeStruct *t) {
    dm::ValRefStruct val_s(m_val);

    m_val = val_s.getFieldRef(*m_it);
    dm::ITypeField *field = t->getField(*m_it);

    m_it++;

    field->accept(m_this);
}

dmgr::IDebug *SolverUnconstrained::m_dbg = 0;

}
}
