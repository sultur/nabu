#include "nabu_types.hpp"

using json = nlohmann::json;
using namespace std;

Note::Note(string filename, string folder_path, NoteType type, set<string> tags)
{
    this->filename = filename;
    this->path = folder_path;
    this->type = type;
    this->tags = tags;
}

/* Initialize from JSON.
 */
Note::Note(json j)
{
    this->filename = j["file"].get<string>();
    this->type = j["type"].get<NoteType>();
    this->tags = j["tags"].get<set<string>>();
    this->path = j["category"].get<string>();
}

set<string> anyset({"-any"});
set<string> noneset({"-none"});

bool Note::fits_specification(vector<pair<set<string>, bfs::path>> spec)
{
    bool tag_fits, cat_fits;
    for (pair<set<string>, bfs::path> atom : spec)
    {
        tag_fits = false, cat_fits = false;
        // If note has correct category/path or includes "-any" but doesn't contain '-none'
        if ((atom.second.compare(this->path) == 0 || atom.second.string().find("-any") != string::npos) &&
            atom.second.string().find("-none") == string::npos)
        {
            cat_fits = true;
        }
        // If note includes tags or tags include '-any' but don't contain '-none'
        if ((includes(this->tags.begin(), this->tags.end(), atom.first.begin(), atom.first.end()) || includes(atom.first.begin(), atom.first.end(), anyset.begin(), anyset.end())) &&
            !includes(atom.first.begin(), atom.first.end(), noneset.begin(), noneset.end()))
        {
            tag_fits = true;
        }

        if (tag_fits && cat_fits)
        {
            return true;
        }
    }
    return false;
}

json Note::get_metadata()
{
    json j;

    j["file"] = this->filename;
    j["type"] = this->type;
    j["tags"] = this->tags;     // Set of strings
    j["category"] = this->path; // Path in folder structure (Category)

    return j;
}
