#!/bin/sh

cd tgui3
git pull --rebase
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_DEPLOYMENT_TARGET=10.7
make $*
~/bin/codesign.sh libtgui3.dylib
cp libtgui3.dylib ~/code/mo3/"Monster RPG 3.app"/Contents/MacOS

cd ../../Nooskewl_Engine
git pull --rebase
cd build
cmake .. -DLOAD_FROM_FILESYSTEM=off -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_DEPLOYMENT_TARGET=10.7
make $*
~/bin/codesign.sh libNooskewl_Engine.dylib
~/bin/codesign.sh Nooskewl_Runner
cp libNooskewl_Engine.dylib ~/code/mo3/"Monster RPG 3.app"/Contents/MacOS
cp Nooskewl_Runner ~/code/mo3/"Monster RPG 3.app"/Contents/MacOS

cd ../../monster-rpg-3
git pull --rebase
cd build
cmake .. -DDEMO=off -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_DEPLOYMENT_TARGET=10.7
make $*
~/bin/codesign.sh libgame.dylib
cp libgame.dylib ~/code/mo3/"Monster RPG 3.app"/Contents/MacOS

cd ../../monster-rpg-3-data
git pull --rebase
../Nooskewl_Engine/tools/cpa/mkcpa-osx.sh ../mo3/data.cpa

cd ../mo3
rm data.cpa.uncompressed
mv data.cpa "Monster RPG 3.app"/Contents/Resources
