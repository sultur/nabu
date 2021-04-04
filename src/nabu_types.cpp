#include "nabu_types.hpp"

using json = nlohmann::json;
using namespace std;

Note::Note(int id, string filename, string folder_path, string description, NoteType type, vector<string> tags)
{
    this->id = id;

    this->filename = filename;
    this->description = description;
    this->type = type;

    this->tags = tags;
    this->path = path;
}

json Note::get_metadata()
{
    json j;

    j["file"] = this->filename;
    j["desc"] = this->description;
    j["type"] = this->type;
    j["tags"] = this->tags;     // List of strings
    j["category"] = this->path; // Path in folder structure (Category)

    return j;
}
