/**
 * SolverBoolectorFieldBuilder.h
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
#include <map>
#include <vector>
#include "dmgr/IDebugMgr.h"
#include "vsc/dm/IModelField.h"
#include "vsc/dm/impl/VisitorBase.h"

struct Btor;
struct BoolectorNode;
struct BoolectorAnonymous;

namespace vsc {
namespace solvers {



class SolverBoolectorFieldBuilder : public vsc::dm::VisitorBase {
public:
    SolverBoolectorFieldBuilder(
        dmgr::IDebugMgr         *dmgr,
        struct Btor             *btor,
        vsc::dm::IModelField    *root_field);

    virtual ~SolverBoolectorFieldBuilder();

    struct BoolectorNode *build(
        const std::vector<int32_t>  &path,
        bool                        is_fixed);

	virtual void visitDataTypeBool(dm::IDataTypeBool *t) override;

	virtual void visitDataTypeEnum(dm::IDataTypeEnum *t) override;

	virtual void visitDataTypeInt(dm::IDataTypeInt *t) override;

	virtual void visitTypeFieldPhy(dm::ITypeFieldPhy *f) override;

protected:
    struct BoolectorAnonymous *get_sort(int32_t width);


private:
    static dmgr::IDebug                             *m_dbg;
    struct Btor                                     *m_btor;
    vsc::dm::IModelField                            *m_root_field;
    bool                                            m_is_fixed;
    dm::ITypeFieldPhy                               *m_field;
    struct BoolectorNode                            *m_node;
    std::map<int32_t,struct BoolectorAnonymous *>   m_sort_m;

};

}
}


