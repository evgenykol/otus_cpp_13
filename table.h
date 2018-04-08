#pragma once

#include <map>
#include <set>

using table_type = std::map<int, std::string>;
using index_type = std::set<int>;

class table
{
private:
    table_type tbl;
    index_type idx;
public:
    table();

    bool insert(int id, std::string &name);
    bool clear();

    table_type &intersection(table_type &rht);
    table_type &symmetric_difference(table_type &rht);
};
