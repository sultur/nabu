#include <nlohmann/json.hpp>

// for convenience
using json = nlohmann::json;

#include "config.h"
#include "nabu_types.h"

#include "tui.cpp"
#include "cli.cpp"

using namespace std;

void load_data()
{
    string data_file_path = "";
    for (int i = 0; i < NABU_ROOT_PATH.size(); i++)
    {
        data_file_path.append(NABU_ROOT_PATH[i]);
    }
    data_file_path.append(".metadata");

    cout << "Loading metadata ..." << endl;

    // TODO READ & PARSE JSON

    cout << "Finished reading metadata ..." << endl;
}

int main(int argc, char const *argv[])
{

    if (argc == 1)
    {
        // No args
        // TUI MODE
        load_data();
        tui();
    }
    else
    {
        // CLI MODE
        load_data();

        // Initialize vector from command line arguments
        vector<string> args(argv, argv + argc);
        // Remove first argument (program name)
        args.erase(args.begin());

        cli(args);
    }

    return 0;
}
