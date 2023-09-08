/*
 * TestUnconstrainedRandomization.cpp
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
#include "TestUnconstrainedRandomization.h"


namespace vsc {
namespace solvers {


TestUnconstrainedRandomization::TestUnconstrainedRandomization() {

}

TestUnconstrainedRandomization::~TestUnconstrainedRandomization() {

}

TEST_F(TestUnconstrainedRandomization, smoke) {
    VSC_DATACLASSES(TestUnconstrainedRandomization_smoke, MyC, R"(
        @vdc.randclass
        class MyC(object):
            a : vdc.rand_uint32_t 
            b : vdc.rand_uint32_t 
            c : vdc.rand_uint32_t 
    )");
    #include "TestUnconstrainedRandomization_smoke.h"

    ASSERT_TRUE(m_ctxt->findDataTypeStruct("MyC") != 0);
    ASSERT_EQ(m_ctxt->findDataTypeStruct("MyC"), MyC_t);
    vsc::dm::IModelField *field = mkRootField("abc", MyC_t);
    uint32_t count = 1000000;
    std::vector<vsc::dm::IModelField *> fields(count);
    for (uint32_t i=0; i<count; i++) {
        fields.push_back(mkRootField("abc", MyC_t));
    }
    ASSERT_TRUE(field);
}

TEST_F(TestUnconstrainedRandomization, smoke2) {
    VSC_DATACLASSES(TestUnconstrainedRandomization_smoke2, MyC, R"(
        @vdc.randclass
        class MyC(object):
            a : vdc.rand_uint32_t 
            b : vdc.rand_uint32_t 
            c : vdc.rand_uint32_t 
            d : vdc.rand_uint32_t 
            a1 : vdc.rand_uint32_t 
            b1 : vdc.rand_uint32_t 
            c1 : vdc.rand_uint32_t 
            d1 : vdc.rand_uint32_t 
            a2 : vdc.rand_uint32_t 
            b2 : vdc.rand_uint32_t 
            c2 : vdc.rand_uint32_t 
            d2 : vdc.rand_uint32_t 
            a3 : vdc.rand_uint32_t 
            b3 : vdc.rand_uint32_t 
            c3 : vdc.rand_uint32_t 
            d3 : vdc.rand_uint32_t 
    )");
    #include "TestUnconstrainedRandomization_smoke2.h"
    enableDebug(false);

    ASSERT_EQ(MyC_t->getFields().size(), 16);

    ASSERT_TRUE(m_ctxt->findDataTypeStruct("MyC") != 0);
    ASSERT_EQ(m_ctxt->findDataTypeStruct("MyC"), MyC_t);
    vsc::dm::IModelField *field = mkRootField("abc", MyC_t);
    ASSERT_TRUE(field);

    IRandStateUP randstate(m_factory->mkRandState("0"));
    ICompoundSolverUP solver(m_factory->mkCompoundSolver());
    RefPathSet target_fields, fixed_fields, include_constraints, exclude_constraints;
    SolveFlags flags = SolveFlags::NoFlags;

    std::vector<vsc::dm::IModelFieldUP> root_fields;
    root_fields.push_back(vsc::dm::IModelFieldUP(field, false));
    target_fields.add({0});

    ASSERT_TRUE(field->getDataType());
    ASSERT_TRUE(dynamic_cast<dm::IDataTypeStruct *>(field->getDataType()));
    ASSERT_EQ(field->getDataType(), MyC_t);
    ASSERT_EQ(dynamic_cast<dm::IDataTypeStruct *>(field->getDataType())->getFields().size(), 16);

    for (uint32_t i=0; i<10000; i++) {
        solver->randomize(
            randstate.get(),
            root_fields,
            target_fields,
            fixed_fields,
            include_constraints,
            exclude_constraints,
            flags);
    }
}

}
}
