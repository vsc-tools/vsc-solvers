/*
 * RandStateMt19937_64.h
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


class RandStateMt19937_64;
using RandStateUP=std::unique_ptr<RandStateMt19937_64>;
class RandStateMt19937_64 : public IRandState {
public:
	RandStateMt19937_64(const std::string &seed);

	RandStateMt19937_64(const std::mt19937_64 &state);

	virtual ~RandStateMt19937_64();

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
	std::mt19937_64 	m_state;

};

}
}

