#ifndef NABU_TUI_HEADER
#define NABU_TUI_HEADER

#include "../config.hpp"

nlohmann::json tui(nlohmann::json, std::string);
std::string get_filename();
bfs::path get_category();
std::set<std::string> get_tags();

std::pair<bool, std::set<std::string>> edit_tags(nlohmann::json);
std::pair<bool, bfs::path> edit_category(nlohmann::json);

#endif // NABU_TUI_HEADER
