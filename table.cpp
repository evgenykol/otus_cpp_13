#include "table.h"

#include <memory>

void table_processor::register_table(const std::string &table_name)
{
    tables.emplace(std::make_pair(table_name, table_type()));
}

table_processor::error_code table_processor::insert(const std::string &table_name, int id, std::string &value)
{
    if(tables.find(table_name) == tables.end())
    {
        return NO_TABLE;
    }

    table_type &t = tables[table_name];
    if(t.find(id) == t.end())
    {
        t.insert(std::make_pair(id, value));
        return OK;
    }
    else
    {
        return DUPLICATED;
    }
}

table_processor::error_code table_processor::truncate(const std::string &table_name)
{
    if(tables.find(table_name) == tables.end())
    {
        return NO_TABLE;
    }

    table_type &t = tables[table_name];
    if(!t.empty())
    {
        t.clear();
        return OK;
    }
    else
    {
        return TABLE_IS_EMPTY;
    }
}
table_type &table_processor::intersection()
{
    //return nullptr;
}
table_type &table_processor::symmetric_difference()
{
    //return nullptr;
}
