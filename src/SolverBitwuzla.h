/*
 * SolverBitwuzla.h
 *
 *  Created on: Oct 23, 2021
 *      Author: mballance
 */

#pragma once
#include <stdint.h>
#include <unordered_map>
#include "vsc/dm/IContext.h"
#include "vsc/solvers/ISolver.h"

typedef struct Bitwuzla Bitwuzla;
typedef struct BitwuzlaTerm BitwuzlaTerm;
typedef struct BitwuzlaSort BitwuzlaSort;

namespace vsc {
namespace solvers {


class SolverBitwuzla : public ISolver {
public:
	SolverBitwuzla(dm::IContext *ctxt);

	virtual ~SolverBitwuzla();

	Bitwuzla *bitwuzla() const { return m_bitwuzla; }

	// Creates solver data for a field (and possibly sub-fields)
	virtual void initField(dm::IModelField *f) override;

	// Creates solver data for a constraint
	virtual void initConstraint(dm::IModelConstraint *c) override;

	virtual void addAssume(dm::IModelConstraint *c) override;

	virtual void addAssert(dm::IModelConstraint *c) override;

	virtual bool isSAT() override;

	virtual void setFieldValue(dm::IModelField *f) override;

	const BitwuzlaSort *get_sort(int32_t width);

	void addFieldData(dm::IModelField *f, const BitwuzlaTerm *n);

	const BitwuzlaTerm *findFieldData(dm::IModelField *f);

private:
    static dmgr::IDebug                                         *m_dbg;
    dm::IContext                                                *m_ctxt;
	Bitwuzla				*m_bitwuzla;
	std::unordered_map<dm::IModelField *,const BitwuzlaTerm *>		m_field_node_m;
	std::unordered_map<dm::IModelConstraint *,const BitwuzlaTerm *>	m_constraint_node_m;
	std::unordered_map<uint32_t, const BitwuzlaSort *>			m_sort_m;
	bool														m_issat_valid;
	bool														m_issat;
};

}
}

