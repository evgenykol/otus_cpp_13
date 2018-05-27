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

    auto first1 = tables["A"].begin();
    auto last1 = tables["A"].end();
    auto first2 = tables["B"].begin();
    auto last2 = tables["B"].end();


    std::unique_lock<std::mutex> lk(m);
    while (first1 != last1 && first2 != last2)
    {
        if (first1->first < first2->first)
        {
            ++first1;
        }
        else
        {
            if (!(first2->first < first1->first))
            {
                result += std::to_string(first1->first) +"," + first1->second +"," + first2->second + "\n";
                ++first1;
            }
            ++first2;
        }
    }

    return result;
}

std::string table_processor::symmetric_difference()
{
    std::string result;

    auto first1 = tables["A"].begin();
    auto last1 = tables["A"].end();
    auto first2 = tables["B"].begin();
    auto last2 = tables["B"].end();
    std::unique_lock<std::mutex> lk(m);

    while (first1 != last1)
    {
        if (first2 == last2)
        {
            while(first1 != last1)
            {
                result += std::to_string(first1->first) +"," + first1->second + "\n";
                ++first1;
            }
        }

        if (first1->first < first2->first)
        {
            result += std::to_string(first1->first) +"," + first1->second + "\n";
            ++first1;
        }
        else
        {
            if (first2->first < first1->first)
            {
                result += std::to_string(first2->first) +",," + first2->second + "\n";
            }
            else
            {
                ++first1;
            }
            ++first2;
        }
    }

    while(first2 != last2)
    {
        result += std::to_string(first2->first) +",," + first2->second + "\n";
        ++first2;
    }

    return result;
}
