/*
 * Copyright 2019-2021 Matthew Ballance and contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SetFieldUsedRandVisitor.cpp
 *
 *  Created on: Oct 24, 2021
 *      Author: mballance
 */

#include "dmgr/impl/DebugMacros.h"
#include "SetFieldUsedRandVisitor.h"

namespace vsc {
namespace solvers {


SetFieldUsedRandVisitor::SetFieldUsedRandVisitor(dm::IContext *ctxt) {
    DEBUG_INIT("SetFieldUsedRandVisitor", ctxt->getDebugMgr());
}

SetFieldUsedRandVisitor::~SetFieldUsedRandVisitor() {
	// TODO Auto-generated destructor stub
}

void SetFieldUsedRandVisitor::set(dm::IModelField *f) {
	DEBUG_ENTER("set %s", f->name().c_str());
	m_decl_rand_s.clear();
	f->accept(this);
	DEBUG_LEAVE("set %s", f->name().c_str());
}

void SetFieldUsedRandVisitor::visitModelField(dm::IModelField *f) {
	DEBUG_ENTER("visitModelField %s flags=0x%08x",
			f->name().c_str(), f->flags());
	if ((m_decl_rand_s.size() == 0 || m_decl_rand_s.back()) &&
			f->isFlagSet(dm::ModelFieldFlag::DeclRand)) {
		f->setFlag(dm::ModelFieldFlag::UsedRand);
	}

	m_decl_rand_s.push_back(
			(m_decl_rand_s.size() == 0 ||
				f->isFlagSet(dm::ModelFieldFlag::DeclRand)));
	VisitorBase::visitModelField(f);
	m_decl_rand_s.pop_back();

	DEBUG_LEAVE("visitModelField %s flags=0x%08x",
			f->name().c_str(), f->flags());
}

dmgr::IDebug *SetFieldUsedRandVisitor::m_dbg;

}
}
