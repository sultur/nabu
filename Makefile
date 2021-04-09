SRC_FOLDER = src/
MAIN_FILE = $(SRC_FOLDER)nabu.cpp
AUXILIARY_FILES = $(SRC_FOLDER)tui.cpp $(SRC_FOLDER)cli.cpp $(SRC_FOLDER)nabu_types.cpp $(SRC_FOLDER)shared_functionality.cpp
LIBRARY_FLAGS = -lboost_system -lboost_filesystem -lmagic
CFLAGS = -I./single_include/ -std=c++11 $(LIBRARY_FLAGS)

ifeq ($(PREFIX),)
    PREFIX := ${HOME}/.local
endif

ifeq ($(MANPREFIX),)
    MANPREFIX := ${PREFIX}/share/man
endif

compile :
	g++ $(MAIN_FILE) $(AUXILIARY_FILES) $(CFLAGS) -o nabu

clean :
	[ -f nabu ] && rm nabu

man :
	pandoc manpage/nabu.1.md -s -t man -o manpage/nabu.1
	install -d ${MANPREFIX}/man1
	cp manpage/nabu.1 ${MANPREFIX}/man1/

install : nabu manpage/nabu.1
	install -d ${PREFIX}/bin/
	install -m 755 nabu ${PREFIX}/bin/
	install -d ${MANPREFIX}/man1
	install -m 644 manpage/nabu.1 ${MANPREFIX}/man1/
	@echo -e "\nnabu installed at ${PREFIX}/bin"
	@echo -e "Man page installed at ${MANPREFIX}/man1/"
	@echo "Please add it to your MANPATH variable if it isn't there already."
	
uninstall :
	rm -f ${PREFIX}/bin/nabu\
		${MANPREFIX}/man1/nabu.1

