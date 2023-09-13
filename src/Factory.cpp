
#include <unistd.h>
#include "Factory.h"
#include "CompoundSolver.h"
#include "RandStateMt19937_64.h"
#include "RandStateLehmer_64.h"
#include "RandStateLehmer_64_dual.h"
#include "RandStateLehmer_32.h"
#include "vsc/solvers/FactoryExt.h"
#include "SolverFactoryBoolector.h"

namespace vsc {
namespace solvers {

Factory::Factory() : m_dmgr(0) {

}

Factory::~Factory() {

}

ICompoundSolver *Factory::mkCompoundSolver() {
    return new CompoundSolver(m_dmgr, getSolverFactory());
}

IRandState *Factory::mkRandState(const std::string &seed) {
    return new RandStateLehmer_64(seed);
//    return new RandStateLehmer_32(seed);
//    return new RandStateMt19937_64(seed);
}

ISolverFactory *Factory::getSolverFactory() {
    if (!m_solver_f) {
        const char *vsc_solver_strategy = getenv("VSC_SOLVER_STRATEGY");

        if (vsc_solver_strategy && vsc_solver_strategy[0]) {

        } else {
            m_solver_f = ISolverFactoryUP(new SolverFactoryBoolector(m_dmgr));
        }
    }
    return m_solver_f.get();
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
