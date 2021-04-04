
CFLAGS = -I./single_include/ -std=c++11
BOOST = -lboost_system -lboost_filesystem
SRC_FOLDER = src/
MAIN_FILE = $(SRC_FOLDER)nabu.cpp
AUXILIARY_FILES = $(SRC_FOLDER)tui.cpp $(SRC_FOLDER)cli.cpp

compile :
	g++ $(MAIN_FILE) $(AUXILIARY_FILES) $(CFLAGS) $(BOOST) -o nabu

clean :
	[ -f nabu ] && rm nabu

