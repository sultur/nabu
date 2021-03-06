#ifndef NABU_CLI_HEADER
#define NABU_CLI_HEADER

#include "../config.hpp"

std::pair<bool, nlohmann::json> cli(std::vector<std::string>, nlohmann::json, std::string);
int get_id(std::vector<std::string>);

#endif // NABU_CLI_HEADER
