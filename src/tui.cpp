#include "tui.hpp"
#include <sstream>

using json = nlohmann::json;
using namespace std;

json tui(json metadata, string metafile)
{
    string answer;
    cout << "Nabu";

    while (true)
    {
        clean_directory_structure(NABU_ROOT_PATH);
        cout << "\n----------------------------\n"
             << "  1) New note\n"
             << "  2) List notes\n"
             << "  3) Edit note\n"
             << "  4) Delete note\n"
             << "  q) Quit\n\n"
             << "Choice (1,2,3,4,q): ";
        getline(cin, answer);
        cout << endl;

        if (answer.compare("1") == 0) // New note
        {
            // Get filename
            string filename = get_filename();
            // Get category/path
            bfs::path category = get_category();
            // Get tags
            set<string> tags = get_tags();

            pair<json, bool> return_pair = create_note(filename, category, tags, NABU_ROOT_PATH, NOTE_EDITOR);
            json new_metadata = return_pair.first;
            bool overwritten = return_pair.second;

            // Check if other file overwritten
            if (overwritten)
            {
                metadata = delete_duplicate(new_metadata, metadata);
            }

            metadata.push_back(new_metadata);

            // Write new metadata
            ofstream meta_output(metafile, ios::out | ios::trunc);
            meta_output << setw(4) << metadata << endl;
        }
        else if (answer.compare("2") == 0) // List notes
        {
            list_notes(metadata, vector<string>(), NABU_ROOT_PATH);
        }
        else if (answer.compare("3") == 0) // Edit note
        {
            if (metadata.size() == 0)
            {
                cout << "No notes found." << endl;
                continue;
            }
            int id;
            while (true)
            {
                cout << "Enter note id: ";
                getline(cin, answer);
                cout << endl;
                try
                {
                    id = stoi(answer);
                    if (id <= metadata.size())
                    {
                        break;
                    }
                    else
                    {
                        cout << "Please enter an integer between 1 and " << metadata.size() << "." << endl;
                    }
                }
                catch (invalid_argument)
                {
                    cout << "Please enter an integer between 1 and " << metadata.size() << "." << endl;
                }
            }
            pair<bool, set<string>> tag_pair = edit_tags(metadata[id - 1]);
            pair<bool, bfs::path> category_pair = edit_category(metadata[id - 1]);

            pair<json, bool> return_pair = edit_note(metadata[id - 1], tag_pair, category_pair, NABU_ROOT_PATH, NOTE_EDITOR);
            json new_metadata = return_pair.first;
            bool overwritten = return_pair.second;

            metadata[id - 1] = new_metadata;
            // Check if other file overwritten
            if (overwritten)
            {
                metadata = delete_duplicate(new_metadata, metadata, id - 1);
            }

            // Write new metadata
            ofstream meta_output(metafile, ios::out | ios::trunc);
            meta_output << setw(4) << metadata << endl;
        }
        else if (answer.compare("4") == 0) // Delete note
        {
            if (metadata.size() == 0)
            {
                cout << "No notes found." << endl;
                continue;
            }
            int id;
            while (true)
            {
                cout << "Enter note id: ";
                getline(cin, answer);
                cout << endl;
                try
                {
                    id = stoi(answer);
                    if (id <= metadata.size() && id >= 1)
                    {
                        break;
                    }
                    else
                    {
                        cout << "Please enter an integer between 1 and " << metadata.size() << "." << endl;
                    }
                }
                catch (invalid_argument)
                {
                    cout << "Please enter an integer between 1 and " << metadata.size() << "." << endl;
                }
            }
            json note = metadata[id - 1];
            metadata.erase(id - 1);

            delete_note(note, NABU_ROOT_PATH);

            ofstream meta_output(metafile, ios::out | ios::trunc);
            meta_output << setw(4) << metadata << endl;
        }
        else if (answer.compare("q") == 0) // Quit
        {
            break;
        }
    }

    return metadata;
}

string get_filename()
{
    string answer;
    cout << "Enter filename for note: ";
    getline(cin, answer);
    cout << endl;

    replace_all(answer, "..", "");
    replace_all(answer, " ", "_");
    replace_all(answer, "/", "");
    replace_all(answer, "\\", "");

    return answer;
}

bfs::path get_category()
{
    bfs::path category;
    string answer;
    cout << "Enter category of note separated by spaces (folder path): ";
    getline(cin, answer);
    cout << endl;

    replace_all(answer, "..", "");
    replace_all(answer, "  ", " ");

    istringstream ss(answer);
    string folder;
    while (getline(ss, folder, ' '))
    {
        category /= folder;
    }
    return category;
}

set<string> get_tags()
{
    set<string> tags;
    string answer;
    cout << "Enter tags separated by spaces: ";
    getline(cin, answer);
    cout << endl;

    replace_all(answer, "..", "");
    replace_all(answer, "  ", " ");

    istringstream ss(answer);
    string tag;
    while (getline(ss, tag, ' '))
    {
        tags.insert(tag);
    }
    return tags;
}

pair<bool, set<string>> edit_tags(json note)
{
    string answer;
    cout << "Current tags: ";
    string output = "[";
    for (string tag : note["tags"].get<set<string>>())
    {
        output += tag + ", ";
    }
    cout << output.substr(0, output.rfind(',')) << "]" << endl;

    cout << "Do you want to change the tags? (y/N): ";
    getline(cin, answer);
    cout << endl;

    if (answer.compare("y") == 0 || answer.compare("Y") == 0)
    {
        set<string> new_tags = get_tags();
        return make_pair(true, new_tags);
    }
    else
    {
        return make_pair(false, set<string>());
    }
}

pair<bool, bfs::path> edit_category(json note)
{
    string answer;
    cout << "Current category: "
         << note["category"].get<string>()
         << endl;

    cout << "Do you want to change the category? (y/N): ";
    getline(cin, answer);
    cout << endl;

    if (answer.compare("y") == 0 || answer.compare("Y") == 0)
    {
        bfs::path new_category = get_category();
        return make_pair(true, new_category);
    }
    else
    {
        return make_pair(false, bfs::path());
    }
}