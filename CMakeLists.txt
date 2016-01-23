cmake_minimum_required(VERSION 2.8)
project("monster-rpg-3" CXX)

option(DEMO "Build demo version" OFF)

include_directories("include")

set(USER_INCLUDE_PATH "" CACHE FILEPATH "Any extra include directories you want")
set(USER_LIBRARY_PATH "" CACHE FILEPATH "Any extra library directories you want")
set(USER_FRAMEWORK_PATH "" CACHE FILEPATH "An extra OSX framework directory")

include_directories(${USER_INCLUDE_PATH})
link_directories(${USER_LIBRARY_PATH})

set(SRC
	src/brain_actions.cpp
	src/brains.cpp
	src/dll.cpp
	src/gui.cpp
	src/ml_start.cpp
	src/ml_start_upper.cpp
	src/ml_cabbagetown.cpp
	src/monster-rpg-3.cpp
	src/quest.cpp
	src/tweens.cpp
	src/widgets.cpp
)

if(WIN32)
	if(DEMO)
		if(MSVC)
			set(FLAGS "${FLAGS} /DDEMO")
		else()
			set(FLAGS "${FLAGS} -DDEMO")
		endif()
	endif()
else()
	if(DEMO)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEMO")
	endif()
endif()

if(WIN32)
	if(MSVC)
		add_library(MonsterRPG3 SHARED ${SRC})
		set(FLAGS "${FLAGS} /MP /D_CRT_SECURE_NO_WARNINGS /D_WIN32_WINNT=0x501 /DNOOSKEWL_ENGINE_WINDOWS /DMONSTERRPG3_WINDOWS /DWITH_SDL")
		set_target_properties(MonsterRPG3 PROPERTIES COMPILE_FLAGS "${FLAGS}")
		target_link_libraries(MonsterRPG3 "tgui3" "Nooskewl_Engine" "SDL2")
	else()
		add_library(MonsterRPG3 SHARED ${SRC})
		set(FLAGS "${FLAGS} -DNOOSKEWL_ENGINE_WINDOWS -DMONSTERRPG3_WINDOWS -DWITH_SDL")
		set_target_properties(MonsterRPG3 PROPERTIES COMPILE_FLAGS "${FLAGS}")
		set_target_properties(MonsterRPG3 PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
		set_target_properties(MonsterRPG3 PROPERTIES PREFIX "")
		target_link_libraries(MonsterRPG3 "tgui3" "Nooskewl_Engine" "SDL2")
	endif()
elseif(APPLE)
	add_library(game SHARED ${SRC})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWITH_SDL")
	set_target_properties(game PROPERTIES MACOSX_RPATH on)
	set_target_properties(game PROPERTIES BUILD_WITH_INSTALL_RPATH on)
	set_target_properties(game PROPERTIES INSTALL_RPATH "@executable_path/")
	target_link_libraries(game "tgui3" "Nooskewl_Engine")
else()
	add_library(MonsterRPG3 SHARED ${SRC})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWITH_SDL")
	target_link_libraries(MonsterRPG3 "tgui3" "Nooskewl_Engine" "SDL2")
endif()
