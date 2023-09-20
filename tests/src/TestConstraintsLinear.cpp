/*
 * TestConstraintsLinear.cpp
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
#include "TestConstraintsLinear.h"


namespace vsc {
namespace solvers {


TestConstraintsLinear::TestConstraintsLinear() {

}

TestConstraintsLinear::~TestConstraintsLinear() {

}

TEST_F(TestConstraintsLinear, ult_2var) {
    VSC_DATACLASSES(TestConstraintsLinear_ult_2var, MyC, R"(
        @vdc.randclass
        class MyC(object):
            a : vdc.rand_uint32_t 
            b : vdc.rand_uint32_t 

            @vdc.constraint
            def ab_c(self):
                self.a < 15
                self.b < 15
                self.a < self.b
    )");
    #include "TestConstraintsLinear_ult_2var.h"
    enableDebug(false);

    vsc::dm::IModelFieldUP field(mkRootField("abc", MyC_t));

    IRandStateUP randstate(m_factory->mkRandState("0"));
    ICompoundSolverUP solver(m_factory->mkCompoundSolver());
    RefPathSet target_fields, fixed_fields, include_constraints, exclude_constraints;
    SolveFlags flags = SolveFlags::NoFlags;

    for (uint32_t i=0; i<3000; i++) {
        solver->randomize(
            randstate.get(),
            field.get(),
            target_fields,
            fixed_fields,
            include_constraints,
            exclude_constraints,
            flags);
        dm::ValRefStruct field_v(field->getImmVal());
        dm::ValRefInt val_a(field_v.getFieldRef(0));
        dm::ValRefInt val_b(field_v.getFieldRef(1));
        ASSERT_LT(val_a.get_val_u(), val_b.get_val_u());
    }
}

TEST_F(TestConstraintsLinear, ult_2var_8bit) {
    VSC_DATACLASSES(TestConstraintsLinear_ult_2var_8bit, MyC, R"(
        @vdc.randclass
        class MyC(object):
            a : vdc.rand_uint8_t 
            b : vdc.rand_uint8_t 

            @vdc.constraint
            def ab_c(self):
                self.a < 15
                self.b < 15
                self.a < self.b
    )");
    #include "TestConstraintsLinear_ult_2var_8bit.h"
    enableDebug(false);

    vsc::dm::IModelFieldUP field(mkRootField("abc", MyC_t));

    IRandStateUP randstate(m_factory->mkRandState("0"));
    ICompoundSolverUP solver(m_factory->mkCompoundSolver());
    RefPathSet target_fields, fixed_fields, include_constraints, exclude_constraints;
    SolveFlags flags = SolveFlags::NoFlags;

    for (uint32_t i=0; i<3000; i++) {
        solver->randomize(
            randstate.get(),
            field.get(),
            target_fields,
            fixed_fields,
            include_constraints,
            exclude_constraints,
            flags);
        dm::ValRefStruct field_v(field->getImmVal());
        dm::ValRefInt val_a(field_v.getFieldRef(0));
        dm::ValRefInt val_b(field_v.getFieldRef(1));
        ASSERT_LT(val_a.get_val_u(), val_b.get_val_u());
    }
}

TEST_F(TestConstraintsLinear, struct_32bit_ne) {
    VSC_DATACLASSES(TestConstraintsLinear_struct_32bit_ne, MyC, R"(
        @vdc.randclass
        class MyC(object):
            a : vdc.rand_uint32_t
            b : vdc.rand_uint32_t
            c : vdc.rand_uint32_t
            d : vdc.rand_uint32_t

            @vdc.constraint
            def ab_c(self):
                self.a != self.b
                self.a != self.c
                self.b != self.c
    )");
    #include "TestConstraintsLinear_struct_32bit_ne.h"
    enableDebug(true);

    fprintf(stdout, "MyC_t.name: %s\n", MyC_t->name().c_str());
    fflush(stdout);
    ASSERT_EQ(MyC_t->name(), "MyC");

    vsc::dm::IModelFieldUP field(mkRootField("abc", MyC_t));

    IRandStateUP randstate(m_factory->mkRandState("0"));
    ICompoundSolverUP solver(m_factory->mkCompoundSolver());
    RefPathSet target_fields, fixed_fields, include_constraints, exclude_constraints;
    SolveFlags flags = SolveFlags::NoFlags;

    for (uint32_t i=0; i<1; i++) {
        solver->randomize(
            randstate.get(),
            field.get(),
            target_fields,
            fixed_fields,
            include_constraints,
            exclude_constraints,
            flags);
        /*
        dm::ValRefStruct field_v(field->getImmVal());
        dm::ValRefInt val_a(field_v.getField(0));
        dm::ValRefInt val_b(field_v.getField(1));
 //       ASSERT_LT(val_a.get_val_u(), val_b.get_val_u());
         */
    }
}

TEST_F(TestConstraintsLinear, nested_struct_32bit_ne) {
    VSC_DATACLASSES(TestConstraintsLinear_nested_struct_32bit_ne, MyC, R"(
        @vdc.randclass
        class MyI(object):
            a : vdc.rand_uint32_t
            b : vdc.rand_uint32_t
            c : vdc.rand_uint32_t
            d : vdc.rand_uint32_t

            @vdc.constraint
            def ab_c(self):
                self.a != self.b
                self.a != self.c
                self.b != self.c
        
        @vdc.randclass
        class MyC(object):
            a : vdc.rand[MyI]
            b : vdc.rand[MyI]
            c : vdc.rand[MyI]
            d : vdc.rand[MyI]
    )");
    #include "TestConstraintsLinear_nested_struct_32bit_ne.h"
    enableDebug(false);

    fprintf(stdout, "MyC_t.name: %s\n", MyC_t->name().c_str());
    fflush(stdout);
    ASSERT_EQ(MyC_t->name(), "MyC");

    vsc::dm::IModelFieldUP field(mkRootField("abc", MyC_t));

    IRandStateUP randstate(m_factory->mkRandState("0"));
    ICompoundSolverUP solver(m_factory->mkCompoundSolver());
    RefPathSet target_fields, fixed_fields, include_constraints, exclude_constraints;
    SolveFlags flags = SolveFlags::NoFlags;

    for (uint32_t i=0; i<1000; i++) {
        solver->randomize(
            randstate.get(),
            field.get(),
            target_fields,
            fixed_fields,
            include_constraints,
            exclude_constraints,
            flags);
        /*
        dm::ValRefStruct field_v(field->getImmVal());
        dm::ValRefInt val_a(field_v.getField(0));
        dm::ValRefInt val_b(field_v.getField(1));
 //       ASSERT_LT(val_a.get_val_u(), val_b.get_val_u());
         */
    }
}

TEST_F(TestConstraintsLinear, nested_struct_32bit_ne_single) {
    VSC_DATACLASSES(TestConstraintsLinear_nested_struct_32bit_ne_single, MyC, R"(
        @vdc.randclass
        class MyI(object):
            a : vdc.rand_uint32_t
            b : vdc.rand_uint32_t
            c : vdc.rand_uint32_t
            d : vdc.rand_uint32_t

            @vdc.constraint
            def ab_c(self):
                self.a != self.b
                self.a != self.c
                self.b != self.c
        
        @vdc.randclass
        class MyC(object):
            a : vdc.rand[MyI]
    )");
    #include "TestConstraintsLinear_nested_struct_32bit_ne_single.h"
    enableDebug(false);

    fprintf(stdout, "MyC_t.name: %s\n", MyC_t->name().c_str());
    fflush(stdout);
    ASSERT_EQ(MyC_t->name(), "MyC");

    vsc::dm::IModelFieldUP field(mkRootField("abc", MyC_t));

    IRandStateUP randstate(m_factory->mkRandState("0"));
    ICompoundSolverUP solver(m_factory->mkCompoundSolver());
    RefPathSet target_fields, fixed_fields, include_constraints, exclude_constraints;
    SolveFlags flags = SolveFlags::NoFlags;

    for (uint32_t i=0; i<1000; i++) {
        solver->randomize(
            randstate.get(),
            field.get(),
            target_fields,
            fixed_fields,
            include_constraints,
            exclude_constraints,
            flags);
        /*
        dm::ValRefStruct field_v(field->getImmVal());
        dm::ValRefInt val_a(field_v.getField(0));
        dm::ValRefInt val_b(field_v.getField(1));
 //       ASSERT_LT(val_a.get_val_u(), val_b.get_val_u());
         */
    }
}


}
}
