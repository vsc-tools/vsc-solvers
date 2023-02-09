/*
 * TestBase.cpp
 *
 * Copyright 2022 Matthew Ballance and Contributors
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
#include "dmgr/FactoryExt.h"
#include "vsc/dm/FactoryExt.h"
#include "vsc/solvers/FactoryExt.h"
#include "TestBase.h"


namespace vsc {
namespace solvers {


TestBase::TestBase() {

}

TestBase::~TestBase() {

}

void TestBase::SetUp() {
    dmgr::IDebugMgr *dmgr = dmgr_getFactory()->getDebugMgr();

    m_factory = vsc_solvers_getFactory();
    m_factory->init(dmgr);

    vsc::dm::IFactory *dm_f = vsc_dm_getFactory();
    dm_f->init(dmgr);

    m_ctxt = vsc::dm::IContextUP(dm_f->mkContext());
}

void TestBase::TearDown() {

}

void TestBase::enableDebug(bool en) {
    dmgr::IDebugMgr *dmgr = dmgr_getFactory()->getDebugMgr();
    dmgr->enable(en);
}

}
}
