/*
 * SolveSet.cpp
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
#include "SolveSet.h"


namespace vsc {
namespace solvers {


SolveSet::SolveSet() {

}

SolveSet::~SolveSet() {

}

void SolveSet::addField(const std::vector<int32_t> &path) {
    m_field_s.add(path);
    m_target_field_s.add(path);
}

void SolveSet::addConstraint(const std::vector<int32_t> &path) {
    m_constraint_s.add(path);
};

int32_t SolveSet::size() const {
    return m_field_s.size();
}

void SolveSet::merge(SolveSet *rhs) {
    for (RefPathSet::iterator it=rhs->getFields().begin(); it.next(); ) {
        addField(it.path());
    }
    for (RefPathSet::iterator it=rhs->getConstraints().begin(); it.next(); ) {
        addConstraint(it.path());
    }
}

}
}
