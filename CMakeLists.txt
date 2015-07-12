cmake_minimum_required(VERSION 2.8)
project(MonsterRPG3 CXX)

option(TRENTS_BUILD "Build my way" off)

include_directories("include")

set(USER_INCLUDE_PATH "" CACHE FILEPATH "Any extra include directories you want")
set(USER_LIBRARY_PATH "" CACHE FILEPATH "Any extra library directories you want")
set(USER_FRAMEWORK_PATH "" CACHE FILEPATH "An extra OSX framework directory")

include_directories(${USER_INCLUDE_PATH})
link_directories(${USER_LIBRARY_PATH})

set(SRC
	src/dll.cpp
	src/ml_test.cpp
	src/ml_test2.cpp
	src/tweens.cpp
)

add_library(MonsterRPG3 SHARED ${SRC})

if(WIN32)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /D_CRT_SECURE_NO_WARNINGS /D_WIN32_WINNT=0x501 /DTGUI_WINDOWS /DNOOSKEWL_ENGINE_WINDOWS /DMONSTERRPG3_WINDOWS /DWITH_SDL")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D_CRT_SECURE_NO_WARNINGS /D_WIN32_WINNT=0x501 /DTGUI_DEBUG /DTGUI_WINDOWS /DNOOSKEWL_ENGINE_WINDOWS /DMONSTERRPG3_WINDOWS /DWITH_SDL")
	if(TRENTS_BUILD)
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /DTRENTS_BUILD")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DTRENTS_BUILD")
	endif()
	target_link_libraries(MonsterRPG3 "Nooskewl_Engine" "SDL2")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWITH_SDL")
	if(TRENTS_BUILD)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTRENTS_BUILD")
	endif()
	if(APPLE)
		set_target_properties(MonsterRPG3 PROPERTIES MACOSX_RPATH on)
		if(NOT TRENTS_BUILD)
			include_directories("/Library/Frameworks/SDL2.framework/Headers")
			include_directories("/Library/Frameworks/SDL2_ttf.framework/Headers")
		endif()
		if ("${USER_FRAMEWORK_PATH}" STREQUAL "")
			set_target_properties(MonsterRPG3 PROPERTIES LINK_FLAGS "-framework SDL2")
		else()
			set_target_properties(MonsterRPG3 PROPERTIES LINK_FLAGS "-F${USER_FRAMEWORK_PATH} -framework SDL2")
		endif()
		target_link_libraries(MonsterRPG3 "Nooskewl_Engine")
	else()
		target_link_libraries(MonsterRPG3 "Nooskewl_Engine" "SDL2")
	endif()
endif()
