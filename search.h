#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "contents.h"

using vp = std::vector<std::pair<int, int>>;

bool isUint(const std::string& s);
vp findLIS(const vp& source);
int levenshtein(std::string str_1, std::string str_2);

class Search {
    public:
        Search(const std::string& _file_name, const std::string& _contents_name);
        int position_start(int page);//округлит вниз, если нет такой странички
        void find_text(const std::string& name);
        bool exist_page(int page);
        int end_this_page(int page);
        int find_title(const std::string& title);

    private:
        vp page_position;
        const std::string& contents_name; 
        Contents contents;//оглавление
        const std::string& file_name;

};