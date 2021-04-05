#ifndef NABU_SHARED_FUNCS
#define NABU_SHARED_FUNCS

#include <stdlib.h>
#include <magic.h> // For deducing file types
#include <iostream>
#include <vector>
#include <set>
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
std::pair<std::vector<std::string>, bfs::path> extract_tags_and_category(std::vector<std::string>);
nlohmann::json create_note(std::vector<std::string>, std::string, std::string);
void list_notes(nlohmann::json, std::vector<std::string>);
void list_categories();
void list_tags(json);
nlohmann::json edit_note(nlohmann::json, std::vector<std::string>);
void delete_note(nlohmann::json);

NoteType get_notetype(std::string);

#endif // NABU_SHARED_FUNCS
