CXX = g++
OBJECT = lolmacrogame
FILES = src/*.cpp
main : 
	$(CXX) $(FILES) `sdl2-config --libs --cflags` -o ./bin/$(OBJECT)

run:
	./bin/$(OBJECT)
clean :
	rm ./bin/*
