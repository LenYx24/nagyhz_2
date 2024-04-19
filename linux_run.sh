var="Debug"
if [ "$#" -ne 0 ]; then
  var=$1
  echo $1
fi
cmake -S . -B ./linuxbuild -DCMAKE_BUILD_TYPE=$var
cmake --build ./linuxbuild
