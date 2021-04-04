#include "nabu_types.h"

using json = nlohmann::json;
using namespace std;

Note::Note(string filename, string description, NoteType type, vector<string> tags, vector<string> categories)
{
    this->filename = filename;
    this->description = description;
    this->type = type;

    this->tags = tags;
    this->category = "";
    for (string s : categories)
    {
        this->category += bfs::path::preferred_separator;
        this->category += s;
    }
}

json Note::get_metadata()
{
    json j;

    j["filename"] = this->filename;
    j["desc"] = this->description;
    j["type"] = this->type;
    j["tags"] = this->tags;         // List of strings
    j["category"] = this->category; // Path in folder structure

    return j;
}
