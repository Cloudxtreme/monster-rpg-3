Build instructions (Ubuntu 14.04):

* Install packages libsdl2-dev libsdl2-ttf-dev libglm-dev libglew-dev cmake g++
* git clone https://github.com/Nooskewl/tgui3.git
* git clone https://github.com/Nooskewl/Nooskewl_Engine.git
* git clone https://github.com/Nooskewl/monster-rpg-3.git
* cd tgui3 && mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && make
* cd Nooskewl_Engine && mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release -DUSER_INCLUDE_PATH=/path/to/tgui3/include -DUSER_LIBRARY_PATH=/path/to/tgui3/build && make
* cd monster-rpg-2 && mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release -DUSER_INCLUDE_PATH="/path/to/tgui3/include;/path/to/Nooskewl_Engine/include" -DUSER_LIBRARY_PATH="/path/to/tgui3/build;/path/to/Nooskewl_Engine/build" && make
* cd ~ && mkdir mo3
* cp /path/to/tgui3/build/libtgui3.so /path/to/Nooskewl_Engine/build/libNooskewl_Engine.so /path/to/Nooskewl_Engine/build/Nooskewl_Runner mo3
* copy MonsterRPG3.cpa to mo3
* Now you can run ./Nooskewl_Runner and voila.
