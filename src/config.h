#ifndef NABU_CONFIG
#define NABU_CONFIG

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>
namespace bfs = boost::filesystem;

#include <nlohmann/json.hpp>

/* Config variables */

static const bfs::path NABU_ROOT_PATH(strcat(getenv("HOME"), "/nabu/")); // Root folder for note library (note: for getenv() to work, environment variable in argument must be set)

static const std::vector<std::string> NOTE_EDITOR = {getenv("EDITOR")}; // Program to write notes/memos. Will be called with filename as argument

static const std::vector<std::string> NOTE_READER = {"cat"};       // Program to read notes
static const std::vector<std::string> PDF_VIEWER = {"zathura"};    // Program to open PDF files
static const std::vector<std::string> IMG_VIEWER = {"sxiv", "-a"}; // Program to open images

/*------------------*/

enum NoteType
{
    Text,
    Image,
    PDF
};

// Map NoteType values to JSON as strings
NLOHMANN_JSON_SERIALIZE_ENUM(
    NoteType,
    {{Text, "text"},
     {Image, "img"},
     {PDF, "pdf"}})

#endif
