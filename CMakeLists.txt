CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

SET(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

PROJECT(Containers LANGUAGES ASM_NASM CXX)

#INCLUDE_DIRECTORIES(headers)

SET(APP_CXX_SOURCES sources/ios.cpp
                    sources/iostream.cpp
		    sources/new.cpp
		    sources/string_view.cpp
		    sources/main.cpp
		    sources/__abi.cpp)

SET_SOURCE_FILES_PROPERTIES(
    ${APP_CXX_SOURCES}
    PROPERTIES
    COMPILE_OPTIONS "-nostdinc;-nostdinc++;-march=x86-64;-ffreestanding"
    INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/headers")

ADD_EXECUTABLE(app ${APP_CXX_SOURCES} sources/start.asm)

TARGET_LINK_OPTIONS(app PRIVATE -nostdlib -nostdlib++)

ADD_EXECUTABLE(orch sources/orch.cpp)

ADD_CUSTOM_TARGET(Caps ALL sudo setcap 'cap_sys_admin=ep cap_setfcap=ep' orch)
