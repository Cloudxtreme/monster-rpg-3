set(CMAKE_SYSTEM_NAME Windows)

set(HOST "i686-w64-mingw32")

SET(CMAKE_C_COMPILER ${HOST}-gcc CACHE PATH "gcc" FORCE)
SET(CMAKE_CXX_COMPILER ${HOST}-g++ CACHE PATH "gcc" FORCE)
set(CMAKE_AR ${HOST}-ar  CACHE PATH "archive" FORCE)
set(CMAKE_LINKER ${HOST}-ld  CACHE PATH "linker" FORCE)
set(CMAKE_NM ${HOST}-nm  CACHE PATH "nm" FORCE)
set(CMAKE_OBJCOPY ${HOST}-objcopy  CACHE PATH "objcopy" FORCE)
set(CMAKE_OBJDUMP ${HOST}-objdump  CACHE PATH "objdump" FORCE)
set(CMAKE_STRIP ${HOST}-strip  CACHE PATH "strip" FORCE)
set(CMAKE_RANLIB ${HOST}-ranlib  CACHE PATH "ranlib" FORCE)
