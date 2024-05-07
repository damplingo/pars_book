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

///управление через аргументы командной строки
///0 filename.txt output.txt - разметить filename.txt и записать размеченный файл в output.txt
///1 filename.txt - занести в базу глав filename.txt

int main(int argc, char** argv) {

    setlocale(LC_ALL, "");

    if (argc < 2) {
        std::cout<<"not enough arguments\n";
        return 1;
    }

    if (*argv[1] == '0') {
        std::ifstream inf(argv[2]);
        if (!inf) {
            std::cout<<"input file does not exist\n";
            return 0;
        }
        Search search(argv[2], "7110.csv", argv[3]);
        Contents contents("7110.csv");
        search.add_tags_all();
    }

    else if (*argv[1] == '1') {
        std::ifstream inf(argv[2]);
        if (!inf) {
            std::cout<<"input file does not exist\n";
            return 0;
        }
        std::string text = argv[2];
        try {
        pqxx::connection test = pqxx::connection("dbname=book_info user=postgres password=postgres\
                            hostaddr=127.0.0.1 port=5432");
                            
        if (test.is_open()) {
            std::cout << "Opened database successfully: " << test.dbname() << std::endl;
        } else {
            std::cout << "Can't open database" << std::endl;
            return 1;
        }
        
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
        pqxx::connection C = pqxx::connection("dbname=book_info user=postgres password=postgres\
                            hostaddr=127.0.0.1 port=5432");
        pqxx::work W(C);

        Contents contents("7110.csv");
        for (int i = 0; i < contents.get_size(); ++i) {
            std::string res = get_by_tag(contents[i].get_ordernum(), text, "get.txt");
            std::string query = "UPDATE public.in_doc_contents SET content = $$" + get_by_tag(contents[i].get_ordernum(),text, "get.txt")+"$$" + "WHERE id = " +
            std::to_string(contents[i].get_id()) + ";";
            W.exec(query);
        }

        W.commit();
    }
    else {
        std::cout<<"error arguments\n";
    }

    return 0;
} 