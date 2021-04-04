SRC_FOLDER = src/
MAIN_FILE = $(SRC_FOLDER)nabu.cpp
AUXILIARY_FILES = $(SRC_FOLDER)tui.cpp $(SRC_FOLDER)cli.cpp $(SRC_FOLDER)nabu_types.cpp $(SRC_FOLDER)shared_functionality.cpp
LIBRARY_FLAGS = -lboost_system -lboost_filesystem -lmagic
CFLAGS = -I./single_include/ -std=c++11 $(LIBRARY_FLAGS)

compile :
	g++ $(MAIN_FILE) $(AUXILIARY_FILES) $(CFLAGS) -o nabu

clean :
	[ -f nabu ] && rm nabu

