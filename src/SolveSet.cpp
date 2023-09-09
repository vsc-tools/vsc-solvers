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
#include <string.h>
#include "SolveSet.h"


namespace vsc {
namespace solvers {


SolveSet::SolveSet() : m_max_bits(0), m_num_bits(0) {
    memset(m_size, 0, sizeof(m_size));
}

SolveSet::~SolveSet() {

}

void SolveSet::setFlag(SolveSetFlags flags) {

}

void SolveSet::addField(
    const std::vector<int32_t>  &path, 
    SolveSetFieldType           type,
    int32_t                     bits) {
    if (m_field_s.add(path, type)) {
        m_size[(uint32_t)type]++;
    }
    if (bits > 0) {
        if (bits > m_max_bits) {
            m_max_bits = bits;
        }
        m_num_bits += bits;
    }
}

void SolveSet::addConstraint(const std::vector<int32_t> &path) {
    m_constraint_s.add(path);
};

int32_t SolveSet::size(SolveSetFieldType type) const {
    return m_size[(uint32_t)type];
}

void SolveSet::merge(SolveSet *rhs) {
    for (RefPathMap<SolveSetFieldType>::iterator 
        it=rhs->getFields().begin(); it.next(); ) {
        addField(it.path(), it.value(), -1);
    }
    for (RefPathSet::iterator 
        it=rhs->getConstraints().begin(); it.next(); ) {
        addConstraint(it.path());
    }
    if (rhs->m_max_bits > m_max_bits) {
        m_max_bits = rhs->m_max_bits;
    }
    m_num_bits += rhs->m_num_bits;
}

}
}
