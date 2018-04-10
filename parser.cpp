#include "parser.h"

#include <vector>

#include <boost/algorithm/string.hpp>

using tec = table_processor::error_code;

parser::parser()
{
    processor.register_table("A");
    processor.register_table("B");
}

std::string parser::ec_to_str(table_processor::error_code ec, std::string str)
{
    std::string result;

    switch (ec)
    {
    case tec::OK:
        result = "OK";
        break;
    case tec::NO_TABLE:
        result = "ERR no such table " + str;
        break;
    case tec::DUPLICATED:
        result = "ERR duplicate " + str;
        break;
    case tec::TABLE_IS_EMPTY:
        result = "ERR table is empty " + str;
        break;

    default:
        result = "unknown error code";
        break;
    }
    return result + "\n";
}

std::string parser::parse_input(command &cmd)
{
    std::vector<std::string> tokens;
    boost::trim(cmd);
    boost::split(tokens, cmd, boost::is_any_of(" "));
    std::string result = "OK\n";

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
            auto ec = processor.insert(tokens[1], std::stoi(tokens[2]), tokens[3]);
            result = ec_to_str(ec, tokens[2]);
        }
        else if (command_match("TRUNCATE", 2))
        {
            std::cout << "TRUNCATE " << tokens[1] << std::endl;
            auto ec = processor.truncate(tokens[1]);
            result = ec_to_str(ec, tokens[1]);
        }
        else if (command_match("INTERSECTION", 1))
        {
            std::cout << "INTERSECTION" << std::endl;
            processor.intersection();
        }
        else if (command_match("SYMMETRIC_DIFFERENCE", 1))
        {
            std::cout << "SYMMETRIC_DIFFERENCE" << std::endl;
            processor.symmetric_difference();
        }
        else
        {
            std::cout << "unknown command" << std::endl;
            result = "ERR unknown command\n";
        }
    }
    else
    {
        std::cout << "parse_input error" << std::endl;
        result = "ERR bad command format\n";
    }
    return result;
}
