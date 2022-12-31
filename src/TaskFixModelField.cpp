
#include "TaskFixModelField.h"

namespace vsc {
namespace solvers {


TaskFixModelField::TaskFixModelField(uint32_t flags) : m_flags(flags) {

}

TaskFixModelField::~TaskFixModelField() {

}

void TaskFixModelField::fix(const std::vector<dm::IModelField *> &fields) {
    for (std::vector<dm::IModelField *>::const_iterator
            it=fields.begin();
            it!=fields.end(); it++) {
        (*it)->accept(m_this);
    }
}

void TaskFixModelField::visitModelField(dm::IModelField *f) {
    bool fix = (m_flags == Flags_All);

    fix |= (f->isFlagSet(dm::ModelFieldFlag::VecSize) && (m_flags & Flags_VecSize));

    if (fix) {
        f->setFlag(dm::ModelFieldFlag::Resolved);
    }
}

}
}
