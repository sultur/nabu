#include "tui.hpp"

using json = nlohmann::json;
using namespace std;

json tui(json metadata)
{
    cout << "\033[2m\033[3mNabu\033[0m" << endl;
    cout << "\033[2m----------------------------\033[0m" << endl;

    return metadata;
}
