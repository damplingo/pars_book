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



int main() {
    setlocale(LC_ALL, "");
    Search search("7110-1.txt", "7110.csv", "add_tag.txt");
    std::string a = "Упорядочение хаоса";
    //search.find_text(a);
    //search.add_tags_for_title(a);*/
    //search.find_title(a);
    search.add_tags_all();
    return 0;
}