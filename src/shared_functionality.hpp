#ifndef NABU_SHARED_FUNCS
#define NABU_SHARED_FUNCS

#include <stdlib.h>
#include <magic.h> // For deducing file types
#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>
namespace bfs = boost::filesystem;
#include <nlohmann/json.hpp>

const std::string FAINT = "\033[2m";
const std::string BOLD = "\033[1m";
const std::string REGULAR = "\033[0m";

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

void print_note_metadata(nlohmann::json notes, std::string);
NoteType get_notetype(std::string);
std::pair<std::set<std::string>, bfs::path> extract_tags_and_category(std::vector<std::string>);
nlohmann::json interpret_list_specification(nlohmann::json, std::vector<std::string> args);

nlohmann::json create_note(std::vector<std::string>, std::string, std::string);
void list_notes(nlohmann::json, std::vector<std::string>, std::string);
void list_categories(std::string);
void list_tags(nlohmann::json);
nlohmann::json edit_note(nlohmann::json, std::vector<std::string>, std::string, std::string);
void delete_note(nlohmann::json, std::string);

#endif // NABU_SHARED_FUNCS
