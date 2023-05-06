#include<iostream>
#include<ctime>
#include<string>
#include<sstream>

#include "pdcommon.hpp"

int main(int argc, char** argv) {
    Diary diary;        // the diary database
    Log log;            // the log helper
    try {
        diary.load();
    } catch (std::runtime_error& e) {
        log.log("No diary file found.");
        return 0;
    }
    size_t date;
    if (argc == 1) {        // if no date is specified, use today's date
        std::time_t now = std::time(nullptr);
        std::tm local_time = *std::localtime(&now);
        int year = local_time.tm_year + 1900;
        int month = local_time.tm_mon + 1;
        int day = local_time.tm_mday;
        date = year * 10000 + month * 100 + day;
    } else {                // if a date is specified, use it
        try {
            date = stodate(argv[1]);
        } catch (...) {
            std::stringstream ss;
            ss << "Invalid date format: " << argv[1] << ".";
            log.log(ss.str(), Log::Level::ERROR);
            return -1;
        }
    }
    // query the diary
    std::string diary_content;
    try {
        diary_content = diary.query(date);
    } catch (std::runtime_error& e) {
        log.log(e.what(), Log::Level::ERROR);
        return 0;
    }
    // print the diary
    std::cout << "########## Diray: " << date / 10000 << "-" << date / 100 % 100 << "-" << date % 100 << " ##########" << std::endl;
    std::cout << diary_content << std::endl;


    return 0;
}