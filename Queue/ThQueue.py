from enum import IntEnum
from typing import Union
from pathlib import Path
import ctypes
import os
_dir = Path(__file__).parent.resolve()
_lib_name = "libThQueue.so"
ThQueueLib = ctypes.CDLL(str(_dir / _lib_name))

# https://dbader.org/blog/python-ctypes-tutorial-part-2
def wrap_function(lib, funcname, restype, argtypes):
    """Simplify wrapping ctypes functions"""
    func = lib.__getattr__(funcname)
    func.restype = restype
    func.argtypes = argtypes
    return func

class OpaquePointer(ctypes.c_void_p):
    pass

class MessageType(IntEnum):
    INT    = 0
    DOUBLE = 1
    STRING = 2
class MessagePayload(ctypes.Union):
    _fields_ = [("int_msg", ctypes.c_int),
                ("dbl_msg", ctypes.c_double),
                ("str_msg", ctypes.c_char_p)]
class Message(ctypes.Structure):
    _fields_ = [("msg_type", MessageType),
                ("msg"     , MessagePayload)]
CreateIntMsg = wrap_function(ThQueueLib, "CreateIntMsg", Message, ctypes.int     )
CreateDblMsg = wrap_function(ThQueueLib, "CreateDblMsg", Message, ctypes.double  )
CreateStrMsg = wrap_function(ThQueueLib, "CreateStrMsg", Message, ctypes.c_char_p)
DeleteMsg    = wrap_function(ThQueueLib, "DeleteMessage", None  , ctypes.POINTER(Message))
