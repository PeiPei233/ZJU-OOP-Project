#include<iostream>
#include<ctime>
#include<string>
#include<sstream>

#include "pdcommon.hpp"

int main(int argc, char** argv) {
    Diary diary;
    Log log;
    try {
        diary.load();
    } catch (std::runtime_error& e) {
        log.log("No diary file found.");
        return 0;
    }
    size_t min_date = 0, max_date = -1;
    if (argc == 3) {    // if no date is specified, list all diaries
        try {
            min_date = stodate(argv[1]);
            max_date = stodate(argv[2]);
        } catch (...) {
            std::stringstream ss;
            ss << "Invalid date format: " << argv[1] << " or " << argv[2] << ".";
            log.log(ss.str(), Log::Level::ERROR);
            return -1;
        }
    } else if (argc == 2) { // if a date is specified, list diary on that day
        try {
            min_date = stodate(argv[1]);
            max_date = min_date;
        } catch (...) {
            std::stringstream ss;
            ss << "Invalid date format: " << argv[1] << ".";
            log.log(ss.str(), Log::Level::ERROR);
            return -1;
        }
    }
    // query the diary
    auto res = diary.query(min_date, max_date);
    if (argc == 3) {
        std::cout << "Find " << res.size() << " diary(s) between "
                << min_date / 10000 << "-" << min_date / 100 % 100 << "-" << min_date % 100 
                << " and " << max_date / 10000 << "-" << max_date / 100 % 100 << "-" << max_date % 100 
                << "." << std::endl;
    } else {
        std::cout << "Find " << res.size() << " diary(s) in total." << std::endl;
    }
    // print the diary
    for (auto& [date, diary_content]: res) {
        std::cout << "########## Diray: " << date / 10000 << "-" << date / 100 % 100 << "-" << date % 100 << " ##########" << std::endl;
        std::cout << diary_content << std::endl;
    }



    return 0;
}