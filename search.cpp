#include "search.h"
#include <fstream>
#include <utility>
#include <algorithm>
#include <cctype>
#include <stack>
#include <iostream>
#include <limits.h>
#include <string>
#include <locale>
#include <codecvt>

bool isUint(const std::string& s){
    return s.find_first_not_of("\n0123456789 ") == std::string::npos;//пробел тк цифры все с пробелом хранятся
}

bool is_tag(const std::string& str) {
    std::string b = "@#b";
    std::string e = "@#e";
    if (str.substr(0, b.size()) == b || str.substr(0, e.size()) == e) {
        return true;
    }
    else {
        return false;
    }
}

std::string get_by_tag(int ordernum, const std::string& input, const std::string& output) {
    std::ifstream inf(input);
    std::string input_str;
    std::ofstream outf(output);
    std::string begin = "@#b" + std::to_string(ordernum);
    std::string end = "@#e" + std::to_string(ordernum);
    bool write = false;
    std::string res;
    while (std::getline(inf, input_str)) {
        if (input_str == begin) {
            write = true;
        }
        else if (input_str == end) {
            write = false;
            break;
        }

        else if (write == true && !is_tag(input_str)) {
            outf<<input_str<<'\n';
            res += input_str;
            res += '\n';
        }
    }
    return res;
}

Search::Search(const std::string& _file_name, const std::string& _contents_name, const std::string& _add_tag_file): file_name(_file_name), contents_name(_contents_name), add_tag_file_name(_add_tag_file) {
    contents = Contents(contents_name);
    contents_size = contents.get_size();
    std::ifstream inf(file_name);
    
    if (!inf) {
        std::cout<<"There is not file\n";
        return;
    }

    std::string input_str;
    int count_string = -1;//чтобы начиналась с 0 индексация
    while (std::getline(inf, input_str)){
        text.push_back(input_str);
        count_string += 1;
        try{
            if (isUint(input_str) && input_str!=" ") {
                page_position.push_back({stoi(input_str), count_string});
            }
        }
        catch(std::exception& ex) {
            std::cout<<input_str<<" "<<ex.what()<<"\n";
        }
    }

    page_position = findLIS(page_position);

    begin_tags = std::vector<std::string>(text.size(), "");
    end_tags = std::vector<std::vector<std::string>>(text.size(), {""});
    auto it = page_position.begin();
    

}

bool isUint_w(const std::wstring& s){
    return s.find_first_not_of(L"\n0123456789 ") == std::string::npos;//пробел тк цифры все с пробелом хранятся
}


bool Search::exist_page(int page) {
    for (auto& i: page_position) {
        if (i.first == page) {
            return true;
        }
    }
    return false;
} 

int Search::position_start(int page) {
    while (!exist_page(page)) {
        page -= 1;
    }
    
    for (auto& i: page_position) {
        if (i.first == page) {
            return i.second;//номер строки, с которой начинается страница page
        }
    }
    return 0;
}

int Search::end_this_page(int page) {//начало следующей отмеченной страницы
    for (int i = 0; i < page_position.size(); ++i) {
        if (page_position[i].first > page) {
            return page_position[i].second;//номер строки
        }
    }

    return 0; //дошли до последней страницы книги. Достаточно просто будет ее прочитать
}


void Search::add_begin_tags(){ //добавление открывающих и закрывающих тэгов в вектора
    std::stack<Chapter> end_stack;//начало одной главы - это конец одной или нескольких предыдущих

    for (int i = 0; i < contents.get_size(); ++i) {
        std::string tag = "@#b" + std::to_string(contents[i].get_ordernum()) + '\n';
        int number = find_title(contents[i]);
        begin_tags[number] = tag;
        Chapter t;
        if (!end_stack.empty()) t = end_stack.top();
        while (t.get_level() >= contents[i].get_level() && !end_stack.empty()) {
            std::string end_tag = "@#e" + std::to_string(t.get_ordernum()) + '\n';
            end_tags[number].push_back(end_tag);
            end_stack.pop();
            if (!end_stack.empty()) t = end_stack.top();
        }
        end_stack.push(contents[i]);
    }
    Chapter t;//снимаем стэк в конце
    if (!end_stack.empty()) t = end_stack.top();
    while (t.get_level() >= contents[contents.get_size()].get_level() && !end_stack.empty()) {
        std::string end_tag = "@#e" + std::to_string(t.get_ordernum()) + '\n';
        end_tags[text.size()].push_back(end_tag);
        end_stack.pop();
        if (!end_stack.empty()) t = end_stack.top();
    }

}


int Search::find_title(Chapter& this_chapter) { //возвращает номер строки с заголовком
    int start = position_start(this_chapter.get_start());
    int end = end_this_page(this_chapter.get_start());

    int min_levenshtein = INT_MAX;
    int result; //номер строки
    for (int i = start; i <= end; ++i) {
        int this_levenshtein = levenshtein(text[i], this_chapter.get_title());
        if (this_levenshtein < min_levenshtein) {
            min_levenshtein = this_levenshtein;

            result = i;
        }
    }
    return result;

}


void Search::find_text(const std::string& name) {//глава по названию(была пунктом заданиия, но в данной реализации
                                                //в чистом виде не используется)
    if (contents.exist(name)) {
        Chapter this_chapter = contents.find(name);
        Chapter next_chapter = contents.get_next(this_chapter);
        int start_position = find_title(this_chapter);
        int end_position = find_title(next_chapter);
        std::cout<<next_chapter.get_title()<<'\n';
        std::ofstream outf("out_cont.txt");

        for (int i = start_position; i < end_position; ++i) {
            outf<<text[i]<<'\n';
        }
    }
}


void Search::add_tags_all() { //вызов функции, добавляющей тэги add_begin_tags(), запись файла со вставлением тэгов
    std::ofstream outf(add_tag_file_name);
   
    add_begin_tags();
    for (int i = 0; i < text.size(); ++i) {
        for (auto& t: end_tags[i]) {//вывод завершающих тэгов перед строкой
            outf<<t;
        }
        outf<<begin_tags[i];//вывод открывающих тэгов перед строкой
        outf<<text[i]<<'\n';

    }

    for (auto& t: end_tags[text.size()]) {//вывод закрываающих тэгов после текста
            outf<<t;
        } 
}


vp  findLIS(const vp & source) { //ищем наибольшую возрастающую подпоследовательность по значению строки обозначающей страницу
    int n = source.size();
    int prev[n-1];
    int d[n-1];

    for (int i = 0; i < n; ++i) {
        d[i] = 1;
        prev[i] = -1;
        for (int j = 0; j < i; ++j) { //second
            if (source[j].second < source[i].second && source[j].first <= source[i].first && d[j]+1 > d[i]) {
                d[i] = d[j] + 1;
                prev[i] = j;
            }
        }
    
    }
    int pos = 0; //индекс последнего элемента НВП
    int lenght = d[0]; //длина НВП
    for (int i = 0; i < n; ++i) {
        if (d[i] > lenght) {
            pos = i;
            lenght = d[i];
        }
            
    }
    vp  answer;
    while (pos != -1) {
        answer.push_back(source[pos]);
        pos = prev[pos];
    }

    std::reverse(answer.begin(), answer.end());
    return answer;
}

std::wstring ToLower(std::string s) {
    
    //const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
    
    //std::wofstream out("out_tolower.txt");
    //out.imbue(utf8_locale);
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring w_sstring= converter.from_bytes(s);
    std::for_each(w_sstring.begin(), w_sstring.end(), [](auto& c) {
        if (int(c)>=int(L'А') && int(c)<=int(L'Я')) {
            c += 32;
        }
    });
    return w_sstring;
}

int levenshtein(std::string string_1, std::string string_2) { ///len(str_1)<len(str_2)
    
    if (string_1.size() > string_2.size()) {
        std::swap(string_1, string_2);
    }

    std::wstring str_1 = ToLower(string_1);
    std::wstring str_2 = ToLower(string_2);
    int n = str_1.size();///min_size
    int m = str_2.size();///max_size
    int current_m[n+1];

    int previous_diagonal_save;
    int previous_diagonal;

    for (int i = 0; i <= n; ++i) {
        current_m[i] = i;
    }

    for (int i = 1; i <= m; ++i){
        previous_diagonal = current_m[0];
        ++current_m[0];///column


        for (int j = 1; j <= n; ++j) {
            previous_diagonal_save = current_m[j];
            if (str_1[j-1] == str_2[i-1]) {
                current_m[j] = previous_diagonal;
            }
            else {
                current_m[j] = std::min(std::min(current_m[j-1], current_m[j]), previous_diagonal) + 1;
            }
            previous_diagonal = previous_diagonal_save;

        }

    }
    return current_m[n];
}

