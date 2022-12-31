/*
 * SolverBoolectorModelBuilder.h
 *
 *  Created on: Sep 26, 2021
 *      Author: mballance
 */

#include <vector>
#include "SolverBoolector.h"
#include "dmgr/IDebugMgr.h"
#include "vsc/dm/impl/VisitorBase.h"

namespace vsc {
namespace solvers {


class SolverBoolectorSolveModelBuilder : public dm::VisitorBase {
public:
	SolverBoolectorSolveModelBuilder(
        dmgr::IDebugMgr     *dmgr,
        SolverBoolector     *solver);

	virtual ~SolverBoolectorSolveModelBuilder();

	BoolectorNode *build(dm::IModelField *f);

	BoolectorNode *build(dm::IModelConstraint *c);

	virtual void visitDataTypeEnum(dm::IDataTypeEnum *t) override;

	virtual void visitDataTypeInt(dm::IDataTypeInt *t) override;

	virtual void visitDataTypeStruct(dm::IDataTypeStruct *t) override;

	virtual void visitModelConstraint(dm::IModelConstraint *c) override;

	virtual void visitModelConstraintExpr(dm::IModelConstraintExpr *c) override;

	virtual void visitModelConstraintIfElse(dm::IModelConstraintIfElse *c) override;

	virtual void visitModelConstraintImplies(dm::IModelConstraintImplies *c) override;

	virtual void visitModelConstraintScope(dm::IModelConstraintScope *c) override;

	virtual void visitModelConstraintSoft(dm::IModelConstraintSoft *c) override;

	virtual void visitModelExprBin(dm::IModelExprBin *e) override;

	virtual void visitModelExprFieldRef(dm::IModelExprFieldRef *e) override;

	virtual void visitModelExprIn(dm::IModelExprIn *e) override;

	virtual void visitModelExprIndexedFieldRef(dm::IModelExprIndexedFieldRef *e) override;

	virtual void visitModelExprPartSelect(dm::IModelExprPartSelect *e) override;

	virtual void visitModelExprVal(dm::IModelExprVal *e) override;

	virtual void visitModelField(dm::IModelField *f) override;

	virtual void visitModelFieldRoot(dm::IModelFieldRoot *f) override;

	virtual void visitModelFieldType(dm::IModelFieldType *f) override;

private:

	using node_info_t=std::pair<bool, BoolectorNode *>;

private:

	BoolectorNode *toBoolNode(BoolectorNode *n);

	node_info_t expr(dm::IModelExpr *e, int32_t ctx_width);

	BoolectorNode *extend(
			BoolectorNode		*n,
			int32_t				ctx_width,
			bool				is_signed);


private:
    static dmgr::IDebug                 *m_dbg;
	SolverBoolector						*m_solver;
	std::vector<dm::IModelField *>		m_field_s;
	std::pair<bool,BoolectorNode *>		m_node_i;
	std::vector<int32_t>				m_width_s;
	bool								m_build_field;
};

}
}

