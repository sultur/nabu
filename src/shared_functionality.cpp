#include "shared_functionality.hpp"
#include "nabu_types.hpp"

using json = nlohmann::json;
using namespace std;

void print_note_metadata(json notes)
{
    if (notes.is_array())
    {
        for (auto &note : notes)
        {
            cout << note << '\n';
        }
    }
}

string get_filetype(string filename)
{
    string magic_full;
    magic_t magic_cookie;

    /* MAGIC_MIME tells magic to return a mime of the file,
       but you can specify different things	*/
    magic_cookie = magic_open(MAGIC_MIME);

    if (magic_cookie == NULL)
    {
        return "";
    }

    if (magic_load(magic_cookie, NULL) != 0)
    {
        magic_close(magic_cookie);
        return "";
    }

    magic_full = magic_file(magic_cookie, filename.c_str());
    magic_close(magic_cookie);
    return magic_full;
}

void create_note(int id, std::string file, std::string desc, NoteType type, std::vector<std::string> tags, std::vector<std::string> categories)
{
    bfs::path p;
    string folder_path = "";
    for (string c : categories)
    {
        folder_path += bfs::path::preferred_separator;
        folder_path += c;
    }

    Note note(id, file, folder_path, desc, type, tags);
    switch (type)
    {
    case Text:
        break;
    case Image:
        /*
    image/gif; charset=binary
    image/jpeg; charset=binary
    image/png; charset=binary
    image/svg+xml; charset=us-ascii
    */
        break;
    case PDF:
        // application/pdf; charset=binary
        break;
    }
}

void list_notes(std::vector<std::string> args)
{
}
