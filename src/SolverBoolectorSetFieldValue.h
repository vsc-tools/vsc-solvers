/**
 * SolverBoolectorSetFieldValue.h
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
#include "vsc/dm/impl/ValRef.h"

struct BoolectorNode;

namespace vsc {
namespace solvers {



class SolverBoolectorSetFieldValue : public virtual dm::VisitorBase {
public:
    SolverBoolectorSetFieldValue(
        dmgr::IDebugMgr     *dmgr,
        struct Btor         *btor,
        dm::IModelField     *root_field);

    virtual ~SolverBoolectorSetFieldValue();

    void set(
        const std::vector<int32_t> &path, 
        struct BoolectorNode       *node);

	virtual void visitDataTypeBool(dm::IDataTypeBool *t) override;

	virtual void visitDataTypeEnum(dm::IDataTypeEnum *t) override;

	virtual void visitDataTypeInt(dm::IDataTypeInt *t) override;

private:
    static dmgr::IDebug             *m_dbg;
    struct Btor                     *m_btor;
    dm::IModelField                 *m_root_field;
    struct BoolectorNode            *m_node;
    dm::ValRef                      m_val;

};

}
}


