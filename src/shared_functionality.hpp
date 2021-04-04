#ifndef NABU_SHARED_FUNCS
#define NABU_SHARED_FUNCS

#include <stdlib.h>
#include <magic.h> // For deducing file types
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>
namespace bfs = boost::filesystem;
#include <nlohmann/json.hpp>

using namespace std;

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

void print_note_metadata(nlohmann::json notes);

void create_note(int, std::string, std::string, NoteType, std::vector<std::string>, std::vector<std::string>);
void list_notes(std::vector<std::string>);
void edit_note();
void delete_note();

std::string get_filetype(std::string);

#endif // NABU_SHARED_FUNCS
