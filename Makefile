all: nms.hpp
	g++ nms.cpp utils.cpp example.cpp `pkg-config opencv --cflags` `pkg-config opencv --libs` -std=c++11 -o example

run:
	./example

clean:
	rm -rf example
