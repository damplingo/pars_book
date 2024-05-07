#include "contents.h"

Chapter::Chapter(int _id, int _ordernum, std::string _index, std::string _title, int start_page_,
        int end_page_, int level_): id(_id), ordernum(_ordernum), index(_index), title(_title),
        start_page(start_page_), end_page(end_page_), level(level_) {}

int Chapter::get_id() {
    return id;
}

int Chapter::get_start() {
    return start_page;
}

int Chapter::get_end() {
    return end_page;
}

int Chapter::get_level() {
    return level;
}

std::string& Chapter::get_title() {
    return title;
}

int Chapter::get_ordernum() {
    return ordernum;
}

Contents::Contents(std::string file_name_): file_name(file_name_) {
    this->size = 0;
    std::ifstream inf(file_name);
    if (!inf){
        std::cout<<"There is not file\n";
        return; //выкинуть exeption
    }

    std::string input_str;
    std::getline(inf, input_str);
    while (std::getline(inf, input_str)) {
        std::istringstream str(input_str);
        std::string line;
        int count = 0;
        int id, ordernum, start_page, end_page, level;
        std::string title, index;

        while (std::getline(str, line, ';')) {
            if (count == 0) {
                id = stoi(line);
            }
            if (count == 2) {
                ordernum = stoi(line);
            }

            if (count == 3) {
                index = line;
            }

            if (count == 4) {
                title = line;
            }

            if (count == 5) {
                start_page = stoi(line);
            }

            if (count == 6) {
                end_page = stoi(line);
            }

            if (count == 8) {
                level = stoi(line);
            }
            count += 1; 
        }

        if (count != 0){
                Chapter a(id, ordernum, index, title, start_page, end_page, level);
                contents.push_back(a);
                this->size += 1;
        }
    }

    
}

int Contents::get_size(){
    return size;
}

Chapter &Contents::operator[](size_t idx) {
    return contents[idx];
}

std::string& Chapter::get_index() {
    return index;
}

bool Contents::exist(const std::string& name) {
    for (int i = 0; i < size; ++i) {
        if (contents[i].get_title() == name) {
            return true;
        }
    }
    return false;
}

Chapter& Contents::find(const std::string& name) {
    if (exist(name)) {
        for (int i = 0; i < size; ++i) {
            if (contents[i].get_title() == name) {
                return contents[i];
            }
        }
    }
    std::cout<<"have not this content";
}

Chapter& Contents::get_next(Chapter& chapter) {
    for (int i = chapter.get_ordernum(); i < size; ++i) {
        if (contents[i].get_start() == chapter.get_end() && contents[i].get_level() <= chapter.get_level()) { //вернет первый следующий заголовок(работает и в ситуации конца раздела)
            return contents[i];
        }
    }
}
