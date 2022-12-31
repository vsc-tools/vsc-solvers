
#include "Factory.h"
#include "CompoundSolverDefault.h"

namespace vsc {
namespace solvers {

Factory::Factory() : m_dmgr(0) {

}

Factory::~Factory() {

}

ICompoundSolver *Factory::mkCompoundSolver(dm::IContext *ctxt) {
    return new CompoundSolverDefault(ctxt, &m_solver_factory);
}

IFactory *Factory::inst() {
    if (!m_inst) {
        m_inst = FactoryUP(new Factory());
    }
    return m_inst.get();
}

extern "C" IFactory *vsc_solvers_getFactory() {
    return Factory::inst();
}

FactoryUP Factory::m_inst;

}
}

