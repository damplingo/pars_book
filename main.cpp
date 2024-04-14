#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <clocale>
#include "search.h"
#include "contents.h"



int main() {
    
    Search search("7110-1.txt", "7110.csv");
    std::string a = "Поколения языков программирования";
    //search.find_text(a);
    search.add_tags_for_title("add_tag.txt", a);
    //search.find_title(a);
    return 0;
}