
#include "TestModelFieldFactory.h"
#include "vsc/dm/impl/TaskBuildModelField.h"
#include "vsc/dm/IContext.h"
#include "vsc/dm/impl/ModelBuildContext.h"

using namespace vsc::dm;

namespace vsc {
namespace solvers {

TEST_F(TestModelFieldFactory, smoke) {
    enableDebug(true);

    IDataTypeStruct *my_t = m_ctxt->mkDataTypeStruct("my_t");
    IDataTypeInt *ui8_t = m_ctxt->mkDataTypeInt(false, 8);
    m_ctxt->addDataTypeInt(ui8_t);

    my_t->addField(m_ctxt->mkTypeFieldPhy("a", ui8_t, false, TypeFieldAttr::Rand, 0), true);
    my_t->addField(m_ctxt->mkTypeFieldPhy("b", ui8_t, false, TypeFieldAttr::Rand, 0), true);

    ITypeConstraintBlock *c = m_ctxt->mkTypeConstraintBlock("ab_c");
    c->addConstraint(m_ctxt->mkTypeConstraintExpr(
        m_ctxt->mkTypeExprBin(
            m_ctxt->mkTypeExprFieldRef(
                ITypeExprFieldRef::RootRefKind::TopDownScope,
                0,
                {0}), // a
            BinOp::Lt,
            m_ctxt->mkTypeExprFieldRef(
                ITypeExprFieldRef::RootRefKind::TopDownScope,
                0,
                {1})  // b
        )), true);
    my_t->addConstraint(c, true);

    m_ctxt->addDataTypeStruct(my_t);

    ModelBuildContext build_ctxt(m_ctxt.get());

    IModelFieldUP field(my_t->mkRootField(&build_ctxt, "abc", false));

    ASSERT_EQ(field->getFields().size(), 2);
    ASSERT_EQ(field->getField(0)->name(), "a");
    ASSERT_EQ(field->getField(1)->name(), "b");

    IRandStateUP state(m_factory->mkRandState("0"));
    ICompoundSolverUP solver(m_factory->mkCompoundSolver(m_ctxt.get()));

    for (uint32_t i=0; i<10; i++) {
        ASSERT_TRUE(solver->solve(
            state.get(),
            {field.get()},
            {},
            SolveFlags::RandomizeTopFields|SolveFlags::Randomize
        ));
        ASSERT_LT(
            field->getField(0)->val()->val_u(),
            field->getField(1)->val()->val_u());
    }
}

TEST_F(TestModelFieldFactory, struct_type_subfields) {
    IDataTypeStruct *my_t = m_ctxt->mkDataTypeStruct("my_t");
    IDataTypeInt *ui8_t = m_ctxt->mkDataTypeInt(false, 8);
    m_ctxt->addDataTypeInt(ui8_t);


    IDataTypeStruct *my_subfield_t = m_ctxt->mkDataTypeStruct("my_subfield_t");
    my_subfield_t->addField(m_ctxt->mkTypeFieldPhy("a", ui8_t, false, TypeFieldAttr::Rand, 0), true);
    my_subfield_t->addField(m_ctxt->mkTypeFieldPhy("b", ui8_t, false, TypeFieldAttr::Rand, 0), true);

    ITypeConstraintBlock *c = m_ctxt->mkTypeConstraintBlock("ab_c");
    c->addConstraint(m_ctxt->mkTypeConstraintExpr(
        m_ctxt->mkTypeExprBin(
            m_ctxt->mkTypeExprFieldRef(
                ITypeExprFieldRef::RootRefKind::TopDownScope,
                0,
                {0}), // a
            BinOp::Lt,
            m_ctxt->mkTypeExprFieldRef(
                ITypeExprFieldRef::RootRefKind::TopDownScope,
                0,
                {1}) // b
        )), true);
    my_subfield_t->addConstraint(c, true);

    m_ctxt->addDataTypeStruct(my_subfield_t);

    my_t->addField(m_ctxt->mkTypeFieldPhy("a", my_subfield_t, false, TypeFieldAttr::Rand, 0), true);
    my_t->addField(m_ctxt->mkTypeFieldPhy("b", my_subfield_t, false, TypeFieldAttr::Rand, 0), true);
    m_ctxt->addDataTypeStruct(my_t);

    ModelBuildContext build_ctxt(m_ctxt.get());

    IModelFieldUP field(my_t->mkRootField(&build_ctxt, "abc", false));

    ASSERT_EQ(field->getFields().size(), 2);
    ASSERT_EQ(field->getField(0)->name(), "a");
    ASSERT_EQ(field->getField(1)->name(), "b");

    IRandStateUP state(m_factory->mkRandState("0"));
    ICompoundSolverUP solver(m_factory->mkCompoundSolver(m_ctxt.get()));

    for (uint32_t i=0; i<10; i++) {
        ASSERT_TRUE(solver->solve(
            state.get(),
            {field.get()},
            {},
            SolveFlags::RandomizeTopFields|SolveFlags::Randomize
        ));
        fprintf(stdout, "a.a=%lld a.b=%lld b.a=%lld b.b=%lld\n",
            field->getField(0)->getField(0)->val()->val_u(),
            field->getField(0)->getField(1)->val()->val_u(),
            field->getField(1)->getField(0)->val()->val_u(),
            field->getField(1)->getField(1)->val()->val_u());
        ASSERT_LT(
            field->getField(0)->getField(0)->val()->val_u(),
            field->getField(0)->getField(1)->val()->val_u());
        ASSERT_LT(
            field->getField(1)->getField(0)->val()->val_u(),
            field->getField(1)->getField(1)->val()->val_u());
    }
}

TEST_F(TestModelFieldFactory, struct_type_subfields_above_constraint) {
    IDataTypeStruct *my_t = m_ctxt->mkDataTypeStruct("my_t");
    IDataTypeInt *ui8_t = m_ctxt->mkDataTypeInt(false, 8);
    m_ctxt->addDataTypeInt(ui8_t);


    IDataTypeStruct *my_subfield_t = m_ctxt->mkDataTypeStruct("my_subfield_t");
    my_subfield_t->addField(m_ctxt->mkTypeFieldPhy("a", ui8_t, false, TypeFieldAttr::Rand, 0), true);
    my_subfield_t->addField(m_ctxt->mkTypeFieldPhy("b", ui8_t, false, TypeFieldAttr::Rand, 0), true);

    ITypeConstraintBlock *c = m_ctxt->mkTypeConstraintBlock("ab_c");
    c->addConstraint(m_ctxt->mkTypeConstraintExpr(
        m_ctxt->mkTypeExprBin(
            m_ctxt->mkTypeExprFieldRef(
                ITypeExprFieldRef::RootRefKind::TopDownScope, 0, {0}), // a
            BinOp::Lt,
            m_ctxt->mkTypeExprFieldRef(
                ITypeExprFieldRef::RootRefKind::TopDownScope, 0, {1}) // b
        )), true);
    my_subfield_t->addConstraint(c, true);

    m_ctxt->addDataTypeStruct(my_subfield_t);

    my_t->addField(m_ctxt->mkTypeFieldPhy("a", my_subfield_t, false, TypeFieldAttr::Rand, 0), true);
    my_t->addField(m_ctxt->mkTypeFieldPhy("b", my_subfield_t, false, TypeFieldAttr::Rand, 0), true);

    my_t->addConstraint(m_ctxt->mkTypeConstraintExpr(
        m_ctxt->mkTypeExprBin(
            m_ctxt->mkTypeExprFieldRef(
                ITypeExprFieldRef::RootRefKind::TopDownScope, 0, {0, 0}), // a.a
            BinOp::Eq,
            m_ctxt->mkTypeExprFieldRef(
                ITypeExprFieldRef::RootRefKind::TopDownScope, 0, {0, 1})  // b.a
            )), true);

    m_ctxt->addDataTypeStruct(my_t);

    ModelBuildContext build_ctxt(m_ctxt.get());

    IModelFieldUP field(my_t->mkRootField(&build_ctxt, "abc", false));

    ASSERT_EQ(field->getFields().size(), 2);
    ASSERT_EQ(field->getField(0)->name(), "a");
    ASSERT_EQ(field->getField(1)->name(), "b");

    IRandStateUP state(m_factory->mkRandState("0"));
    ICompoundSolverUP solver(m_factory->mkCompoundSolver(m_ctxt.get()));

    for (uint32_t i=0; i<10; i++) {
        ASSERT_TRUE(solver->solve(
            state.get(),
            {field.get()},
            {},
            SolveFlags::RandomizeTopFields|SolveFlags::Randomize
        ));
        fprintf(stdout, "a.a=%lld a.b=%lld b.a=%lld b.b=%lld\n",
            field->getField(0)->getField(0)->val()->val_u(),
            field->getField(0)->getField(1)->val()->val_u(),
            field->getField(1)->getField(0)->val()->val_u(),
            field->getField(1)->getField(1)->val()->val_u());
        ASSERT_LT(
            field->getField(0)->getField(0)->val()->val_u(),
            field->getField(0)->getField(1)->val()->val_u());
        ASSERT_LT(
            field->getField(1)->getField(0)->val()->val_u(),
            field->getField(1)->getField(1)->val()->val_u());
        ASSERT_EQ(
            field->getField(0)->getField(0)->val()->val_u(),
            field->getField(1)->getField(0)->val()->val_u());
    }
}

}
}