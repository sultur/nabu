#ifndef NABU_TYPES
#define NABU_TYPES

#include "shared_functionality.hpp"

class Note
{
public:
    std::string filename;
    std::string path;
    NoteType type;

    Note(std::string, std::string, NoteType, std::vector<std::string>);
    Note(nlohmann::json);
    nlohmann::json get_metadata();

    std::vector<std::string> tags;
};

#endif
