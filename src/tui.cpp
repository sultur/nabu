#include "tui.hpp"

using json = nlohmann::json;
using namespace std;

json tui(json metadata)
{
    cout << FAINT << "Nabu" << endl;
    cout << "----------------------------" << REGULAR << endl;

    return metadata;
}
