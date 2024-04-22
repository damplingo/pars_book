#include "search.h"
#include <fstream>
#include <utility>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits.h>
#include <string>
#include <locale>
#include <codecvt>

bool isUint(const std::string& s){
    return s.find_first_not_of("\n0123456789 ") == std::string::npos;//пробел тк цифры все с пробелом хранятся
}


Search::Search(const std::string& _file_name, const std::string& _contents_name): file_name(_file_name), contents_name(_contents_name) {
    contents = Contents(contents_name);
    std::ifstream inf(file_name);
    
    if (!inf) {
        std::cout<<"There is not file\n";
        return;
    }

    std::ofstream outf("out_2.txt");
    std::string input_str;
    int previous_page = 0;
    bool after_contents = false;
    while (std::getline(inf, input_str)){

        try{
            if (isUint(input_str) && input_str!=" ") {
                page_position.push_back({stoi(input_str), inf.tellg()});
                previous_page = stoi(input_str);
            }
        }
        catch(std::exception& ex) {
            std::cout<<input_str<<" "<<ex.what()<<"\n";
        }
    }

    page_position = findLIS(page_position);
    auto it = page_position.begin();
    while (it != page_position.end()) {
        outf<<it->first<<" "<<it->second<<"\n";
        ++it;
    }

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
            return i.second;//положение каретки
        }
    }
    return 0;///***
}

int Search::end_this_page(int page) {
    for (int i = 0; i < page_position.size(); ++i) {
        if (page_position[i].first > page) {
            return page_position[i].second;//положение каретки
        }
    }

    return 0; //дошли до последней страницы книги. Достаточно просто будет ее прочитать
}

int Search::find_title(const std::string& title) {//вернет позицию с которой начинается строка с заголовком
    std::ifstream inf(file_name);
    Chapter this_chapter = contents.find(title);
    int start = position_start(this_chapter.get_start());
    
    int end = end_this_page(this_chapter.get_start());
    std::string low_title(title);


    inf.seekg(start);
    std::string str_input;
    int result;
    int current_position = start;
    int min_levenshtein = INT_MAX;
    while (std::getline(inf, str_input)) {
        std::string low_str(str_input);
        if (current_position >= end) break;
        if (levenshtein(low_str, low_title) < min_levenshtein) {
            min_levenshtein = levenshtein(low_str, low_title);
    
            result = current_position;
        }
        current_position = inf.tellg();
        

    }
    return result;
}


void Search::find_text(const std::string& name) {//продумать для последней главы отдельно
    if (contents.exist(name)) {
        Chapter this_chapter = contents.find(name);
        std::ifstream inf(file_name);
        Chapter next_chapter = contents.get_next(this_chapter);
        int start_position = find_title(name);
        int end_position = find_title(next_chapter.get_title());

        std::ofstream outf("out_cont.txt");
        std::string str_input;
        inf.seekg(start_position);
        std::getline(inf, str_input);
        while (std::getline(inf, str_input) && inf.tellg() <= end_position) {
            outf<<str_input<<"\n";
            
        }

    }

    else std::cout<<"can not find\n";
    return;
}


void Search::add_tags_for_title(const std::string& title) {// после тестирования передавать поток, а не имя
    Chapter chapter = contents.find(title);
    std::string begin_title = "@#b" + std::to_string(chapter.get_ordernum());
    std::string end_title = "@#e"+ std::to_string(chapter.get_ordernum());

    int pos_b = find_title(title);
    Chapter next_chapter = contents.get_next(chapter);
    int pos_e = find_title(next_chapter.get_title());
    std::cout<<"next chapter in add_tags "<<next_chapter.get_title()<<"\n";
    std::cout<<pos_b<<" "<<pos_e<<'\n';
    std::ofstream outf("add_tag.txt");

    std::string str_input;
    std::ifstream inf(file_name);
    inf.seekg(pos_b);
    outf<<begin_title<<'\n';
    outf<<title<<'\n';
    std::getline(inf, str_input);
    
    while (std::getline(inf, str_input) && inf.tellg() <= pos_e) {
            outf<<str_input<<'\n';
        }
    outf<<end_title;    

}


vp  findLIS(const vp & source) { //ищем наибольшую возрастающую подпоследовательность по номеру символа
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
    
    const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
    
    std::wofstream out("out_tolower.txt");
    out.imbue(utf8_locale);
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring w_sstring= converter.from_bytes(s);
    //std::wstring w_sstring(s.begin(), s.end());
    out<<w_sstring;
    std::for_each(w_sstring.begin(), w_sstring.end(), [](auto& c) {
        if (int(c)>=int(L'А') && int(c)<=int(L'Я')) {
            c += 32;
        }
    });
    out<<w_sstring<<'\n';
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

