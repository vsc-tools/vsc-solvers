/*
 * TestRefPathSet.cpp
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
#include "TestRefPathSet.h"
#include "vsc/solvers/impl/RefPathSet.h"


namespace vsc {
namespace solvers {


TestRefPathSet::TestRefPathSet() {

}

TestRefPathSet::~TestRefPathSet() {

}

TEST_F(TestRefPathSet, smoke) {
    RefPathSet pset;

    ASSERT_TRUE(pset.add({0, 1, 2, 3}));
    ASSERT_FALSE(pset.add({0, 1, 2, 3}));
    ASSERT_TRUE(pset.add({0, 2, 2, 3}));
    ASSERT_TRUE(pset.add({0, 1, 2, 4}));
}

TEST_F(TestRefPathSet, find_add) {
    RefPathSet pset;

    ASSERT_FALSE(pset.find({0, 1, 2, 3}));
    ASSERT_TRUE(pset.add({0, 1, 2, 3}));
    ASSERT_TRUE(pset.find({0, 1, 2, 3}));
    ASSERT_FALSE(pset.add({0, 1, 2, 3}));

    ASSERT_TRUE(pset.add({0, 2, 2, 3}));
    ASSERT_TRUE(pset.add({0, 1, 2, 4}));
}

TEST_F(TestRefPathSet, insert_p) {
    RefPathSet pset;

    for (int32_t i=0; i<1000000; i++) {
        ASSERT_TRUE(pset.add({0, 1, 2, 3, i}));
    }
    for (int32_t i=0; i<1000000; i++) {
        ASSERT_FALSE(pset.add({0, 1, 2, 3, i}));
    }
}

TEST_F(TestRefPathSet, iterate_leaf_only_10) {
    RefPathSet pset;

    for (int32_t i=0; i<10; i++) {
        ASSERT_TRUE(pset.add({i}));
    }
    RefPathSet::iterator it = pset.begin();

    uint32_t i;
    for (i=0; i<10 && it.next(); i++) {
        ASSERT_EQ(it.path().size(), 1);
        ASSERT_EQ(it.path().at(0), i);
    }
    ASSERT_EQ(i, 10);
    ASSERT_FALSE(it.next());
}

TEST_F(TestRefPathSet, iterate_depth_1_single_10) {
    RefPathSet pset;

    for (int32_t i=0; i<10; i++) {
        ASSERT_TRUE(pset.add({10, i}));
    }
    RefPathSet::iterator it = pset.begin();

    uint32_t i;
    for (i=0; i<10 && it.next(); i++) {
        ASSERT_EQ(it.path().size(), 2);
        ASSERT_EQ(it.path().at(0), 10);
        ASSERT_EQ(it.path().at(1), i);
    }
    ASSERT_EQ(i, 10);
    ASSERT_FALSE(it.next());
}

TEST_F(TestRefPathSet, iterate_depth_2_dual_10) {
    RefPathSet pset;

    for (int32_t i=0; i<10; i++) {
        ASSERT_TRUE(pset.add({i, i}));
    }
    RefPathSet::iterator it = pset.begin();

    uint32_t i;
    for (i=0; i<10 && it.next(); i++) {
        ASSERT_EQ(it.path().size(), 2);
        ASSERT_EQ(it.path().at(0), i);
        ASSERT_EQ(it.path().at(1), i);
    }
    ASSERT_EQ(i, 10);
    ASSERT_FALSE(it.next());
}

TEST_F(TestRefPathSet, iterate_depth_4_dual_10) {
    RefPathSet pset;

    for (int32_t i=0; i<5; i++) {
        ASSERT_TRUE(pset.add({i, i+10, i+20, i}));
    }
    RefPathSet::iterator it = pset.begin();

    uint32_t i;
    for (i=0; i<10 && it.next(); i++) {
        ASSERT_EQ(it.path().size(), 4);
        fprintf(stdout, "Path: ");
        for (std::vector<int32_t>::const_iterator
            iit=it.path().begin();
            iit!=it.path().end(); iit++) {
            fprintf(stdout, "%d ", *iit);
        }
        fprintf(stdout, "\n");
//        ASSERT_EQ(it.path().at(0), i);
        ASSERT_EQ(it.path().at(1), it.path().at(0)+10);
        ASSERT_EQ(it.path().at(2), it.path().at(0)+20);
        ASSERT_EQ(it.path().at(3), it.path().at(0));
    }
    ASSERT_EQ(i, 5);
    ASSERT_FALSE(it.next());
}

TEST_F(TestRefPathSet, iterate_depth_4_single_10) {
    RefPathSet pset;

    for (int32_t i=0; i<5; i++) {
        ASSERT_TRUE(pset.add({i, i, i, i}));
    }
    RefPathSet::iterator it = pset.begin();

    uint32_t i;
    for (i=0; i<10 && it.next(); i++) {
        ASSERT_EQ(it.path().size(), 4);
        fprintf(stdout, "Path: ");
        for (std::vector<int32_t>::const_iterator
            iit=it.path().begin();
            iit!=it.path().end(); iit++) {
            fprintf(stdout, "%d ", *iit);
        }
        fprintf(stdout, "\n");
//        ASSERT_EQ(it.path().at(0), i);
        ASSERT_EQ(it.path().at(1), it.path().at(0));
        ASSERT_EQ(it.path().at(2), it.path().at(0));
        ASSERT_EQ(it.path().at(3), it.path().at(0));
    }
    ASSERT_EQ(i, 5);
    ASSERT_FALSE(it.next());
}

}
}
