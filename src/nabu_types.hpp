#ifndef NABU_TYPES
#define NABU_TYPES

#include "shared_functionality.hpp"

class Note
{
public:
    std::string filename;
    std::string path;
    NoteType type;

    Note(std::string, std::string, NoteType, std::set<std::string>);
    Note(nlohmann::json);
    nlohmann::json get_metadata();
    bool fits_specification(std::vector<std::pair<std::set<std::string>, bfs::path>>);

    std::set<std::string> tags;
};

#endif
