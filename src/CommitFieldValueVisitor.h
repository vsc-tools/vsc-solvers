/*
 * CommitFieldValueVisitor.h
 *
 *  Created on: Oct 24, 2021
 *      Author: mballance
 */

#pragma once
#include <vector>
#include "vsc/solvers/ISolver.h"
#include "vsc/dm/impl/VisitorBase.h"

namespace vsc {
namespace solvers {


class CommitFieldValueVisitor : public VisitorBase {
public:
	CommitFieldValueVisitor(ISolver *solver);

	virtual ~CommitFieldValueVisitor();

	void commit(IModelField *f);

	virtual void visitDataTypeInt(IDataTypeInt *t) override;

	virtual void visitModelField(IModelField *f) override;

private:
	ISolver									*m_solver;
	std::vector<IModelField *>				m_field_s;

};

}
}
}

