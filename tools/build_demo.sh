#!/bin/bash

ROOT=$(pwd)
TGUI3_DIR=${ROOT}/tgui3
NE_DIR=${ROOT}/Nooskewl_Engine
MO3_DIR=${ROOT}/monster-rpg-3
DATA_DIR=${ROOT}/monster-rpg-3-demo-data
TARGET_DIR=${ROOT}/mo3

# Clone TGUI3 if its directory doesn't already exist.
if [ ! -d "tgui3" ]; then

  echo "### Cloning TGUI3 ###"
  git clone https://github.com/Nooskewl/tgui3.git
fi

# Clone Nooskewl Engine if its directory doesn't already exist.
if [ ! -d "Nooskewl_Engine" ]; then

  echo "### Cloning Nooskewl Engine ###"
  git clone https://github.com/Nooskewl/Nooskewl_Engine.git
fi

# Clone Monster RPG 3 if its directory doesn't already exist.
if [ ! -d "monster-rpg-3" ]; then

  echo "### Cloning Monster RPG 3 ###"
  git clone https://github.com/Nooskewl/monster-rpg-3.git
fi

# Clone Monster RPG 3 data if its directory doesn't already exist.
if [ ! -d "monster-rpg-3-demo-data" ]; then

  echo "### Cloning Monster RPG 3 data ###"
  git clone https://github.com/Nooskewl/monster-rpg-3-demo-data.git
fi

echo "### Building TGUI3 ###"
pushd ${TGUI3_DIR}
git pull --rebase
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make $*
popd

echo "### Building Nooskewl Engine ###"
pushd ${NE_DIR}
git pull --rebase
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DUSER_INCLUDE_PATH="${TGUI3_DIR}/include" -DUSER_LIBRARY_PATH="${TGUI3_DIR}/build"
make $*
popd

echo "### Building Monster RPG 3 ###"
pushd ${MO3_DIR}
git pull --rebase
mkdir -p build && cd build
cmake .. -DDEMO=on -DCMAKE_BUILD_TYPE=Release -DUSER_INCLUDE_PATH="${TGUI3_DIR}/include;${NE_DIR}/include" -DUSER_LIBRARY_PATH="${TGUI3_DIR}/build;${NE_DIR}/build"
make $*
popd

echo "### Building data archive ###"
pushd ${DATA_DIR}
git pull --rebase
rm -f MonsterRPG3.cpa*
${NE_DIR}/tools/cpa/mkcpa.sh MonsterRPG3.cpa
popd

echo "### Copying binaries to target dir ###"
mkdir -p ${TARGET_DIR}
mkdir -p ${TARGET_DIR}/data
cp -f ${TGUI3_DIR}/build/libtgui3.so \
      ${NE_DIR}/build/{Nooskewl_Runner,libNooskewl_Engine.so} \
      ${MO3_DIR}/build/libMonsterRPG3.so \
      ${DATA_DIR}/MonsterRPG3.cpa \
      ${TARGET_DIR}/data
cp -f ${MO3_DIR}/icon.png ${MO3_DIR}/tools/MonsterRPG3 ${TARGET_DIR}
