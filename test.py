import ctypes

libname = "./lib/libexample.so"
mylib = ctypes.CDLL(libname)

result = mylib.add(3, 4)
print("The result is:", result)
