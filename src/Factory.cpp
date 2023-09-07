
#include "Factory.h"
#include "CompoundSolver.h"
#include "RandStateMt19937_64.h"
#include "RandStateLehmer_64.h"
#include "RandStateLehmer_64_dual.h"
#include "RandStateLehmer_32.h"
#include "vsc/solvers/FactoryExt.h"

namespace vsc {
namespace solvers {

Factory::Factory() : m_dmgr(0) {

}

Factory::~Factory() {

}

ICompoundSolver *Factory::mkCompoundSolver() {
    return new CompoundSolver(m_dmgr, m_solver_f.get());
}

IRandState *Factory::mkRandState(const std::string &seed) {
    return new RandStateLehmer_64(seed);
//    return new RandStateLehmer_32(seed);
//    return new RandStateMt19937_64(seed);
}

IFactory *Factory::inst() {
    if (!m_inst) {
        m_inst = FactoryUP(new Factory());
    }
    return m_inst.get();
}


FactoryUP Factory::m_inst;

}
}

vsc::solvers::IFactory *vsc_solvers_getFactory() {
    return vsc::solvers::Factory::inst();
}
