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
    Off     = 0
    Config  = 1
    Trigger = 2
    Test    = 3
    Unknown = 4
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
    def __init__(self, ptr, resource_mgt: "BeamModCoils"):
        self.obj = ptr;
        self.parent = resource_mgt
        self.name   = GetCoilName(self)
        self.id     = GetCoilId(self)
        self.type   = CoilType(GetCoilType(self))

    @property
    def _as_parameter_(self) -> int:
        """Allows passing a BMWCoil instance directly to ctypes functions."""
        if self.parent.base_addr is None:
            raise RuntimeError("Cannot use BMWCoil after its parent buffer has been freed.")
        return self.handle
    def GetStatus(self) -> CoilStatus:
        return CoilStatus(GetCoilStatus(self))

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
        self.ncoils = ctypes.c_size_t(0)
        self.base_addr  = CreateListOfCoils(ctypes.byref(self.ncoils))
        if not self.coils:
            raise MemoryError(f"{_lib_name_} could not allocate coil array!")
        self.nbytes = GetCoilMemSize()
        self.coils  = __GetCoilArrayFromBase(self.base_addr, self.nbytes, self.ncoils.value)

    def __del__(self):
        DeleteListOfCoils(self.base_addr, self.ncoils)
        self.base_addr = None
        self.ncoils = 0

    def __GetCoilArrayFromBase(self, base_addr, mem_size, count):
        array = [
        BMWCoils(base_addr + (i * mem_size), self)
        for i in range(count)]
