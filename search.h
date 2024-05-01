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
        Search(const std::string& _file_name, const std::string& _contents_name,  const std::string& _add_tag_file_name);
        int position_start(int page);//округлит вниз, если нет такой странички
        void find_text(const std::string& name);
        bool exist_page(int page);
        int end_this_page(int page);
        int find_title(Chapter& Chapter);
        //void add_tags_for_title(const std::string& title, std::fstream& outf);
        void add_begin_tags();
        void add_tags_for_title(const std::string& title);
        void add_tags_all();
    private:
        std::vector<std::string> text;
        vp page_position;
        std::vector<std::string> begin_tags; //на i-м месте начала главы открывающий тэг, если строка не начало главы - пустота.
        std::vector<std::vector<std::string>> end_tags;
        const std::string& contents_name; 
        Contents contents;//оглавление
        const std::string& file_name;
        const std::string add_tag_file_name;

};