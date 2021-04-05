#ifndef NABU_CONFIG
#define NABU_CONFIG

#include "src/shared_functionality.hpp"

/* Config variables */
// note: for getenv() to work, the environment variable must be set

// Provide absolute path for each variable
static const std::string home_folder = getenv("HOME"); // Expand '~'
static const std::string NABU_ROOT_PATH = home_folder + "/nabu"; // Root folder for note library

static const std::string NOTE_EDITOR = getenv("EDITOR"); // Program to write notes/memos. Will be called with file name as argument

static const std::string NOTE_READER = "/bin/cat";    // Program to read notes
static const std::string PDF_VIEWER = "/usr/bin/zathura"; // Program to open PDF files
static const std::string IMG_VIEWER = "/usr/bin/sxiv -a"; // Program to open images

#endif
