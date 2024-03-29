
#pragma once
#include <memory>
#include "vsc/solvers/IFactory.h"


namespace vsc {
namespace solvers {

class Factory;
using FactoryUP=std::unique_ptr<Factory>;
class Factory : public virtual IFactory {
public:
    Factory();

    virtual ~Factory();

	virtual void init(dmgr::IDebugMgr *dmgr) override {
        m_dmgr = dmgr;
    }

    virtual dmgr::IDebugMgr *getDebugMgr() override {
        return m_dmgr;
    }

    virtual ISolverFactory *getSolverFactory() override;

	virtual ICompoundSolver *mkCompoundSolver() override;

    virtual IRandState *mkRandState(const std::string &seed) override;

    static IFactory *inst();


private:
    static FactoryUP                    m_inst;
    dmgr::IDebugMgr                     *m_dmgr;
    ISolverFactoryUP                    m_solver_f;

};


}
}


