from enum import IntEnum
from typing import Union
from pathlib import Path
import ctypes
import os
import Bindings.ThQueue as Msg
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
class BMWMsgType(IntEnum):
    REQUEST = 0
    COMMAND = 1
class BMWSubSys(IntEnum):
    Freq = 0
    Ampl = 1
    Cycl = 2
    Wave = 3
    Trig = 4
    Stat = 5
    Erro = 6
class BMWMsg(ctypes.Structure):
    _fields_ = [("coil"    , ctypes.c_void_p),
                ("msg_type", ctypes.c_int),
                ("msg_sys" , ctypes.c_int),
                ("content" , Msg.Message)]

CreateListOfCoils = wrap_function(BMWLib, "CreateListOfHallACoils", OpaquePointer  , [ctypes.POINTER(ctypes.c_size_t)])
DeleteListOfCoils = wrap_function(BMWLib, "DestroyListOfCoils"    , None           , [OpaquePointer, ctypes.c_size_t] )
GetCoilMemSize    = wrap_function(BMWLib, "GetCoilMemSize"        , ctypes.c_size_t, None                             )

SetCoilAmplitude  = wrap_function(BMWLib, "SetBMWAmplitude"      , ctypes.c_bool  , [OpaquePointer, ctypes.c_double] )
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
BeamModShutdown   = wrap_function(BMWLib, "BMWShutdown"           , None           , None                             )

GetReceiver       = wrap_function(BMWLib, "GetBMWReceiver"        , OpaquePointer  , None                             )
GetSender         = wrap_function(BMWLib, "GetBMWSender"          , OpaquePointer  , None                             )
CheckForMsg       = wrap_function(BMWLib, "CheckForMsg"           , ctypes.c_bool  , [OpaquePointer, ctypes.POINTER(BMWMsg)])
SendMsg           = wrap_function(BMWLib, "SendMsg"               , None           , [OpaquePointer, ctypes.POINTER(BMWMsg)])
CreateMsg         = wrap_function(BMWLib, "CreateBMWMessage"      , BMWMsg         , [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, Msg.Message])
DeleteMsg         = wrap_function(BMWLib, "DeleteBMWMessage"      , None           , [ctypes.POINTER(BMWMsg)]         )

class BMWMsgSender(OpaquePointer):
    def __init__(self):
        self._sender = GetSender()
    def Send(self, msg: BMWMsg):
        SendMsg(self._sender, ctypes.byref(msg))
        return
        
class BMWMsgReceiver(OpaquePointer):
    def __init__(self):
        self._receiver = GetReceiver()
    def CheckForMsg(self):
        msg = BMWMsg()
        new_message_flag = CheckForMsg(self._receiver, ctypes.byref(msg))
        return new_message_flag, msg


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
