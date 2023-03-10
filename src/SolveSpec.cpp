/*
 * Copyright 2019-2021 Matthew Ballance and contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SolveSpec.cpp
 *
 *  Created on: Oct 7, 2021
 *      Author: mballance
 */

#include "SolveSpec.h"

namespace vsc {
namespace solvers {


SolveSpec::SolveSpec(
		const std::vector<SolveSet *>			&solvesets,
		const std::vector<dm::IModelField *>		&unconstrained,
		const std::vector<dm::IModelFieldVec *>		&unconstrained_sz_vec) :
				m_unconstrained(unconstrained),
				m_unconstrained_sz_vec(unconstrained_sz_vec) {

	for (auto it=solvesets.begin(); it!=solvesets.end(); it++) {
		m_solvesets.push_back(SolveSetUP(*it));
	}


}

SolveSpec::~SolveSpec() {
	// TODO Auto-generated destructor stub
}

}
}

