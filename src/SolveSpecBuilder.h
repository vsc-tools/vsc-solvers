/*
 * SolveSpecBuilder.h
 *
 *  Created on: Oct 7, 2021
 *      Author: mballance
 */

#pragma once
#include <stdint.h>
#include <vector>
#include <unordered_map>
#include "vsc/dm/IContext.h"
#include "vsc/dm/impl/VisitorBase.h"
#include "SolveSet.h"
#include "SolveSpec.h"

namespace vsc {
namespace solvers {


class SolveSpecBuilder : public dm::VisitorBase {
public:
	SolveSpecBuilder(dm::IContext *ctx);

	virtual ~SolveSpecBuilder();

	SolveSpec *build(
			const std::vector<dm::IModelField *>		&fields,
			const std::vector<dm::IModelConstraint *>	&constraints
			);

	SolveSpec *build(
			const std::vector<dm::IModelField *>		&fields,
			const std::vector<dm::IModelConstraintUP>	&constraints
			);

	virtual void visitDataTypeEnum(dm::IDataTypeEnum *t) override;

	virtual void visitDataTypeInt(dm::IDataTypeInt *t) override;

	virtual void visitDataTypeStruct(dm::IDataTypeStruct *t) override;

	virtual void visitModelConstraintExpr(dm::IModelConstraintExpr *c) override;

	virtual void visitModelConstraintForeach(dm::IModelConstraintForeach *c) override;

	virtual void visitModelConstraintIfElse(dm::IModelConstraintIfElse *c) override;

	virtual void visitModelConstraintImplies(dm::IModelConstraintImplies *c) override;

//	virtual void visitModelConstraintSoft(dm::IModelConstraintSoft *c) override;

//	virtual void visitModelConstraintUnique(dm::IModelConstraintUnique *c) override;

	virtual void visitModelExprFieldRef(dm::IModelExprFieldRef *e) override;

	virtual void visitModelExprIndexedFieldRef(dm::IModelExprIndexedFieldRef *e) override;

	virtual void visitModelField(dm::IModelField *f) override;

private:

	void constraint_enter(dm::IModelConstraint *c);

	void constraint_leave(dm::IModelConstraint *c);

	void process_fieldref(dm::IModelField *f);

private:
    static dmgr::IDebug                             *m_dbg;
	dm::IContext									*m_ctx;
	uint32_t									    m_pass;
	SolveSetFlag								m_flags;
	std::vector<SolveSetUP>						m_solveset_l;
	std::unordered_map<SolveSet *, int32_t>		m_solveset_m;
	SolveSet									*m_active_solveset;
	std::unordered_map<dm::IModelField *,int32_t>	m_unconstrained_m;
	std::vector<dm::IModelField *>					m_unconstrained_l;

	std::unordered_map<dm::IModelField*,SolveSet*>	m_solveset_field_m;
	std::vector<dm::IModelConstraint *>				m_constraint_s;

	std::vector<dm::IModelField *>					m_field_s;


};

}
}

