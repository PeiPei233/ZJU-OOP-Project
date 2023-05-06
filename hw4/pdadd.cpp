#include<iostream>
#include<ctime>
#include<string>
#include<sstream>

#include "pdcommon.hpp"

int main(int argc, char** argv) {
    Diary diary;    // the diary database
    Log log;        // the log helper
    // try to load the diary file
    try {
        diary.load();
    } catch (std::runtime_error& e) {
        log.log("No diary file found, creating a new one.");
    }
    std::string diary_content {};   // the content of the diary
    std::string tmp;
    size_t date;                    // the date of the diary
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
    // check if the diary for the date already exists
    if (diary.contains(date)) {
        std::stringstream ss;
        ss << "Diary for date " 
           << date / 10000 << "-" << date / 100 % 100 << "-" << date % 100 
           << " exists and will be overwriten.";
        log.log(ss.str(), Log::Level::WARNING);
    }
    // get the content of the diary
    std::cout << "########## Diary: " << date / 10000 << "-" << date / 100 % 100 << "-" << date % 100 << " ##########" << std::endl;

    while (std::getline(std::cin, tmp)) {
        if (tmp == ".") {
            break;
        }
        diary_content += tmp + "\n";
    }
    // add the diary to the diary file
    diary.add(date, diary_content);
    // save the diary file
    try {
        diary.save();
        log.log("Diary saved.");
    } catch (std::runtime_error& e) {
        log.log(e.what(), Log::Level::ERROR);
    } 
    return 0;
}