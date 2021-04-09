#include "shared_functionality.hpp"
#include "nabu_types.hpp"

using json = nlohmann::json;
using namespace std;

void print_note_metadata(vector<pair<int, json>> metadata, string root = "")
{
    if (metadata.size() == 0)
    {
        cout << "No notes match specification." << endl;
        return;
    }
    // Output columns:
    // ID, File, Type, Category, Tags, Abs. path
    vector<vector<string>> output_table = {{"ID", "File", "Type", "Category", "Tags", "Abs. path"}};
    vector<int> longest_strings = {2, 4, 4, 8, 4, 9};

    // Construct table
    for (int i = 0; i < metadata.size(); i++)
    {
        string id = to_string(metadata[i].first);
        longest_strings[0] = (id.length() > longest_strings[0]) ? id.length() : longest_strings[0];

        string file = metadata[i].second["file"].get<string>();
        longest_strings[1] = (file.length() > longest_strings[1]) ? file.length() : longest_strings[1];

        NoteType type = metadata[i].second["type"].get<NoteType>();
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

        string category = metadata[i].second["category"].get<string>();
        longest_strings[3] = (category.length() > longest_strings[3]) ? category.length() : longest_strings[3];

        vector<string> tags = metadata[i].second["tags"].get<vector<string>>();
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
pair<pair<bool, set<string>>, pair<bool, bfs::path>> extract_tags_and_category(vector<string> args)
{
    bool provided_tags = false, provided_category = false;
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
            provided_category = true;
            continue;
        }
        else if (s.compare("-t") == 0)
        {
            is_tag = true;
            is_category = false;
            provided_tags = true;
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

    return make_pair(make_pair(provided_tags, tags), make_pair(provided_category, category));
}

/* Interpret -and/-or argument chain.
 */
vector<pair<int, json>> interpret_list_specification(vector<pair<int, json>> metadata, vector<string> args)
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
    vector<pair<int, json>> new_metadata;
    for (int i = 0; i < metadata.size(); i++)
    {
        Note note(metadata[i].second);
        if (note.fits_specification(spec))
        {
            new_metadata.push_back(metadata[i]);
        }
    }

    return new_metadata;
}

pair<json, bool> create_note(string filename, bfs::path category, set<string> tags, string root, string editor)
{
    bool overwritten = false;
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
        overwritten = true;
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

    // Create new note
    Note note(filepath.filename().string(), category.string(), type, tags);

    // Return metadata for new note and whether a note was overwritten
    return make_pair(note.get_metadata(), overwritten);
}

void list_notes(json metadata, vector<string> args, string root)
{
    // Create id's
    vector<pair<int, json>> output_metadata;
    for (int i = 0; i < metadata.size(); i++)
    {
        output_metadata.push_back(make_pair(i + 1, metadata[i]));
    }

    if (args.size() > 0)
    {
        output_metadata = interpret_list_specification(output_metadata, args);
    }
    print_note_metadata(output_metadata, root);
}

void list_categories(string root)
{
    bfs::path root_folder(root);
    bfs::path curr_path, parent_path;
    int curr_depth = -1, last_depth = -1, indent = 0;

    // Recursively iterate through directories below root folder
    for (auto dir = bfs::recursive_directory_iterator(root_folder);
         dir != bfs::recursive_directory_iterator();
         ++dir)
    {
        if (bfs::is_directory(dir->path()))
        {
            curr_path = bfs::relative(dir->path(), root_folder);

            curr_depth = dir.depth();
            if (curr_depth <= last_depth)
            {
                cout << "\n";
                if (!parent_path.empty() && (curr_depth > 0))
                {
                    bfs::path::iterator curr_it = curr_path.begin();
                    bfs::path::iterator par_it = parent_path.begin();
                    indent = 0;

                    // Calculate length of indent
                    while ((curr_it != curr_path.end()) && (par_it != parent_path.end()))
                    {
                        if (par_it->compare(*curr_it) == 0)
                        {
                            indent += par_it->string().size();
                        }
                        else
                        {
                            break;
                        }
                        curr_it++;
                        par_it++;
                    }
                    // Print alignment
                    cout << string(indent + ((curr_depth - 1) * 3), ' ');
                }
            }
            if (curr_depth > 0)
            {
                cout << " > ";
            }
            cout << dir->path().filename().string();

            last_depth = curr_depth;
            parent_path = curr_path;
        }
    }
    cout << endl;
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

pair<json, bool> edit_note(json note_data, pair<bool, set<string>> tag_pair, pair<bool, bfs::path> category_pair, string root, string editor)
{
    // Check if user provided either '-t' or '-c' as an argument
    bool overwritten = false;

    bfs::path filepath(root);
    filepath /= note_data["category"].get<string>();
    filepath /= note_data["file"].get<string>();

    if (bfs::exists(filepath))
    {
        // Change category
        if (category_pair.first)
        {
            bfs::path new_path(root);
            new_path /= category_pair.second;

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
                overwritten = true;
            }

            // Move file to new category
            bfs::copy_file(filepath, new_path, bfs::copy_options::overwrite_existing);
            bfs::remove_all(filepath);
            cout << "Note moved from " << note_data["category"].get<string>() << " to " << category_pair.second.string() << ".\n";
            note_data["category"] = category_pair.second.string();
            filepath = new_path;
        }

        // Edit note content
        string command = editor + " " + filepath.string();
        system(command.c_str());

        // Apply new tags
        if (tag_pair.first)
        {
            note_data["tags"] = tag_pair.second;
            cout << "New tags applied\n";
        }
    }
    else
    {
        cout << "Note wasn't found." << endl;
        exit(1);
    }

    // Remove empty directories
    clean_directory_structure(root);
    return make_pair(note_data, overwritten);
}

void delete_note(json note_data, string root)
{
    bfs::path filepath(root);
    filepath /= note_data["category"].get<string>();
    filepath /= note_data["file"].get<string>();
    bfs::remove_all(filepath);

    // Remove empty directories
    clean_directory_structure(root);
}

/* Remove all empty directories below root path.
 */
void clean_directory_structure(string root)
{
    bfs::path root_folder(root);
    bool found_empty;
    vector<bfs::path> to_remove;
    do
    {
        found_empty = false;
        for (auto dir = bfs::recursive_directory_iterator(root_folder);
             dir != bfs::recursive_directory_iterator();
             ++dir)
        {
            if (bfs::is_empty(dir->path()))
            {
                found_empty = true;
                to_remove.push_back(dir->path());
            }
        }
        for (bfs::path p : to_remove)
        {
            bfs::remove_all(p);
        }
        to_remove.clear();
    } while (found_empty);
}

/* Given a note and array of other notes in JSON format, finds note in array with
 * same path and removes from array, returning new array.
 */
json delete_duplicate(json new_note, json metadata, int skip_index)
{
    bfs::path note_path;
    note_path /= new_note["category"].get<string>();
    note_path /= new_note["file"].get<string>();

    if (metadata.is_array())
    {
        for (int i = 0; i < metadata.size(); i++)
        {
            if (i == skip_index)
            {
                continue;
            }
            json obj = metadata[i];
            if (obj["file"].get<string>().compare(new_note["file"].get<string>()) == 0)
            {
                bfs::path other_path;
                other_path /= obj["category"].get<string>();
                other_path /= obj["file"].get<string>();

                if (other_path.compare(note_path) == 0)
                {
                    metadata.erase(i);
                    return metadata;
                }
            }
        }
    }
    return metadata;
}

void replace_all(string &source, const string &from, const string &to)
{
    string newString;
    newString.reserve(source.length()); // avoids a few memory allocations

    string::size_type lastPos = 0;
    string::size_type findPos;

    while (string::npos != (findPos = source.find(from, lastPos)))
    {
        newString.append(source, lastPos, findPos - lastPos);
        newString += to;
        lastPos = findPos + from.length();
    }

    // Care for the rest after last occurrence
    newString += source.substr(lastPos);

    source.swap(newString);
}
