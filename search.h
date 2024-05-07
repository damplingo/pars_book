#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "contents.h"

using vp = std::vector<std::pair<int, int>>;

bool isUint(const std::string& s);
vp findLIS(const vp& source);//наибольшая возрастающая подпоследовательность
int levenshtein(std::string str_1, std::string str_2);
std::string get_by_tag(int ordernum, const std::string& input, const std::string& output);
bool is_tag(const std::string& str);

class Search {
    public:
        int contents_size;
        Search(const std::string& _file_name, const std::string& _contents_name,  const std::string& _add_tag_file_name);
        int position_start(int page);//округлит вниз, если нет такой странички
        void find_text(const std::string& name);//глава по названию. Функция в данной реализации не используется, но была пунктом задания
        bool exist_page(int page);
        int end_this_page(int page);
        int find_title(Chapter& Chapter);//поиск главы
        void add_begin_tags();//добавление открывающих и закрывающих тэгов в вектора
        void add_tags_all();//вывод в файл строк и привязанных к ним тэгов. На выходе - файл с разметкой

    private:
        std::vector<std::string> text;//текст книги
        vp page_position;//соответствие страница - номер строки
        std::vector<std::string> begin_tags; //открывающий тэг, привязанный строке
        std::vector<std::vector<std::string>> end_tags;//закрывающие тэги, привязанные к строке
        const std::string& contents_name; 
        Contents contents;//оглавление
        const std::string& file_name;
        const std::string add_tag_file_name;

};