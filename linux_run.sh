var="Debug"
if [ "$#" -ne 0 ]; then
  var=$1
  echo $1
fi
cmake -S . -B ./build_linux -DCMAKE_BUILD_TYPE=$var -G Ninja
cmake --build ./build_linux
