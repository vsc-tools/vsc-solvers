/*
 * TestBuildSolveSets.cpp
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
#include "TestBuildSolveSets.h"
#include "TaskBuildSolveSets.h"


namespace vsc {
namespace solvers {


TestBuildSolveSets::TestBuildSolveSets() {

}

TestBuildSolveSets::~TestBuildSolveSets() {

}

TEST_F(TestBuildSolveSets, smoke) {
    VSC_DATACLASSES(TestBuildSolveSets_smoke, MyC, R"(
        @vdc.randclass
        class MyC(object):
            a : vdc.rand_uint32_t 
            b : vdc.rand_uint32_t 

            @vdc.constraint
            def ab_c(self):
                self.a < self.b
    )");
    #include "TestBuildSolveSets_smoke.h"

    enableDebug(true);
    RefPathSet target_fields, fixed_fields, include_constraints, exclude_constraints;
    RefPathSet unconstrained;

    vsc::dm::IModelFieldUP field(mkRootField("abc", MyC_t));

    std::vector<ISolveSetUP> solvesets;

    TaskBuildSolveSets(
        m_factory->getDebugMgr(),
        field.get(),
        target_fields,
        fixed_fields,
        include_constraints,
        exclude_constraints).build(solvesets, unconstrained);
    
    ASSERT_EQ(solvesets.size(), 1);
    ASSERT_EQ(solvesets.at(0)->getFields().size(), 2);
    ASSERT_EQ(solvesets.at(0)->getConstraints().size(), 1);
    ASSERT_EQ(unconstrained.size(), 0);

}

TEST_F(TestBuildSolveSets, three_related_vars) {
    VSC_DATACLASSES(TestBuildSolveSets_three_related_vars, MyC, R"(
        @vdc.randclass
        class MyC(object):
            a : vdc.rand_uint32_t 
            b : vdc.rand_uint32_t 
            c : vdc.rand_uint32_t 

            @vdc.constraint
            def ab_c(self):
                self.a < self.b
                self.b < self.c
    )");
    #include "TestBuildSolveSets_three_related_vars.h"

    enableDebug(true);
    RefPathSet target_fields, fixed_fields, include_constraints, exclude_constraints;
    RefPathSet unconstrained;

    vsc::dm::IModelFieldUP field(mkRootField("abc", MyC_t));

    std::vector<ISolveSetUP> solvesets;

    TaskBuildSolveSets(
        m_factory->getDebugMgr(),
        field.get(),
        target_fields,
        fixed_fields,
        include_constraints,
        exclude_constraints).build(solvesets, unconstrained);
    
    ASSERT_EQ(solvesets.size(), 1);
    ASSERT_EQ(solvesets.at(0)->getFields().size(), 3);
    ASSERT_EQ(solvesets.at(0)->getConstraints().size(), 2);
    ASSERT_EQ(unconstrained.size(), 0);
}

TEST_F(TestBuildSolveSets, three_related_vars_one_unconstrained) {
    VSC_DATACLASSES(TestBuildSolveSets_three_related_vars_one_unconstrained, MyC, R"(
        @vdc.randclass
        class MyC(object):
            a : vdc.rand_uint32_t 
            b : vdc.rand_uint32_t 
            c : vdc.rand_uint32_t 
            d : vdc.rand_uint32_t 

            @vdc.constraint
            def ab_c(self):
                self.a < self.b
                self.b < self.c
    )");
    #include "TestBuildSolveSets_three_related_vars_one_unconstrained.h"

    enableDebug(true);
    RefPathSet target_fields, fixed_fields, include_constraints, exclude_constraints;
    RefPathSet unconstrained;

    vsc::dm::IModelFieldUP field(mkRootField("abc", MyC_t));

    std::vector<ISolveSetUP> solvesets;

    TaskBuildSolveSets(
        m_factory->getDebugMgr(),
        field.get(),
        target_fields,
        fixed_fields,
        include_constraints,
        exclude_constraints).build(solvesets, unconstrained);
    
    ASSERT_EQ(solvesets.size(), 1);
    ASSERT_EQ(solvesets.at(0)->getFields().size(), 3);
    ASSERT_EQ(solvesets.at(0)->getConstraints().size(), 2);
    ASSERT_EQ(unconstrained.size(), 1);
}

TEST_F(TestBuildSolveSets, nested_struct_local_constraints) {
    VSC_DATACLASSES(TestBuildSolveSets_nested_struct_local_constraints, MyC, R"(
        @vdc.randclass
        class MyI(object):
            a : vdc.rand_uint32_t 
            b : vdc.rand_uint32_t 

            @vdc.constraint
            def ab_c(self):
                self.a < self.b

        @vdc.randclass
        class MyC(object):
            a : vdc.rand[MyI]

    )");
    #include "TestBuildSolveSets_nested_struct_local_constraints.h"

    enableDebug(true);
    RefPathSet target_fields, fixed_fields, include_constraints, exclude_constraints;
    RefPathSet unconstrained;

    vsc::dm::IModelFieldUP field(mkRootField("abc", MyC_t));

    std::vector<ISolveSetUP> solvesets;

    TaskBuildSolveSets(
        m_factory->getDebugMgr(),
        field.get(),
        target_fields,
        fixed_fields,
        include_constraints,
        exclude_constraints).build(solvesets, unconstrained);
    
    ASSERT_EQ(solvesets.size(), 1);
    ASSERT_EQ(solvesets.at(0)->getFields().size(), 2);
    ASSERT_EQ(solvesets.at(0)->getConstraints().size(), 1);
    ASSERT_EQ(unconstrained.size(), 0);
}

TEST_F(TestBuildSolveSets, nested_struct_local_constraints_nested_unconstrained) {
    VSC_DATACLASSES(TestBuildSolveSets_nested_struct_local_constraints_nested_unconstrained, MyC, R"(
        @vdc.randclass
        class MyI(object):
            a : vdc.rand_uint32_t 
            b : vdc.rand_uint32_t 
            c : vdc.rand_uint32_t

            @vdc.constraint
            def ab_c(self):
                self.a < self.b

        @vdc.randclass
        class MyC(object):
            a : vdc.rand[MyI]

    )");
    #include "TestBuildSolveSets_nested_struct_local_constraints_nested_unconstrained.h"

    enableDebug(false);
    RefPathSet target_fields, fixed_fields, include_constraints, exclude_constraints;
    RefPathSet unconstrained;

    vsc::dm::IModelFieldUP field(mkRootField("abc", MyC_t));

    std::vector<ISolveSetUP> solvesets;

    TaskBuildSolveSets(
        m_factory->getDebugMgr(),
        field.get(),
        target_fields,
        fixed_fields,
        include_constraints,
        exclude_constraints).build(solvesets, unconstrained);
    
    ASSERT_EQ(solvesets.size(), 1);
    ASSERT_EQ(solvesets.at(0)->getFields().size(), 2);
    ASSERT_EQ(solvesets.at(0)->getConstraints().size(), 1);
    ASSERT_EQ(unconstrained.size(), 1);
    RefPathSet::iterator it = unconstrained.begin();
    ASSERT_TRUE(it.next());
    const std::vector<int32_t> &path = it.path();
    ASSERT_EQ(path.size(), 2);
    ASSERT_EQ(path.at(0), 0);
    ASSERT_EQ(path.at(1), 2);
}

}
}
