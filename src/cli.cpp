#include "cli.h"

using json = nlohmann::json;
using namespace std;

int cli(vector<string> args, json metadata)
{
    string action = args[0];
    if (action.compare("new") == 0)
    {
        cout << "create new memo" << endl;
    }
    else if (action.compare("grep") == 0)
    {
        cout << "return memos containing keywords" << endl;
    }
    else if (action.compare("find") == 0)
    {
        cout << "return filepath" << endl;
    }
    else if (action.compare("update") == 0)
    {
        cout << "update memo info" << endl;
    }

    return 0;
}
