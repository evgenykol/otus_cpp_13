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
    std::vector<int> vecA;
    std::vector<int> vecB;
    std::vector<int> vecSD;

    std::unique_lock<std::mutex> lk(m);

    vecA.reserve(tables["A"].size());
    for(auto &key : tables["A"])
    {
        vecA.push_back(key.first);
    }

    vecB.reserve(tables["B"].size());
    for(auto &key : tables["B"])
    {
        vecB.push_back(key.first);
    }
    lk.unlock();

    std::set_intersection(vecA.begin(), vecA.end(),
                                  vecB.begin(), vecB.end(),
                                  std::back_inserter(vecSD));

    for(auto &key : vecSD)
    {
        auto a = tables["A"].find(key);
        auto b = tables["B"].find(key);
        result += std::to_string(key) + "," + tables["A"].find(key)->second + "," + tables["B"].find(key)->second + "\n";
    }

    return result;
}

std::string table_processor::symmetric_difference()
{
    std::string result;
    std::vector<int> vecA;
    std::vector<int> vecB;
    std::vector<int> vecSD;

    std::unique_lock<std::mutex> lk(m);
    vecA.reserve(tables["A"].size());
    for(auto &key : tables["A"])
    {
        vecA.push_back(key.first);
    }

    vecB.reserve(tables["B"].size());
    for(auto &key : tables["B"])
    {
        vecB.push_back(key.first);
    }
    lk.unlock();

    std::set_symmetric_difference(vecA.begin(), vecA.end(),
                                  vecB.begin(), vecB.end(),
                                  std::back_inserter(vecSD));

    for(auto &key : vecSD)
    {
        auto kv = tables["A"].find(key);
        if(kv != tables["A"].end())
        {
            result += std::to_string(kv->first) +"," + kv->second + "\n";
        }
        else
        {
            kv = tables["B"].find(key);
            result += std::to_string(kv->first) +",," + kv->second + "\n";
        }

    }

    return result;
}
