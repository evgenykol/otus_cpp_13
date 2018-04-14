#pragma once

#include <map>
#include <set>
#include <vector>
#include <string>
#include <memory>

using table_type = std::map<int, std::string>;
using index_type = std::set<int>;

using return_type = std::string;

class table
{
private:
    table_type tbl;
    index_type idx;
public:
    table() {}

    bool insert(int id, std::string &name);
    bool truncate();

    table_type &intersection(table_type &rht);
    table_type &symmetric_difference(table_type &rht);
};

class table_processor
{
    std::map<std::string, table_type> tables;
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
