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


class CommitFieldValueVisitor : public dm::VisitorBase {
public:
	CommitFieldValueVisitor(ISolver *solver);

	virtual ~CommitFieldValueVisitor();

	void commit(dm::IModelField *f);

	virtual void visitDataTypeInt(dm::IDataTypeInt *t) override;

	virtual void visitModelField(dm::IModelField *f) override;

private:
	ISolver							*m_solver;
	std::vector<dm::IModelField *>				m_field_s;

};

}
}

