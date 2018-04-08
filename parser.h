#pragma once

#include <string>

using command = std::string;

class parser
{
public:
    parser();

    parse_input(command &cmd);
    make_error_code();
    register_table();
};
