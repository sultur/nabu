#ifndef NABU_CONFIG
#define NABU_CONFIG

#include "src/shared_functionality.hpp"

/* Config variables */
// note: for getenv() to work, the environment variable must be set

static const bfs::path NABU_ROOT_PATH(strcat(getenv("HOME"), "/nabu/")); // Root folder for note library

static const std::vector<std::string> NOTE_EDITOR = {getenv("EDITOR")}; // Program to write notes/memos. Will be called with file name as argument

static const std::vector<std::string> NOTE_READER = {"cat"};       // Program to read notes
static const std::vector<std::string> PDF_VIEWER = {"zathura"};    // Program to open PDF files
static const std::vector<std::string> IMG_VIEWER = {"sxiv", "-a"}; // Program to open images

#endif
