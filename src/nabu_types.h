#ifndef NABU_TYPES
#define NABU_TYPES

class Note
{
public:
    std::string title;
    std::string filename;

    Note();
    void set_title(std::string new_title);
    void set_filename(std::string new_filename);
    std::string to_json();

    std::vector<std::string> tags;
    std::vector<std::string> categories;
};






#endif
