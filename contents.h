#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

//работа с полученным csv файлом

//класс главы
class Chapter {
    public:
        Chapter(int id, int _ordernum, std::string _index, std::string _title, int start_page_,
        int end_page_, int level_);
        Chapter() = default;
        int get_start();
        int get_end();
        int get_level();
        int get_id();
        std::string& get_title();
        std::string& get_index();
        int get_ordernum();
    private:
        std::string index;
        std::string title;
        int id;
        int start_page;
        int end_page;
        int level;
        int ordernum;
};


//класс оглавления
class Contents {
    public:
        Contents() = default;
        Contents(std::string file_name);
        int get_size();
        Chapter &operator[](size_t idx);
        bool exist(const std::string& name);
        Chapter& find(const std::string& name);
        Chapter& get_next(Chapter& chapter);

    private:
        std::vector<Chapter> contents;
        std::string file_name;
        int size;
};