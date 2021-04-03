
CFLAGS = -I./single_include/ -std=c++11
SRC_FOLDER = src/

all : clean nabu

nabu :
	g++ $(SRC_FOLDER)nabu.cpp $(CFLAGS) -o nabu

clean :
	[ -f nabu ] && rm nabu

