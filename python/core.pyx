
import ctypes
import os
cimport debug_mgr.core as dm_core
cimport vsc_dm.core as vsc_dm
cimport vsc_solvers.decl as decl

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

cdef Factory _inst = None
cdef class Factory(object):

    cdef init(self, dm_core.Factory f):
        self._hndl.init(f._hndl.getDebugMgr())

    @staticmethod
    def inst():
        cdef decl.IFactory *hndl = NULL
        cdef Factory factory
        global _inst

        if _inst is None:
            ext_dir = os.path.dirname(os.path.abspath(__file__))

            core_lib = os.path.join(ext_dir, "libvsc-solvers.so")
            if not os.path.isfile(core_lib):
                raise Exception("Extension library core \"%s\" doesn't exist" % core_lib)
            
            so = ctypes.cdll.LoadLibrary(core_lib)
            func = so.vsc_solvers_getFactory
            func.restype = ctypes.c_void_p
            
            hndl = <decl.IFactoryP>(<intptr_t>(func()))
            factory = Factory()
            factory._hndl = hndl
            factory.init(dm_core.Factory.inst())
            _inst = factory

        return _inst

    cpdef RandState mkRandState(self, seed):
        return RandState.mk(self._hndl.mkRandState(str(seed).encode()))

    cpdef CompoundSolver mkCompoundSolver(self, vsc_dm.Context ctxt):
        return CompoundSolver.mk(self._hndl.mkCompoundSolver(ctxt._hndl))


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

