#include "config.h"
#include "nabu_types.h"

#include "tui.h"
#include "cli.h"

#define METADATAFILE ".metadata.json"

using json = nlohmann::json;
using namespace std;

json load_data();
void validate_home_directory();

int main(int argc, char const *argv[])
{
    validate_home_directory();
    json metadata = load_data();

    if (argc == 1)
    {
        // No args

        // TUI MODE
        tui(metadata);
    }
    else
    {
        // Initialize vector from command line arguments
        vector<string> args(argv, argv + argc);
        // Remove first argument (program name)
        args.erase(args.begin());

        // CLI MODE
        cli(args, metadata);
    }

    return 0;
}

/* Confirm that home folder exists and metadata file exists.
 * If not, they are created.
 */
void validate_home_directory()
{
    string ans;

    bfs::path metafile(NABU_ROOT_PATH.c_str());
    metafile /= METADATAFILE;

    if (!bfs::is_directory(NABU_ROOT_PATH))
    {
        cout << "No home folder found\n"
             << "Would you like to create a folder for Nabu at "
             << NABU_ROOT_PATH.native()
             << "? (Y/n) ";

        getline(cin, ans);
        if (ans.compare("n") == 0 || ans.compare("N") == 0)
        {
            cout << "Please configure the home folder in config.h and recompile using 'make'." << endl;
            exit(0);
        }

        // Create Nabu root folder
        bfs::create_directory(NABU_ROOT_PATH);

        // Create empty metadata file
        ofstream output(metafile.c_str());
    }

    if (!bfs::exists(metafile))
    {
        cout << "No metadata found.\n"
             << "Do you want to create a new empty metadata file for Nabu at "
             << metafile.native()
             << "? (Y/n) ";

        getline(cin, ans);
        if (ans.compare("n") == 0 || ans.compare("N") == 0)
        {
            cout << "Please import or create your own .metadata.json file." << endl;
            exit(0);
        }

        // Create empty metadata file
        ofstream output(metafile.c_str());
    }
}

/* Load and return metadata for notes as JSON.
 */
json load_data()
{
    bfs::path metafile(NABU_ROOT_PATH.c_str());
    metafile /= METADATAFILE;

    cout << "Loading metadata from " << metafile.native() << " ..." << endl;

    json metadata;
    ifstream inputfile(metafile.c_str());

    // Check if file is empty
    if (!bfs::is_empty(metafile))
    {
        inputfile >> metadata;
    }

    cout << "Finished reading metadata ..." << endl;

    return metadata;
}
