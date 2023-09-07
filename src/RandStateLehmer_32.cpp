/*
 * RandStateLehmer_32.cpp
 *
 *  Created on: Oct 31, 2021
 *      Author: mballance
 */

#include <random>
#include "RandStateLehmer_32.h"

namespace vsc {
namespace solvers {


RandStateLehmer_32::RandStateLehmer_32(const std::string &seed) : m_seed(seed) {
    m_state = 1;
    for (uint32_t i=0; i<seed.size(); i++) {
        m_state += seed.at(i);
        m_state ^= 25;
    }
}

RandStateLehmer_32::RandStateLehmer_32(const RandStateLehmer_32 &rhs) {
    m_state = rhs.m_state;
}

RandStateLehmer_32::~RandStateLehmer_32() {

}

int32_t RandStateLehmer_32::randint32(
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

uint64_t RandStateLehmer_32::rand_ui64() {
	return next_ui64();
}

int64_t RandStateLehmer_32::rand_i64() {
	return static_cast<int64_t>(next_ui64());
}

void RandStateLehmer_32::randbits(dm::IModelVal *val) {
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

uint64_t RandStateLehmer_32::next_ui64() {
    uint64_t ret;
    m_state *= 0xda942042e4dd58b5;
    ret = (m_state >> 32);
    /*
    ret <<= 32;
    m_state *= 0xda942042e4dd58b5;
    ret |= (m_state >> 32);
     */
    return ret;
}

void RandStateLehmer_32::setState(IRandState *other) {
	m_state = dynamic_cast<RandStateLehmer_32 *>(other)->m_state;
}

IRandState *RandStateLehmer_32::clone() const {
	return new RandStateLehmer_32(*this);
}

IRandState *RandStateLehmer_32::next() {
    rand_ui64(); // Mutate state

	// Return a clone
	return clone(); 
}

}
}
