#include "../config.hpp"

#include "nabu_types.hpp"
#include "tui.hpp"
#include "cli.hpp"

#define METADATAFILE ".metadata.json"

using json = nlohmann::json;
using namespace std;

json load_data(bfs::path);
void validate_home_directory(bfs::path);

int main(int argc, char const *argv[])
{
    bfs::path metafile(NABU_ROOT_PATH);
    metafile /= METADATAFILE;

    validate_home_directory(metafile);
    json metadata = load_data(metafile);

    bool rewrite = false;
    if (argc == 1)
    {
        // TUI MODE
        metadata = tui(metadata);
    }
    else
    {
        // Initialize vector from command line arguments
        vector<string> args(argv, argv + argc);
        // Remove first argument (program name)
        args.erase(args.begin());

        // CLI MODE
        pair<bool, json> return_pair = cli(args, metadata, NABU_ROOT_PATH);
        rewrite = return_pair.first;
        metadata = return_pair.second;
    }

    // Rewrite metadata file if appropriate
    if (rewrite)
    {
        ofstream meta_output(metafile.c_str(), ios::out | ios::trunc);
        meta_output << setw(4) << metadata << std::endl;
    }
    return 0;
}

/* Confirm that home folder exists and metadata file exists.
 * If not, they are created.
 */
void validate_home_directory(bfs::path metafile)
{
    string ans;

    if (!bfs::is_directory(bfs::path(NABU_ROOT_PATH)))
    {
        cout << "No home folder found\n"
             << "Would you like to create a folder for Nabu at "
             << NABU_ROOT_PATH
             << "? (Y/n) ";

        getline(cin, ans);
        if (ans.compare("n") == 0 || ans.compare("N") == 0)
        {
            cout << "Please configure the home folder in config.h and recompile using 'make'." << endl;
            exit(0);
        }

        // Create Nabu root folder
        bfs::create_directory(bfs::path(NABU_ROOT_PATH));

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
json load_data(bfs::path metafile)
{
    json metadata;
    ifstream inputfile(metafile.c_str());

    // Check if file is empty
    if (!bfs::is_empty(metafile))
    {
        // Read and parse metadata from JSON
        inputfile >> metadata;

        if (!metadata.is_array())
        {
            cout << "Metadata is incorrect, should be JSON array." << endl;
            exit(1);
        }
        if (metadata.size() >= 1 && !metadata[0].is_object())
        {
            cout << "Metadata is incorrect, elements of array should be JSON objects." << endl;
            exit(1);
        }
    }

    return metadata;
}
