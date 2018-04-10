#pragma once

#include <iostream>
#include <string>

#include "table.h"

using command = std::string;

class parser
{
public:
    parser();

    std::string parse_input(command &cmd);
    std::string ec_to_str(table_processor::error_code ec, std::string str);

private:
    table_processor processor;
};
