#include "nabu_types.hpp"

using json = nlohmann::json;
using namespace std;

Note::Note(string filename, string folder_path, NoteType type, vector<string> tags)
{
    this->filename = filename;
    this->path = folder_path;
    this->type = type;
    this->tags = tags;
}

Note::Note(json j)
{
    this->filename = j["file"].get<string>();
    this->type = j["type"].get<NoteType>();
    this->tags = j["tags"].get<vector<string>>();
    this->path = j["category"].get<string>();
}

json Note::get_metadata()
{
    json j;

    j["file"] = this->filename;
    j["type"] = this->type;
    j["tags"] = this->tags;     // List of strings
    j["category"] = this->path; // Path in folder structure (Category)

    return j;
}
