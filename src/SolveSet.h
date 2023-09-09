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
#include "vsc/solvers/ISolveSet.h"

namespace vsc {
namespace solvers {


class SolveSet;
using SolveSetUP=dm::UP<SolveSet>;
class SolveSet : public virtual ISolveSet {
public:
    SolveSet();

    virtual ~SolveSet();

    virtual SolveSetFlags getFlags() const override { return m_flags; }

    void setFlag(SolveSetFlags flags);

    void addField(
        const std::vector<int32_t>  &path,
        SolveSetFieldType           type,
        int32_t                     bits=-1);

    void addConstraint(const std::vector<int32_t> &path);

    virtual const RefPathMap<SolveSetFieldType> &getFields() const override {
        return m_field_s;
    }

    const RefPathSet &getConstraints() const { return m_constraint_s; }

    int32_t size(SolveSetFieldType type=SolveSetFieldType::Target) const;

    void merge(SolveSet *rhs);

private:
    SolveSetFlags                   m_flags;
    uint32_t                        m_max_bits;
    uint32_t                        m_num_bits;
    uint32_t                        m_size[(uint32_t)SolveSetFieldType::NumTypes];

    RefPathMap<SolveSetFieldType>   m_field_s;
    RefPathSet                      m_constraint_s;


};

}
}


