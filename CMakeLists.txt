cmake_minimum_required(VERSION 2.8)
project(MonsterRPG3 CXX)

option(TRENTS_BUILD "Build my way" off)

include_directories("include")

set(USER_INCLUDE_PATH "" CACHE FILEPATH "Any extra include directories you want")
set(USER_LIBRARY_PATH "" CACHE FILEPATH "Any extra library directories you want")

include_directories(${USER_INCLUDE_PATH})
link_directories(${USER_LIBRARY_PATH})

if(WIN32)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /D_CRT_SECURE_NO_WARNINGS /D_WIN32_WINNT=0x501 /DWITH_SDL")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D_CRT_SECURE_NO_WARNINGS /D_WIN32_WINNT=0x501 /DWITH_SDL /DTGUI_DEBUG")
	if(TRENTS_BUILD)
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /DTRENTS_BUILD")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DTRENTS_BUILD")
	endif()
els()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWITH_SDL")
	if(TRENTS_BUILD)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTRENTS_BUILD")
	endif()
endif()

set(SRC
	src/dll.cpp
	src/ml_test.cpp
	src/ml_test2.cpp
	src/tweens.cpp
)

add_library(MonsterRPG3 SHARED ${SRC})
target_link_libraries(MonsterRPG3 "Nooskewl_Engine" "SDL2")
