/**
 * SolverBoolector.h
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
#include "vsc/solvers/ISolver.h"
#include "vsc/solvers/impl/RefPathPtrMap.h"

struct Btor;
struct BoolectorNode;

namespace vsc {
namespace solvers {



class SolverBoolector : public virtual ISolver {
public:
    SolverBoolector(dmgr::IDebugMgr *dmgr);

    virtual ~SolverBoolector();

    virtual bool randomize(
        IRandState                              *randstate,
        dm::IModelField                         *root_field,
        ISolveSet                               *solveset) override;

private:
    static dmgr::IDebug                     *m_dbg;
    dmgr::IDebugMgr                         *m_dmgr;
    struct Btor                             *m_btor;
    bool                                    m_issat;
    RefPathPtrMap<struct BoolectorNode>     m_field_m;

};

}
}


