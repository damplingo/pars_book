#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <clocale>
#include <locale>
#include <codecvt>
#include "search.h"
#include "contents.h"
#include <pqxx/pqxx>



int main() {
    setlocale(LC_ALL, "");
    //Search search("7110-1.txt", "7110.csv", "add_tag.txt");
    //std::string a = "Упорядочение хаоса";
    //search.find_text(a);
    /*search.add_tags_all();
    get_by_tag(64, "add_tag.txt", "get.txt");*/

     try {
        pqxx::connection C("dbname=book_info user=postgres password=postgres\
                            hostaddr=127.0.0.1 port=5432");
        if (C.is_open()) {
            std::cout << "Opened database successfully: " << C.dbname() << std::endl;
        } else {
            std::cout << "Can't open database" << std::endl;
            return 1;
        }
        
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}