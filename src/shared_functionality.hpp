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

void print_note_metadata(std::vector<std::pair<int, nlohmann::json>>, std::string);
NoteType get_notetype(std::string);
std::pair<std::pair<bool, std::set<std::string>>, std::pair<bool, bfs::path>> extract_tags_and_category(std::vector<std::string>);
std::vector<std::pair<int, nlohmann::json>> interpret_list_specification(std::vector<std::pair<int, nlohmann::json>>, std::vector<std::string> args);

std::pair<nlohmann::json, bool> create_note(std::string, bfs::path, std::set<std::string>, std::string, std::string);
void list_notes(nlohmann::json, std::vector<std::string>, std::string);
void list_categories(std::string);
void list_tags(nlohmann::json);
std::pair<nlohmann::json, bool> edit_note(nlohmann::json, std::pair<bool, std::set<std::string>>, std::pair<bool, bfs::path>, std::string, std::string);
void delete_note(nlohmann::json, std::string);

nlohmann::json delete_duplicate(nlohmann::json, nlohmann::json, int=-1);
void clean_directory_structure(std::string);

void replace_all(std::string &, const std::string &, const std::string &);

#endif // NABU_SHARED_FUNCS
