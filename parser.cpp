#include "parser.h"

#include <vector>

#include <boost/algorithm/string.hpp>

parser::parser()
{

}

bool parser::parse_input(command &cmd)
{
    std::vector<std::string> tokens;
    boost::split(tokens, cmd, boost::is_any_of(" "));

    auto command_match = [&](std::string command, size_t count)
    {
        boost::to_upper(tokens[0]);
        return (tokens[0] == command) && (tokens.size() == count);
    };

    if (tokens.size())
    {
        if(command_match("INSERT", 4))
        {
            std::cout << "INSERT " << tokens[1] << " " << tokens[2] << " "<< tokens[3] << std::endl;
        }
        else if (command_match("TRUNCATE", 2))
        {
            std::cout << "TRUNCATE " << tokens[1] << std::endl;
        }
        else if (command_match("INTERSECTION", 1))
        {
            std::cout << "INTERSECTION" << std::endl;
        }
        else if (command_match("SYMMETRIC_DIFFERENCE", 1))
        {
            std::cout << "SYMMETRIC_DIFFERENCE" << std::endl;
        }
        else
        {
            std::cout << "unknown command" << std::endl;
        }
    }
    else
    {
        std::cout << "parse_input error" << std::endl;
    }
}
