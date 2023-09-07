/*
 * RandStateMt19937_64.cpp
 *
 *  Created on: Oct 31, 2021
 *      Author: mballance
 */

#include <random>
#include "RandStateMt19937_64.h"

namespace vsc {
namespace solvers {


RandStateMt19937_64::RandStateMt19937_64(const std::string &seed) : m_seed(seed) {
	std::seed_seq seq(seed.begin(), seed.end());
	m_state = std::mt19937_64(seq);
}

RandStateMt19937_64::RandStateMt19937_64(const std::mt19937_64 &state) : m_state(state) {

}

RandStateMt19937_64::~RandStateMt19937_64() {
	// TODO Auto-generated destructor stub
}

int32_t RandStateMt19937_64::randint32(
			int32_t		min,
			int32_t		max) {
	uint64_t next_v = next_ui64();
	if (min == max) {
		return min;
	} else {
		next_v = (next_v % (max-min+1)) + min;
		return next_v;
	}
}

uint64_t RandStateMt19937_64::rand_ui64() {
	return next_ui64();
}

int64_t RandStateMt19937_64::rand_i64() {
	return static_cast<int64_t>(next_ui64());
}

void RandStateMt19937_64::randbits(dm::IModelVal *val) {
	if (val->bits() <= 64) {
		uint64_t bits = next_ui64();
		val->val_u(bits);
	} else {
		uint32_t n_words = (val->bits()-1)/64+1;
		for (uint32_t i=0; i<n_words; i++) {
			uint64_t rv = next_ui64();
#ifdef UNDEFINED
		val.set_word(i, rv);

		if (i+1 < n_words) {
			i++;
			val.set_word(i, rv >> 32);
		}

		if (i+1 == n_words && (val.bits()%32) != 0) {
			// Mask the last word
			val.set_word(i,
					(val.get_word(i) & (1 << (val.bits()%32))-1));
		}
#endif
		}
	}
}

uint64_t RandStateMt19937_64::next_ui64() {
	uint64_t ret = m_state();
	return ret;
}

void RandStateMt19937_64::setState(IRandState *other) {
	m_state = dynamic_cast<RandStateMt19937_64 *>(other)->m_state;
}

IRandState *RandStateMt19937_64::clone() const {
	RandStateMt19937_64 *ret = new RandStateMt19937_64(m_state);
	return ret;
}

IRandState *RandStateMt19937_64::next() {
	m_state(); // Mutate state
	// Return a clone
	return new RandStateMt19937_64(m_state);
}

}
}
