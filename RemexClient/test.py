from enum import IntEnum
import ctypes
import os
current_dir = os.getcwd()
mylib = ctypes.CDLL(os.path.join(current_dir, "myRemexLib.so"))

# https://dbader.org/blog/python-ctypes-tutorial-part-2
def wrap_function(lib, funcname, restype, argtypes):
    """Simplify wrapping ctypes functions"""
    func = lib.__getattr__(funcname)
    func.restype = restype
    func.argtypes = argtypes
    return func

class OpaquePointer(ctypes.c_void_p):
    pass

class RemexEnum(IntEnum):
    VOID   = 0
    FLOAT  = 1
    DOUBLE = 2
    INT    = 3
    SHORT  = 4
    UINT   = 5
    USHORT  = 6
    

class RemexValue(ctypes.Structure):
    _fields_ = [("float" , ctypes.c_float ),
                ("double", ctypes.c_double),
                ("int"   , ctypes.c_int   ),
                ("short" , ctypes.c_short ),
                ("uint"  , ctypes.c_uint  ),
                ("ushort", ctypes.c_ushort)]
    def __repr__(self):
        return '({0}, {1}, {2}, {3}, {4}, {5})'.format(self.float, self.double, self.int, self.short, self.uint, self.ushort)


class CMsgServer(ctypes.Structure):
    def __init__(self, lib, CMsghost, CMsgpass):
        self.CreateFunc  = wrap_function(lib, "CreateCMsgServer",  OpaquePointer, [ctypes.c_char_p, ctypes.c_char_p])
        self.CleanupFunc = wrap_function(lib, "DestoryCMsgServer", None, [OpaquePointer])
        self.Server = self.CreateFunc(CMsghost, CMsgpass)
        self.Host   = CMsghost
        self.Pass   = CMsgpass
        print(f"PYTHON: Creating a CMsgServer to {self.Host} (Pass = {self.Pass})")
    def __del__(self):
        print(f"PYTHON: Deleting CMsgServer")
        self.CleanupFunc(self.Server)

class RemexClient(ctypes.Structure):
    def __init__(self, lib, RemexHost, CMsgServer):
        self.CreateFunc = wrap_function(lib, "CreateClient", OpaquePointer, [ctypes.c_char_p, OpaquePointer])
        self.CleanupFunc= wrap_function(lib, "DestoryClient", None, [OpaquePointer])
        self.Send       = wrap_function(lib,"SendCommand"  , RemexValue, [OpaquePointer, ctypes.c_char_p, ctypes.c_int])
        self.Client     = self.CreateFunc(RemexHost, CMsgServer.Server)
        print(f"PYTHON: Creating Remex Client: Host = {RemexHost}")
    def __del__(self):
        print(f"PYTHON: Deleting Remex Client")
        self.CleanupFunc(self.Client)
    def send(self, cmd, remex_type):
        print(f"Sending Command = {cmd} and expecting return type {remex_type}")
        return self.Send(self.Client, cmd, remex_type)
        
        
        
#CMsg   = CMsgServer(mylib,None , bytes("MollerIntegrating", "utf-8"))
CMsg   = CMsgServer(mylib, bytes("adaq1.jlab.org", "utf-8"), bytes("MollerIntegrating", "utf-8"))
Client = RemexClient(mylib, bytes("ROC1", "utf-8"), CMsg);
Client.send(bytes("setSCANData(1,11)", "utf-8"), RemexEnum.VOID)
Client.send(bytes("setSCANData(2,12)", "utf-8"), RemexEnum.VOID)
Client.send(bytes("setSCANData(3,13)", "utf-8"), RemexEnum.VOID)
Client.send(bytes("setSCANData(4,140000)", "utf-8"), RemexEnum.VOID)

Received = Client.send(bytes("getSCANData(1)", "utf-8"), RemexEnum.INT)
print(f"Received = {Received}")
Received = Client.send(bytes("getSCANData(2)", "utf-8"), RemexEnum.INT)
print(f"Received = {Received}")
Received = Client.send(bytes("getSCANData(3)", "utf-8"), RemexEnum.INT)
print(f"Received = {Received}")
Received = Client.send(bytes("getSCANData(4)", "utf-8"), RemexEnum.INT)
print(f"Received = {Received}")

