#ifndef NABU_TYPES
#define NABU_TYPES

#include "shared_functionality.hpp"

class Note
{
public:
    int id;
    std::string filename;
    std::string path;
    std::string description;
    NoteType type;

    Note(int, std::string, std::string, std::string, NoteType, std::vector<std::string>);
    nlohmann::json get_metadata();

    std::vector<std::string> tags;
};

#endif
