#include "search.h"
#include <fstream>
#include <utility>
#include <algorithm>
#include <cctype>
#include <limits.h>

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
    std::cout<<page_position.size()<<"\n";
    while (it != page_position.end()) {
        outf<<it->first<<" "<<it->second<<"\n";
        ++it;
    }

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
            return i.second;
        }
    }
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

    std::transform(low_title.begin(), low_title.end(), low_title.begin(), [](char c){ return std::tolower(c);});

    inf.seekg(start);
    std::string str_input;
    int result;
    int current_position = start;
    std::ofstream out("out_stream.txt");
    int min_levenshtein = INT_MAX;
    while (std::getline(inf, str_input)) {
        std::string low_str(str_input);
        if (current_position >= end) break;
        std::transform(low_str.begin(), low_str.end(), low_str.begin(), [](char c){ return std::tolower(c);});
        if (levenshtein(low_str, low_title) < min_levenshtein) {
            min_levenshtein = levenshtein(low_str, low_title);
            result = current_position;
            out<<low_str<<" "<<title<<" "<<min_levenshtein<<"\n";
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
        std::ofstream inform("inform.txt");
        int start_position = find_title(name);
        int end_position = find_title(next_chapter.get_title());

        std::ofstream outf("out_cont.txt");
        inform<<this_chapter.get_title()<<" start_pos"<<start_position<<"\n";
        inform<<next_chapter.get_title()<<" start_pos"<<end_position<<"\n";

        std::string str_input;
        inf.seekg(start_position);
        std::getline(inf, str_input);
        //inform<<str_input<<" first str\n";
        while (std::getline(inf, str_input) && inf.tellg() < end_position) {
            outf<<str_input<<"\n";
            
            /*if (!this_chapter.get_index().empty()) {
                if (levenshtein(this_chapter.get_index() + name, str_input) <= 2) {
                    write = true;
                }
            }
            
            if (levenshtein(name, str_input) <= 2) {
                write = true;
            }

            if (levenshtein(next_chapter.get_title(), str_input) <= 2 &&
            inf.tellg() >= end) {
                write = false;
                return;
            }*/

            /*if (write) {
                outf<<str_input<<"\n";
            }*/
        }

    }
    else std::cout<<"can not find\n";
    return;
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


int levenshtein(std::string string_1, std::string string_2) { ///len(str_1)<len(str_2)
    std::string str_1(string_1);
    std::string str_2(string_2);
    if (str_1.size() > str_2.size()) {
        std::swap(str_1, str_2);
    }

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

