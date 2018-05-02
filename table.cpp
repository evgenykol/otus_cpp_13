#include "table.h"

#include <memory>
#include <algorithm>

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
        std::unique_lock<std::mutex> lk(m);
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
std::string table_processor::intersection()
{
    std::string result;
    auto &table_a = tables["A"];
    auto &table_b = tables["B"];

    std::unique_lock<std::mutex> lk(m);

    for(auto p : table_a) //nA * log(nA)
    {
        auto b = table_b.find(p.first); //log(nB)
        if(b != table_b.end())
        {
             result += std::to_string(p.first) +"," + p.second +"," + b->second + "\n";
        }
    }

    return result;
}

std::string table_processor::symmetric_difference()
{
    std::string result;
    auto &table_a = tables["A"];
    auto &table_b = tables["B"];

    std::unique_lock<std::mutex> lk(m);

    for(auto p : table_a) //nA * log(nA)
    {
        if(table_b.find(p.first) == table_b.end()) //log(nB)
        {
             result += std::to_string(p.first) +"," + p.second + "\n";
        }
    }

    for(auto p : table_b) //nB * log(nB)
    {
        if(table_a.find(p.first) == table_a.end()) //log(nA)
        {
             result += std::to_string(p.first) +",," + p.second + "\n";
        }
    }

    return result;
}
