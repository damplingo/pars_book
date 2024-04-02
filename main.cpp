#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "search.h"
#include "contents.h"


int main() {
    
    Search searsh("7110-1.txt", "7110.csv");
    std::string a = "Объектная модель";
    searsh.find_text(a);
    /*std::transform(low_title.begin(), low_title.end(), low_title.begin(),
    [](wchar_t c){ return std::towlower(c);});*/

    /*std::wstring low_title;
    std::wcin>>low_title;
    for (wchar_t& ch : low_title) {
        ch = std::towlower(ch);
    }
    outf<<low_title;*/

    return 0;
}