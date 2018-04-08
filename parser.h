#pragma once

#include <iostream>
#include <string>

using command = std::string;

class parser
{
public:
    parser();

    bool parse_input(command &cmd);
    bool make_error_code();
    bool register_table();
private:
};
