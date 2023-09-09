/**
 * SolverUnconstrained.h
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
#include "dmgr/IDebugMgr.h"
#include "vsc/dm/impl/VisitorBase.h"
#include "vsc/solvers/IRandState.h"
#include "vsc/solvers/impl/RefPathSet.h"

namespace vsc {
namespace solvers {



class SolverUnconstrained : public virtual dm::VisitorBase {
public:
    SolverUnconstrained(dmgr::IDebugMgr *dmgr);

    virtual ~SolverUnconstrained();

    bool randomize(
        IRandState                              *randstate,
        dm::IModelField                         *root_field,
        const RefPathSet                        &target_fields);

	virtual void visitDataTypeBool(dm::IDataTypeBool *t) override;

	virtual void visitDataTypeEnum(dm::IDataTypeEnum *t) override;

	virtual void visitDataTypeInt(dm::IDataTypeInt *t) override;

	virtual void visitDataTypeStruct(dm::IDataTypeStruct *t) override;

private:
    static dmgr::IDebug                         *m_dbg;
    IRandState                                  *m_randstate;
    std::vector<int32_t>::const_iterator        m_it;
    std::vector<int32_t>::const_iterator        m_it_end;
    dm::ValRef                                  m_val;

};

}
}


