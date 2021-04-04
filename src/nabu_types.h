#ifndef NABU_TYPES
#define NABU_TYPES

#include "config.h"

class Note
{
public:
    std::string filename;
    std::string description;
    NoteType type;

    Note(std::string, std::string, NoteType, std::vector<std::string>, std::vector<std::string>);
    nlohmann::json get_metadata();

    std::vector<std::string> tags;
    std::string category;
};

#endif
