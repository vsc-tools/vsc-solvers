/*
 * SolveSetSwizzlerPartsel.cpp
 *
 *  Created on: Oct 24, 2021
 *      Author: mballance
 */

#include "dmgr/impl/DebugMacros.h"
#include "SolveSetSwizzlerPartsel.h"

namespace vsc {
namespace solvers {


SolveSetSwizzlerPartsel::SolveSetSwizzlerPartsel(
    dm::IContext            *ctxt,
    IRandState              *randstate) : m_ctxt(ctxt), m_randstate(randstate) {
	m_solver = 0;
	m_sset = 0;
    DEBUG_INIT("SolveSetSwizzlerPartsel", ctxt->getDebugMgr());
}

SolveSetSwizzlerPartsel::~SolveSetSwizzlerPartsel() {
	// TODO Auto-generated destructor stub
}

void SolveSetSwizzlerPartsel::swizzle(
			ISolver		*solver,
			SolveSet	*sset) {
	DEBUG_ENTER("swizzle");
	m_solver = solver;
	m_sset = sset;

	// TODO: handle solve order
	swizzle_field_l(sset->rand_fields());

	DEBUG_LEAVE("swizzle");
}

void SolveSetSwizzlerPartsel::swizzle_field_l(
		const std::vector<dm::IModelField *>		&fields) {
	DEBUG_ENTER("swizzle_field_l");

	uint32_t max_swizzle = 4;
	// Create a copy for random selection
	std::vector<dm::IModelField *> fields_c(fields.begin(), fields.end());
	std::vector<dm::IModelConstraintUP> constraints;

	DEBUG_ENTER("Select %d fields from %d", max_swizzle, fields_c.size());
	for (uint32_t i=0; i<max_swizzle && fields_c.size(); i++) {
		uint32_t target_idx = m_randstate->randint32(0, fields_c.size()-1);
		dm::IModelField *field = fields_c.at(target_idx);
		fields_c.erase(fields_c.begin()+target_idx);
		swizzle_field(field, constraints);
		DEBUG("select field %s", field->name().c_str());
	}
	DEBUG_LEAVE("Select %d fields from %d (%d constraints)",
			max_swizzle, fields_c.size(), constraints.size());

	// Build out the solver representation
	for (auto c_it=constraints.begin(); c_it!=constraints.end(); c_it++) {
		m_solver->initConstraint(c_it->get());
	}

	// Now, apply as many as possible
	for (auto c_it=constraints.begin(); c_it!=constraints.end(); c_it++) {
		m_solver->addAssume(c_it->get());
		if (m_solver->isSAT()) {
			DEBUG("Constraint is SAT");
			m_solver->addAssert(c_it->get());
		} else {
			DEBUG("Constraint is UNSAT");
		}
	}

	DEBUG_LEAVE("swizzle_field_l");
}

void SolveSetSwizzlerPartsel::swizzle_field(
		dm::IModelField 				*f,
		std::vector<dm::IModelConstraintUP>		&constraints) {
	DEBUG_ENTER("swizzle_field %s", f->name().c_str());

	// TODO: handle distribution
	if (false /* Have distribution */) {
	} else if (false /* Have multi-interval domain */) {
	} else {
		create_rand_domain_constraint(f, constraints);
	}
	DEBUG_LEAVE("swizzle_field %s", f->name().c_str());
}

void SolveSetSwizzlerPartsel::create_rand_domain_constraint(
			dm::IModelField				*f,
			std::vector<dm::IModelConstraintUP>	&constraints) {
	DEBUG_ENTER("create_rand_domain_constraint %s", f->name().c_str());
	// TODO: Should be able to incorporate min/max information.
	// Specifically, there is a width inside min/max
	// TODO: should probably do this with 'val' to ensure we
	// can handle wide variables
	dm::IModelValUP val(m_ctxt->mkModelVal());
	dm::IModelValUP slice(m_ctxt->mkModelVal());
	uint32_t width = f->val()->bits();

	val->set(f->val());

	m_randstate->randbits(val.get());
	DEBUG("randbits: 0x%08llx", val->val_u());

	uint32_t max_intervals = 4;

	std::vector<dm::IModelConstraintUP> swizzle_c;
	if (width > max_intervals) {
		// Partition into 'max_intervals' ranges
		DEBUG("must partition: width=%d max_intervals=%d", width, max_intervals);

		uint32_t bit=0;
		for (uint32_t i=0; i<max_intervals; i++) {
			int32_t lower = bit;
			int32_t upper = bit;
			if (i+1 == max_intervals) {
				DEBUG("[%d] last bin %d..%d", i, bit, width-1);
				upper = width-1;
			} else if ((max_intervals-i) >= (width-bit)) {
				// Remaining intervals are of size 1
				DEBUG("[%d] single-bit bin %d", i, bit);
			} else {
				// Intervals range from 1..(W-N-B)
				// - Total width:    W
				// - Max intervals:  M
				// - Bin:            I
				// - Bit:            B
				// - Remaining bits: R (before must switch to single-bit intervals)
				//   - R = (W-M-B-I)
				// W M I B Ri
				// 8-4-0-0 4   -- 1..4 -- 1
				// 8-4-1-1 3   -- 1..3 -- 1
				// 8-4-2-2 2   -- 1..2 -- 1
				// 8-4-3-3 1   -- 1..2 -- 1
				//
				// Ceiling is defined by Ri
				// W Ri B
				// 8 4  0 1..4   -- 1
				// 8 3  1 1..4   -- 1
				// 8 2  2 1..4   -- 1
				// 8 1  3 5      -- 5
				//
				// 1..(W-Ri-B)
				//
				// If W=8, N=4, bit=0: 1..5
				// If W=8, N=4, bit=1: 1..4
				int32_t n_bits = m_randstate->randint32(
						1,
						(width-(max_intervals-i)-bit));
				DEBUG("[%d] rand-width bin %d..%d (lim=%d)",
						i, bit, (bit+n_bits-1),
						(width-(max_intervals-i)-bit));
				upper = bit+n_bits-1;
			}
            val->slice(
                slice.get(),
                upper,
                lower
            );

			swizzle_c.push_back(dm::IModelConstraintUP(
                    m_ctxt->mkModelConstraintExpr(
                        m_ctxt->mkModelExprBin(
                            m_ctxt->mkModelExprPartSelect(
                                m_ctxt->mkModelExprFieldRef(f),
                                upper,
                                lower),
                            dm::BinOp::Eq,
                            m_ctxt->mkModelExprVal(slice.get())
                        )
                    )));
			bit = upper+1;
		}
	} else {
		// Single bits
		DEBUG("single bits: width=%d max_intervals=%d", width, max_intervals);
		for (uint32_t i=0; i<width; i++) {
            val->slice(
                slice.get(),
                i,
                i);

			swizzle_c.push_back(dm::IModelConstraintUP(
                m_ctxt->mkModelConstraintExpr(
                    m_ctxt->mkModelExprBin(
                        m_ctxt->mkModelExprPartSelect(
                            m_ctxt->mkModelExprFieldRef(f),
                            i,
                            i),
                        dm::BinOp::Eq,
                        m_ctxt->mkModelExprVal(slice.get())
                    ))));
		}
	}

	// Finally, move the slice constraints in pseudo-random order
	// back into the result constraints vector
	while (swizzle_c.size()) {
		int32_t sel_idx = m_randstate->randint32(0, swizzle_c.size()-1);
		dm::IModelConstraint *c = swizzle_c[sel_idx].release();
		swizzle_c.erase(swizzle_c.begin()+sel_idx);
		constraints.push_back(dm::IModelConstraintUP(c));
	}
	DEBUG_LEAVE("create_rand_domain_constraint %s", f->name().c_str());
}

dmgr::IDebug         *SolveSetSwizzlerPartsel::m_dbg = 0;

}
}

