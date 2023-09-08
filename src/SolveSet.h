/**
 * SolveSet.h
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
#pragma once
#include "vsc/dm/impl/UP.h"
#include "vsc/solvers/impl/RefPathSet.h"

namespace vsc {
namespace solvers {

class SolveSet;
using SolveSetUP=dm::UP<SolveSet>;
class SolveSet {
public:
    SolveSet();

    virtual ~SolveSet();

    void addField(const std::vector<int32_t> &path);

    void addConstraint(const std::vector<int32_t> &path);

    const RefPathSet &getFields() const { return m_field_s; }

    const RefPathSet &getConstraints() const { return m_constraint_s; }

    int32_t size() const;

    void merge(SolveSet *rhs);

private:
    RefPathSet                      m_field_s;
    RefPathSet                      m_target_field_s;
    RefPathSet                      m_constraint_s;


};

}
}


