#ifndef NABU_CONFIG
#define NABU_CONFIG

#include <stdlib.h>
#include <vector>
#include <string>

static const std::vector<std::string> NABU_ROOT_PATH = {getenv("HOME"), "/nabu/"}; // Root folder for note library (make sure it has trailing slash)
static const std::vector<std::string> NOTE_READER    = {"cat"};                    // Program to read notes
static const std::vector<std::string> PDF_VIEWER     = {"zathura"};                // Program to open PDF files
static const std::vector<std::string> IMG_VIEWER     = {"sxiv", "-a"};             // Program to open images


#endif
