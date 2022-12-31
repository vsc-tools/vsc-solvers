/*
 * SetFieldUsedRandVisitor.h
 *
 *  Created on: Oct 24, 2021
 *      Author: mballance
 */

#pragma once
#include <vector>
#include "vsc/dm/IContext.h"
#include "vsc/dm/impl/VisitorBase.h"

namespace vsc {
namespace solvers {


class SetFieldUsedRandVisitor : public dm::VisitorBase {
public:
	SetFieldUsedRandVisitor(dm::IContext *ctxt);

	virtual ~SetFieldUsedRandVisitor();

	void set(dm::IModelField *f);

	virtual void visitModelField(dm::IModelField *f) override;

private:
    static dmgr::IDebug     *m_dbg;
	std::vector<bool>		m_decl_rand_s;

};


}
}

