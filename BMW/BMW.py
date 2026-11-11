from enum import IntEnum
from typing import Union
from pathlib import Path
import ctypes
import os
_dir = Path(__file__).parent.resolve()
_lib_name = "libRemexClient.so"
RemexClientLib = ctypes.CDLL(str(_dir / _lib_name))

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
class CoilType(IntEnum):
	Coil   = 0
	Energy = 1
    Empty  = 2

class MessageType(IntEnum):
		Status   = 0
		Cycles   = 1
		SineWave = 2
		RampWave = 3
		Ampl     = 4
		Freq     = 5
		Error    = 6

class Message(ctypes.Structure):
    _fields_ = [("id"  , ctypes.c_int),
                ("type", ctypes.c_int ),
                ("contents", ctypes.c_void_p)]
    def status(self):
        return MessageType(self.type)
    def __repr__(self):
        return '(msg received id: {0}{1})'.format(self.success, self.values)

