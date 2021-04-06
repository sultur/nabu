#include "cli.hpp"

using json = nlohmann::json;
using namespace std;

int get_id(vector<string> args)
{
    int id;
    if (args.size() < 1)
    {
        cout << "Please provide note id." << endl;
        exit(1);
    }
    try
    {
        id = stoi(args[0]);
    }
    catch (std::invalid_argument)
    {
        cout << "Second argument should be id of note." << endl;
        exit(1);
    }
    return id;
}

pair<bool, json> cli(vector<string> args, json metadata, string root)
{
    bool rewrite = false;
    string action = args[0];
    args.erase(args.begin());

    if (action.compare("new") == 0 || action.compare("n") == 0)
    {
        if (args.size() < 1)
        {
            cout << "Please provide filename and optionally any tags or category." << endl;
            exit(1);
        }
        json new_metadata = create_note(args, root, NOTE_EDITOR);

        metadata.push_back(new_metadata);
        rewrite = true;
    }
    else if (action.compare("read") == 0 || action.compare("r") == 0)
    {
        int id = get_id(args);
        json note = metadata[id - 1];

        string command = "";
        bfs::path filepath(NABU_ROOT_PATH);
        filepath /= note["category"].get<string>();
        filepath /= note["file"].get<string>();

        NoteType type = note["type"].get<NoteType>();
        switch (type)
        {
        case Image:
            command += IMG_VIEWER;
            break;
        case PDF:
            command += PDF_VIEWER;
            break;
        case Text:
            command += NOTE_READER;
            break;
        }

        command += " " + filepath.string();
        system(command.c_str());
    }
    else if (action.compare("list") == 0 || action.compare("l") == 0)
    {
        list_notes(metadata, args, root);
    }
    else if (action.compare("edit") == 0 || action.compare("e") == 0)
    {
        int id = get_id(args);

        metadata[id - 1] = edit_note(metadata[id - 1], args, root, NOTE_EDITOR);
        rewrite = true;
    }
    else if (action.compare("delete") == 0 || action.compare("d") == 0)
    {
        int id = get_id(args);
        json note = metadata[id - 1];
        metadata.erase(id - 1);

        delete_note(note, root);
        rewrite = true;
    }
    else if (action.compare("list-categories") == 0 || action.compare("lc") == 0)
    {
        list_categories(root);
    }
    else if (action.compare("list-tags") == 0 || action.compare("lt") == 0)
    {
        list_tags(metadata);
    }

    return make_pair(rewrite, metadata);
}
