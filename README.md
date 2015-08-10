# Build instructions

The tools/build.sh script can be used to download and build everything, if you have access to the monster-rpg-3-data repo which is for early access backers only. The build instructions below can be used otherwise, but you'll need to get a datafile somewhere.

# Build instructions (Ubuntu 14.04)

## Prerequisites

The following packages need to be installed: `libsdl2-dev libsdl2-ttf-dev libglm-dev libglew-dev cmake g++`

## Getting the code

```
git clone https://github.com/Nooskewl/tgui3.git
git clone https://github.com/Nooskewl/Nooskewl_Engine.git
git clone https://github.com/Nooskewl/monster-rpg-3.git
```

## Building TGUI3

```
cd tgui3
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

## Building Nooskewl Engine

```
cd Nooskewl_Engine
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DUSER_INCLUDE_PATH=/path/to/tgui3/include -DUSER_LIBRARY_PATH=/path/to/tgui3/build
make
```

## Building Monster RPG 3

```
cd monster-rpg-3
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DUSER_INCLUDE_PATH="/path/to/tgui3/include;/path/to/Nooskewl_Engine/include" -DUSER_LIBRARY_PATH="/path/to/tgui3/build;/path/to/Nooskewl_Engine/build"
make
```

## Copy files

```
mkdir mo3
cp /path/to/tgui3/build/libtgui3.so /path/to/Nooskewl_Engine/build/libNooskewl_Engine.so /path/to/Nooskewl_Engine/build/Nooskewl_Runner /path/to/monster-rpg-3/build/libMonsterRPG3.so mo3
cp /path/to/MonsterRPG3.cpa mo3
```

## Running the game

```
cd mo3
./Nooskewl_Runner
```
