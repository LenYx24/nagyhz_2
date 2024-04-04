CXX = g++
OBJECT = lolmacrogame
FILES = src/*.cpp
main : 
	$(CXX) $(FILES) -o ./bin/$(OBJECT)

clean :
	rm ./bin/*
