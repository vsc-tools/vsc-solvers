/*
 * RandStateLehmer_64.h
 *
 *  Created on: Oct 31, 2021
 *      Author: mballance
 */

#pragma once
#include <random>
#include <stdint.h>
#include "vsc/solvers/IRandState.h"
#include "vsc/dm/IModelVal.h"

namespace vsc {
namespace solvers {


class RandStateLehmer_64 : public IRandState {
public:
	RandStateLehmer_64(const std::string &seed);

	RandStateLehmer_64(const RandStateLehmer_64 &rhs);

	virtual ~RandStateLehmer_64();

	virtual const std::string &seed() const override {
		return m_seed;
	}

	virtual int32_t randint32(
			int32_t		min,
			int32_t		max) override;

	virtual uint64_t rand_ui64() override;

	virtual int64_t rand_i64() override;

	/**
	 * Fills the value with a random bit pattern
	 */
	virtual void randbits(dm::IModelVal *val) override;
	
	virtual void setState(IRandState *other) override;

	virtual IRandState *clone() const override;

	virtual IRandState *next() override;

protected:
	uint64_t next_ui64();

private:
	std::string			m_seed;
    __uint128_t         m_state;

};

}
}

