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
            // TODO
        }
    }
    else if (notes.is_object())
    {
        // TODO
    }
}

/* Use libmagic in order to determine file type and categorize
 * as either Image, PDF or Text.
 */
NoteType get_notetype(string filename)
{
    string magic_full;
    magic_t magic_cookie;

    /* MAGIC_MIME tells magic to return a mime of the file,
       but you can specify different things	*/
    magic_cookie = magic_open(MAGIC_MIME);

    if (magic_cookie == NULL)
    {
        cout << "Can't initialize magic database." << endl;
        exit(1);
    }

    if (magic_load(magic_cookie, NULL) != 0)
    {
        magic_close(magic_cookie);
        cout << "Can't load magic database." << endl;
        exit(1);
    }

    magic_full = magic_file(magic_cookie, filename.c_str());
    magic_close(magic_cookie);

    // Deduce file type and categorize as either Image, PDF or Text (default)
    if (magic_full.find("image") != string::npos)
    {
        return Image;
    }
    if (magic_full.find("pdf") != string::npos)
    {
        return PDF;
    }
    return Text;
}

/* Extract tags and category from an argument vector.
 */
pair<vector<string>, bfs::path> extract_tags_and_category(vector<string> args)
{
    vector<string> tags;
    bfs::path category;

    // Extract tags and category (folder path)
    bool is_tag = false, is_category = false;
    for (string s : args)
    {
        if (s.compare("..") == 0)
        {
            // Ignore '..' as category
            continue;
        }
        else if (s.compare("-c") == 0)
        {
            is_category = true;
            is_tag = false;
            continue;
        }
        else if (s.compare("-t") == 0)
        {
            is_tag = true;
            is_category = false;
            continue;
        }

        if (is_tag)
        {
            tags.push_back(s);
        }
        if (is_category)
        {
            category /= s;
        }
    }

    return make_pair(tags, category);
}

json create_note(vector<string> args, string root, string editor)
{
    string filename = args[0];
    args.erase(args.begin());

    pair<vector<string>, bfs::path> tag_category_pair = extract_tags_and_category(args);
    vector<string> tags = tag_category_pair.first;
    bfs::path category = tag_category_pair.second;

    bfs::path filepath(root);
    filepath /= category.native();
    filepath /= filename;

    if (bfs::exists(filepath))
    {
        cout << "File already exists! Do you want to overwrite it? (y/N) ";
        string ans;
        getline(cin, ans);
        if (!ans.compare("y") == 0 && !ans.compare("Y") == 0)
        {
            exit(0);
        }
    }

    // Create category folders if they don't exist
    if (!bfs::exists(filepath.parent_path()))
    {
        bfs::create_directories(filepath.parent_path());
    }

    // Construct editor command
    string command = editor + " " + filepath.native();
    system(command.c_str()); // Call editor

    if (!bfs::exists(filepath))
    {
        cout << "Cancelled" << endl;
        exit(0);
    }

    NoteType type = get_notetype(filepath.string());

    // Return metadata for new note
    return Note(filepath.filename().string(), category.string(), type, tags).get_metadata();
}

void list_notes(json metadata, std::vector<std::string> args)
{
    // TODO interpret arguments (-and/-or)

    print_note_metadata(metadata);
}

void list_categories()
{
    //TODO
}

void list_tags(json metadata)
{
    set<string> tagset;
    //TODO

}

json edit_note(json note_data, std::vector<std::string> args)
{
    //TODO
    return note_data;
}

void delete_note(json note_data)
{
    //TODO
}
