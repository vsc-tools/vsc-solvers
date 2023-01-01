
cimport libvsc_dm.core as vsc_dm
cimport libvsc_solvers.decl as decl

cdef class CompoundSolver(object):

    def __dealloc__(self):
        del self._hndl

    cpdef bool solve(self, RandState randstate, fields, constraints, flags):
        pass

    @staticmethod
    cdef mk(decl.ICompoundSolver *hndl):
        ret = CompoundSolver()
        ret._hndl = hndl
        return ret

cdef class Factory(object):
    pass


cdef class RandState(object):

    cpdef str seed(self):
        return self._hndl.seed().decode()

    cpdef randint32(self, int32_t l, int32_t h):
        return self._hndl.randint32(l, h)

    cpdef randbits(self, vsc_dm.ModelVal v):
        self._hndl.randbits(v._hndl)

    cpdef void setState(self, RandState other):
        self._hndl.setState(other._hndl)

    cpdef RandState clone(self):
        return RandState.mk(self._hndl.clone())

    cpdef RandState next(self):
        return RandState.mk(self._hndl.next())

    @staticmethod
    cdef mk(decl.IRandState *hndl):
        ret = RandState()
        ret._hndl = hndl
        return ret

