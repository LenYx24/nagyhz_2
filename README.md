# The homework project for Basics of Programming 2 class

The game is a turn-based strategy game, it copies League of Legends and makes a simplified version of it, but with a twist.
One player controls all the champions of his team.

# Running
First install SFML.
## for ubuntu:
`sudo apt update &&
sudo apt install \
    libxrandr-dev \
    libxcursor-dev \
    libudev-dev \
    libfreetype-dev \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev`

git clone the project.

In the same folder, run:
For linux:  
`cmake -S . -B build -DCMAKE_BUILD_TYPE=Release &&  
cmake --build build`  
For windows:  
`cmake -S . -B build &&   
cmake --build build --config Release`  

This template was followed in the repo from the original SFML creators:
[link](https://github.com/SFML/cmake-sfml-project/tree/master)
