
#pragma once
#include "vsc/dm/impl/VisitorBase.h"

namespace vsc {
namespace solvers {


class TaskFixModelField : public virtual dm::VisitorBase {
public:
    static const uint32_t       Flags_Scalar  = (1 << 0);
    static const uint32_t       Flags_VecSize = (1 << 1);
    static const uint32_t       Flags_All     = (Flags_Scalar | Flags_VecSize);


public:

    TaskFixModelField(uint32_t flags = Flags_All);

    virtual ~TaskFixModelField();

    void fix(const std::vector<dm::IModelField *> &fields);

	virtual void visitModelField(dm::IModelField *f) override;

private:
    uint32_t                            m_flags;

};

}
}
