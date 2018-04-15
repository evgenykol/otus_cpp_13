#pragma once

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <mutex>

using table_type = std::map<int, std::string>;

class table_processor
{
    std::map<std::string, table_type> tables;
    std::mutex m;
public:
    table_processor() {}

    enum error_code
    {
        OK,
        NO_TABLE,
        DUPLICATED,
        TABLE_IS_EMPTY
    };

    void register_table(const std::string &table_name);

    error_code insert(const std::string &table_name, int id, std::string &value);
    error_code truncate(const std::string &table_name);
    std::string intersection();
    std::string symmetric_difference();
};
