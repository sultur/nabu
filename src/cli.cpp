#include "cli.hpp"

using json = nlohmann::json;
using namespace std;

pair<bool, json> cli(vector<string> args, json metadata, string root)
{
    bool rewrite = false;
    string action = args[0];
    args.erase(args.begin());

    if (action.compare("new") == 0 || action.compare("n") == 0)
    {
        json new_metadata = create_note(args, root, NOTE_EDITOR);

        metadata.push_back(new_metadata);
        rewrite = true;
    }
    else if (action.compare("read") == 0 || action.compare("r") == 0)
    {
        //TODO
    }
    else if (action.compare("list") == 0 || action.compare("l") == 0)
    {
        list_notes(metadata, args);
    }
    else if (action.compare("edit") == 0 || action.compare("e") == 0)
    {
        int id;
        try
        {
            id = stoi(args[0]);
        }
        catch (std::invalid_argument)
        {
            cout << "First argument should be id of note." << endl;
            exit(1);
        }

        edit_note(metadata[id - 1], args);
        rewrite = true;
    }
    else if (action.compare("delete") == 0 || action.compare("d") == 0)
    {
        //TODO
        rewrite = true;
    }
    else if (action.compare("list-categories") == 0 || action.compare("lc") == 0)
    {
        list_categories();
    }
    else if (action.compare("list-tags") == 0 || action.compare("lt") == 0)
    {
        list_tags(metadata);
    }

    return make_pair(rewrite, metadata);
}
