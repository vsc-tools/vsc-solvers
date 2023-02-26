/**
 * TaskApplySoftBinSearch.h
 *
 * Copyright 2022 Matthew Ballance and Contributors
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
#include "vsc/dm/IModelConstraint.h"
#include "vsc/solvers/ISolver.h"

namespace vsc {
namespace solvers {



class TaskApplySoftBinSearch {
public:
    TaskApplySoftBinSearch(ISolver *solver);

    virtual ~TaskApplySoftBinSearch();

    bool apply(
        const std::vector<dm::IModelConstraintUP> &constraints);

private:
    bool is_sat();

private:
    ISolver                     *m_solver;

};

}
}


