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
        if (s.compare("..") == 0 || s.compare(".") == 0)
        {
            // Ignore '..' and '.' as categories
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

    // Create category folders if they don't exist
    if (!bfs::exists(filepath.parent_path()))
    {
        bfs::create_directories(filepath.parent_path());
    }
    else if (bfs::exists(filepath))
    {
        cout << "File already exists! Do you want to overwrite it? (y/N) ";
        string ans;
        getline(cin, ans);
        if (!ans.compare("y") == 0 && !ans.compare("Y") == 0)
        {
            exit(0);
        }
        ofstream outputfile(filepath.c_str(), ios::trunc);
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
    if (args.size() > 0)
    {
        // TODO interpret arguments (-and/-or)
    }
    print_note_metadata(metadata);
}

void list_categories(string root)
{
    bfs::path nabu_root(root);
    for (auto dir = bfs::recursive_directory_iterator(nabu_root);
         dir != bfs::recursive_directory_iterator();
         ++dir)
    {
        if (bfs::is_directory(dir->path()))
        {
            // TODO maybe make output more visually appealing
            cout << string(dir.depth(), '.')
                 << string(dir.depth(), ' ')
                 << dir->path().filename().string()
                 << bfs::path::preferred_separator
                 << '\n';
        }
    }
}

void list_tags(json notes)
{
    // Gather all tags into set
    set<string> tagset;
    for (auto &note : notes)
    {
        vector<string> tags = note["tags"].get<vector<string>>();
        tagset.insert(tags.begin(), tags.end());
    }

    // Print tags surrounded by braces and separated by commas
    string output = "[";
    for (string tag : tagset)
    {
        output += tag + ", ";
    }

    cout << "Tags:\n"
         << output.substr(0, output.rfind(',')) << "]" << endl;
}

json edit_note(json note_data, std::vector<std::string> args, string root, string editor)
{
    // Check if user provided either '-t' or '-c' as an argument
    bool change_tags = false, change_category = false;
    for (string s : args)
    {
        if (s.compare("-t") == 0)
        {
            change_tags = true;
        }
        else if (s.compare("-c") == 0)
        {
            change_category = true;
        }
    }

    pair<vector<string>, bfs::path> tag_category_pair = extract_tags_and_category(args);
    vector<string> new_tags = tag_category_pair.first;
    bfs::path new_category = tag_category_pair.second;

    bfs::path filepath(root);
    filepath /= note_data["category"].get<string>();
    filepath /= note_data["file"].get<string>();

    if (bfs::exists(filepath))
    {
        // Change category
        if (change_category)
        {
            bfs::path new_path(root);
            new_path /= new_category;

            // Create new category if it doesn't already exist
            bfs::create_directories(new_path);

            new_path /= note_data["file"].get<string>();

            // If file with same name already exists in new location
            if (bfs::exists(new_path))
            {
                cout << "File with same name already exists in specified category! Do you want to overwrite it? (y/N) ";
                string ans;
                getline(cin, ans);
                if (!ans.compare("y") == 0 && !ans.compare("Y") == 0)
                {
                    exit(0);
                }
            }

            // Move file to new category
            bfs::copy_file(filepath, new_path, bfs::copy_options::overwrite_existing);
            bfs::remove_all(filepath);
            cout << "Note moved from " << note_data["category"].get<string>() << " to " << new_category.string() << ".\n";
            filepath = new_path;
        }

        // Edit note content
        string command = editor + " " + filepath.string();
        system(command.c_str());

        // Apply new tags
        if (change_tags)
        {
            note_data["tags"] = new_tags;
            cout << "New tags applied\n";
        }
    }
    else
    {
        cout << "Note wasn't found." << endl;
        exit(1);
    }

    return note_data;
}

void delete_note(json note_data, string root)
{
    //TODO
}
