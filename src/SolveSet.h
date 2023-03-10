/*
 * SolveSet.h
 *
 *  Created on: Oct 7, 2021
 *      Author: mballance
 */

#pragma once
#include <memory>
#include <unordered_set>
#include <vector>
#include "dmgr/IDebugMgr.h"
#include "vsc/dm/IModelConstraint.h"
#include "vsc/dm/IModelConstraintSoft.h"
#include "vsc/dm/IModelField.h"
#include "vsc/dm/IModelFieldVec.h"

namespace vsc {
namespace solvers {


enum class SolveSetFlag {
	NoFlags = 0,
	HaveForeach = (1 << 0)
};

static inline SolveSetFlag operator | (const SolveSetFlag lhs, const SolveSetFlag rhs) {
	return static_cast<SolveSetFlag>(
			static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
}

static inline SolveSetFlag operator |= (SolveSetFlag &lhs, const SolveSetFlag rhs) {
	lhs = static_cast<SolveSetFlag>(
			static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
	return lhs;
}

static inline SolveSetFlag operator & (const SolveSetFlag lhs, const SolveSetFlag rhs) {
	return static_cast<SolveSetFlag>(
			static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
}

static inline SolveSetFlag operator ~ (const SolveSetFlag lhs) {
	return static_cast<SolveSetFlag>(~static_cast<uint32_t>(lhs));
}

class SolveSet;
using SolveSetUP = std::unique_ptr<SolveSet>;

class SolveSet {
public:
	SolveSet(dmgr::IDebugMgr *dmgr);

	virtual ~SolveSet();

	void add_field(dm::IModelField *f);

	void add_constraint(dm::IModelConstraint *c);

	void add_soft_constraint(dm::IModelConstraintSoft *c);

	const std::vector<dm::IModelField *> &all_fields() const {
		return m_all_fields;
	}

	const std::vector<dm::IModelField *> &rand_fields() const {
		return m_rand_fields;
	}

	const std::vector<dm::IModelFieldVec *> &constrained_sz_vec() const {
		return m_constrained_sz_vec;
	}

	const std::vector<dm::IModelConstraint *> &constraints() const {
		return m_constraints;
	}

	const std::vector<dm::IModelConstraintSoft *> &soft_constraints() const {
		return m_soft_constraints;
	}

	SolveSetFlag getFlags() const { return m_flags; }

	void setFlags(SolveSetFlag flags) { m_flags |= flags; }

	bool hasFlags(SolveSetFlag flags) { return (m_flags & flags) == flags; }

	/**
	 * Merge data from the `src` set into this one
	 */
	void merge(SolveSet *src);

private:
    static dmgr::IDebug                             *m_dbg;
	std::unordered_set<dm::IModelField *>			m_field_s;
	std::vector<dm::IModelField *>				    m_all_fields;
	std::vector<dm::IModelField *>				    m_rand_fields;
	std::vector<dm::IModelFieldVec *>			    m_constrained_sz_vec;
	std::unordered_set<dm::IModelConstraint *>		m_constraint_s;
	std::vector<dm::IModelConstraint *>			    m_constraints;

	std::unordered_set<dm::IModelConstraintSoft *>	m_soft_constraint_s;
	std::vector<dm::IModelConstraintSoft *>			m_soft_constraints;
	SolveSetFlag						            m_flags;

};

}
}

