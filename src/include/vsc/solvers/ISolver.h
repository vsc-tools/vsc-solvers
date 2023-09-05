/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/*
 * ISolverInst.h
 *
 *  Created on: Jul 7, 2020
 *      Author: ballance
 */

#pragma once
#include <vector>
#include "vsc/dm/IModelField.h"
#include "vsc/dm/ITypeConstraint.h"
#include "vsc/solvers/IRandState.h"
#include "vsc/solvers/impl/RefPathSet.h"

namespace vsc {
namespace solvers {

class ISolver;
using ISolverUP=std::unique_ptr<ISolver>;

/**
 * Represents an instance of a backend solver
 */
class ISolver {
public:

	virtual ~ISolver() { }

    virtual bool randomize(
        IRandState                              *randstate,
        const std::vector<dm::IModelFieldUP>    &root_fields,
        const RefPathSet                        &target_fields,
        const RefPathSet                        &fixed_fields
    ) = 0;

//    virtual bool sat() = 0;

};

}
}

