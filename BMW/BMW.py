from enum import IntEnum
from typing import Union
from pathlib import Path
import ctypes
import os
_dir = Path(__file__).parent.resolve()
_lib_name = "libBMW.so"
BMWLib = ctypes.CDLL(str(_dir / _lib_name))

# https://dbader.org/blog/python-ctypes-tutorial-part-2
def wrap_function(lib, funcname, restype, argtypes):
    """Simplify wrapping ctypes functions"""
    func = lib.__getattr__(funcname)
    func.restype = restype
    func.argtypes = argtypes
    return func

class OpaquePointer(ctypes.c_void_p):
    pass

class WaveformType(IntEnum):
    Sine   = 0
    Ramp   = 1
class TriggerState(IntEnum):
    Enable  = 0
    Disable = 1
class CoilStatus(IntEnum):
    OFF     = 0
    CONFIG  = 1
    TRIGGER = 2
    TEST    = 3
    UNKNOWN = 4
class CoilType(IntEnum):
    Coil   = 0
    Energy = 1
    Empty  = 2
CreateListOfCoils = wrap_function(BMWLib, "CreateListOfHallACoils", OpaquePointer  , [ctypes.POINTER(ctypes.c_size_t)])
DeleteListOfCoils = wrap_function(BMWLib, "DestroyListOfCoils"    , None           , [OpaquePointer, ctypes.c_size_t] )
GetCoilMemSize    = wrap_function(BMWLib, "GetCoilMemSize"        , ctypes.c_size_t, None                             )

SetCoilAmplitude  = wrap_function(BMWLib, "SetBMWAmplitude"       , ctypes.c_bool  , [OpaquePointer, ctypes.c_double] )
SetCoilWaveType   = wrap_function(BMWLib, "SetBMWWave"           , ctypes.c_bool  , [OpaquePointer, ctypes.c_int]     )
SetCoilFrequency  = wrap_function(BMWLib, "SetBMWFrequency"      , ctypes.c_bool  , [OpaquePointer, ctypes.c_double]  )
SetCoilCycles     = wrap_function(BMWLib, "SetBMWCycles"         , ctypes.c_bool  , [OpaquePointer, ctypes.c_int]     )
SetCoilTrigger    = wrap_function(BMWLib, "SetBMWTrigger"        , ctypes.c_bool  , [OpaquePointer, ctypes.c_int]     )

GetCoilName       = wrap_function(BMWLib, "GetCoilName"           , ctypes.c_char_p, [OpaquePointer]                  )
GetCoilId         = wrap_function(BMWLib, "GetCoilId"             , ctypes.c_int   , [OpaquePointer]                  )
GetCoilType       = wrap_function(BMWLib, "GetModType"            , ctypes.c_int   , [OpaquePointer]                  )
GetCoilStatus     = wrap_function(BMWLib, "GetStatus"             , ctypes.c_int   , [OpaquePointer]                  )

EnableBeamMod     = wrap_function(BMWLib, "EnableBeamMod"         , ctypes.c_bool  , None                             )
DisableBeamMod    = wrap_function(BMWLib, "DisableBeamMod"        , ctypes.c_bool  , None                             )
IsBeamModEnabled  = wrap_function(BMWLib, "isBeamModEnabled"      , ctypes.c_bool  , None                             )

BeamModStartup    = wrap_function(BMWLib, "BeamModStartup"        , ctypes.c_bool  , None                             )

class BMWCoils:
    def __init__(self, ptr):
        self.obj    = ptr;
        self.name   = GetCoilName(self).decode("utf-8")
        self.id     = GetCoilId(self)
        self.type   = CoilType(GetCoilType(self))

    @property
    def _as_parameter_(self) -> int:
        """Allows passing a BMWCoil instance directly to ctypes functions."""
        return self.obj

    def GetStatus(self) -> CoilStatus:
        return CoilStatus(GetCoilStatus(self))
    def GetName(self):
        return self.name
    def GetId(self):
        return self.id
    def GetType(self):
        return self.type

    def SetAmplitude(self, val : float) -> bool:
        return SetCoilAmplitude(self, val)
    def SetWaveType(self, val : WaveformType) -> bool:
        return SetCoilWaveType(self, val)
    def SetFrequency(self, val : float) -> bool:
        return SetCoilFrequency(self, val)
    def SetPeriods(self, val : int) -> bool:
        return SetCoilCycles(self, val)
    def EnableTrigger(self, val : TriggerState) -> bool:
        return SetCoilTrigger(self, val)

class BeamModCoils:
    def __init__(self):
        self._ncoils = ctypes.c_size_t(0)
        self._base_addr = None
        self._base_addr  = CreateListOfCoils(ctypes.byref(self._ncoils))
        if not self._base_addr:
            raise MemoryError(f"{_lib_name} could not allocate coil array!")
        self._nbytes = GetCoilMemSize()
        self._coils  = self.__GetCoilArrayFromBase(self._base_addr, self._nbytes, self._ncoils.value)
    def __del__(self):
        self.Close()

    def Close(self):
        if not self._base_addr:
            return
        DeleteListOfCoils(self._base_addr, self._ncoils)
        self._base_addr = None
        self._ncoils = 0

    def __GetCoilArrayFromBase(self, _base_addr, mem_size, count):
        return [
            BMWCoils(_base_addr.value + (i * mem_size))
            for i in range(count)
        ]
    def __iter__(self):
        return iter(self._coils)
