
#include "Factory.h"
//#include "CompoundSolverDefault.h"
#include "RandState.h"
#include "vsc/solvers/FactoryExt.h"

namespace vsc {
namespace solvers {

Factory::Factory() : m_dmgr(0) {

}

Factory::~Factory() {

}

#ifdef UNDEFINED
ICompoundSolver *Factory::mkCompoundSolver(dm::IContext *ctxt) {
    return new CompoundSolverDefault(ctxt, &m_solver_factory);
}
#endif

IRandState *Factory::mkRandState(const std::string &seed) {
    return new RandState(seed);
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
