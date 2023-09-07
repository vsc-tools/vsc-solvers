/*
 * RandStateLehmer_64_dual.cpp
 *
 *  Created on: Oct 31, 2021
 *      Author: mballance
 */

#include <random>
#include "RandStateLehmer_64_dual.h"

namespace vsc {
namespace solvers {


RandStateLehmer_64_dual::RandStateLehmer_64_dual(const std::string &seed) : m_seed(seed) {
    m_idx = 0;
    m_state1 = 1;
    m_state2 = 3;
    for (uint32_t i=0; i<seed.size(); i++) {
        m_state1 += seed.at(i);
        m_state1 ^= 25;
        m_state2 += seed.at(i);
        m_state2 ^= 25;
    }
}

RandStateLehmer_64_dual::RandStateLehmer_64_dual(const RandStateLehmer_64_dual &rhs) {
    m_idx = rhs.m_idx;
    m_state1 = rhs.m_state1;
    m_state2 = rhs.m_state2;
}

RandStateLehmer_64_dual::~RandStateLehmer_64_dual() {

}

int32_t RandStateLehmer_64_dual::randint32(
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

uint64_t RandStateLehmer_64_dual::rand_ui64() {
	return next_ui64();
}

int64_t RandStateLehmer_64_dual::rand_i64() {
	return static_cast<int64_t>(next_ui64());
}

void RandStateLehmer_64_dual::randbits(dm::IModelVal *val) {
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

uint64_t RandStateLehmer_64_dual::next_ui64() {
    if (m_idx == 0) {
        m_idx = 1;
        m_state1 *= 0xda942042e4dd58b5;
        return m_state1 >> 64;
    } else {
        m_idx = 0;
        m_state2 *= 0xda942042e4dd58b5;
        return m_state2 >> 64;
    }
}

void RandStateLehmer_64_dual::setState(IRandState *other) {
	m_idx = dynamic_cast<RandStateLehmer_64_dual *>(other)->m_idx;
	m_state1 = dynamic_cast<RandStateLehmer_64_dual *>(other)->m_state1;
	m_state2 = dynamic_cast<RandStateLehmer_64_dual *>(other)->m_state2;
}

IRandState *RandStateLehmer_64_dual::clone() const {
	return new RandStateLehmer_64_dual(*this);
}

IRandState *RandStateLehmer_64_dual::next() {
    rand_ui64(); // Mutate state

	// Return a clone
	return clone(); 
}

}
}
