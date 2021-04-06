#include "shared_functionality.hpp"
#include "nabu_types.hpp"

using json = nlohmann::json;
using namespace std;

void print_note_metadata(json notes, string root = "")
{
    if (notes.is_array())
    {
        if (notes.size() == 0)
        {
            cout << "No notes match specification." << endl;
            return;
        }
        // Output columns:
        // ID, File, Type, Category, Tags, Abs. path
        vector<vector<string>> output_table = {{"ID", "File", "Type", "Category", "Tags", "Abs. path"}};
        vector<int> longest_strings = {2, 4, 4, 8, 4, 9};

        // Construct table
        for (int i = 0; i < notes.size(); i++)
        {
            string id = to_string(i + 1);
            longest_strings[0] = (id.length() > longest_strings[0]) ? id.length() : longest_strings[0];

            string file = notes[i]["file"].get<string>();
            longest_strings[1] = (file.length() > longest_strings[1]) ? file.length() : longest_strings[1];

            NoteType type = notes[i]["type"].get<NoteType>();
            string typestr;
            switch (type)
            {
            case Image:
                typestr = "Image";
                break;
            case PDF:
                typestr = "PDF";
                break;
            case Text:
                typestr = "Text";
                break;
            }
            longest_strings[2] = (typestr.length() > longest_strings[2]) ? typestr.length() : longest_strings[2];

            string category = notes[i]["category"].get<string>();
            longest_strings[3] = (category.length() > longest_strings[3]) ? category.length() : longest_strings[3];

            vector<string> tags = notes[i]["tags"].get<vector<string>>();
            string tagstr = "[";
            for (string tag : tags)
            {
                tagstr += tag + ",";
            }
            tagstr = tagstr.substr(0, tagstr.rfind(',')) + "]";
            longest_strings[4] = (tagstr.length() > longest_strings[4]) ? tagstr.length() : longest_strings[4];

            bfs::path abspath(root);
            abspath /= category;
            abspath /= file;
            string abstr = abspath.string();
            longest_strings[5] = (abstr.length() > longest_strings[5]) ? abstr.length() : longest_strings[5];

            vector<string> row = {id, file, typestr, category, tagstr, abstr};
            // Add row to output table
            output_table.push_back(row);
        }

        // Print table
        for (int i = 0; i < output_table.size(); i++)
        {
            if (i == 0)
            {
                cout << BOLD;
            }
            for (int l = 0; l < longest_strings.size(); l++)
            {
                cout << setw(longest_strings[l] + 1) << left << output_table[i][l];
            }
            if (i == 0)
            {
                cout << REGULAR << "\n"
                     << FAINT;
                for (int l : longest_strings)
                {
                    cout << string(l + 1, '-');
                }
                cout << REGULAR;
            }
            cout << "\n";
        }
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
pair<set<string>, bfs::path> extract_tags_and_category(vector<string> args)
{
    set<string> tags;
    bfs::path category;

    // Extract tags and category (folder path)
    bool is_tag = false, is_category = false;
    for (string s : args)
    {
        if (s.find("..") != string::npos || s.compare(".") == 0)
        {
            // Ignore '..' and '.' as categories
            cout << "Category cannot contain '..'" << endl;
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
            tags.insert(s);
        }
        if (is_category)
        {
            category /= s;
        }
    }
    if (category.is_absolute())
    {
        category = category.relative_path();
    }

    return make_pair(tags, category);
}

/* Interpret -and/-or argument chain.
 */
json interpret_list_specification(json metadata, vector<string> args)
{
    bool is_and = false, is_or = false, is_tag = false, is_category = false;
    // Counts switches between specifying -t and -c
    int switches = 0;

    // Vector containing tagset-category pairs
    // Metadata will then be narrowed down to notes fitting at least one pair's description
    vector<pair<set<string>, bfs::path>> spec;

    // Construct specification
    for (string arg : args)
    {
        if (arg.compare("-and") == 0)
        {
            is_and = true;
            is_or = false;
            if (spec.size() == 0)
            {
                spec.push_back(make_pair(set<string>(), bfs::path()));
            }
        }
        else if (arg.compare("-or") == 0)
        {
            is_or = true;
            is_and = false;

            // If user specifies e.g. -t tag but no category,
            // make any category match (and vice versa)
            // The switches variable is to catch if user specifies "" as category
            if (switches % 2 == 0 && spec.size() > 0)
            {
                if (is_tag && spec.back().second.empty())
                {
                    spec.back().second /= "-any";
                }
                else if (is_category && spec.back().first.size() == 0)
                {
                    spec.back().first.insert("-any");
                }
            }
            // Create new specification tuple
            spec.push_back(make_pair(set<string>(), bfs::path()));
        }
        else if (arg.compare("-t") == 0)
        {
            is_tag = true;
            if (is_category)
            {
                switches++;
                is_category = false;
            }
        }
        else if (arg.compare("-c") == 0)
        {
            is_category = true;
            if (is_tag)
            {
                switches++;
                is_tag = false;
            }
        }
        else
        {
            if (is_and || is_or)
            {
                // Add to last specification tuple
                if (is_tag)
                {
                    spec.back().first.insert(arg);
                }
                else if (is_category)
                {
                    spec.back().second /= arg;
                }
            }
            else
            {
                if (is_tag)
                {
                    if (spec.size() == 0)
                    {
                        spec.push_back(make_pair(set<string>(), bfs::path()));
                    }
                    spec.back().first.insert(arg);
                }
                else if (is_category)
                {
                    if (spec.size() == 0)
                    {
                        spec.push_back(make_pair(set<string>(), bfs::path()));
                    }
                    spec.back().second /= arg;
                }
            }
        }
    }

    if (is_tag && spec.back().second.empty())
    {
        spec.back().second /= "-any";
    }
    else if (is_category && spec.back().first.size() == 0)
    {
        spec.back().first.insert("-any");
    }

    // Narrow down specification
    json new_metadata = json::array();
    for (int i = 0; i < metadata.size(); i++)
    {
        Note note(metadata[i]);
        if (note.fits_specification(spec))
        {
            new_metadata.push_back(metadata[i]);
        }
    }

    return new_metadata;
}

json create_note(vector<string> args, string root, string editor)
{
    string filename = args[0];
    args.erase(args.begin());

    pair<set<string>, bfs::path> tag_category_pair = extract_tags_and_category(args);
    set<string> tags = tag_category_pair.first;
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

void list_notes(json metadata, vector<string> args, string root)
{
    if (args.size() > 0)
    {
        metadata = interpret_list_specification(metadata, args);
    }
    print_note_metadata(metadata, root);
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
            // TODO make output more visually appealing
            /*
            major1 minor1 sub1
                   minor2 sub1
                          sub2 subsub
                   minor3
            major2
            */
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

    pair<set<string>, bfs::path> tag_category_pair = extract_tags_and_category(args);
    set<string> new_tags = tag_category_pair.first;
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
            note_data["category"] = new_category.string();
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
    bfs::path filepath(root);
    filepath /= note_data["category"].get<string>();
    filepath /= note_data["file"].get<string>();
    bfs::remove_all(filepath);
}
