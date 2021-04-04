#include "cli.hpp"

using json = nlohmann::json;
using namespace std;

int cli(vector<string> args, json metadata)
{
    string action = args[0];
    if (action.compare("new") == 0 || action.compare("n") == 0)
    {
        cout << "create new note" << endl;
    }
    else if (action.compare("list") == 0 || action.compare("l") == 0)
    {
        cout << "list notes" << endl;
    }
    else if (action.compare("edit") == 0 || action.compare("e") == 0)
    {
        cout << "edit note" << endl;
    }
    else if (action.compare("delete") == 0 || action.compare("d") == 0)
    {
        cout << "delete note" << endl;
    }

    return 0;
}
