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
        log.log("No diary file found, removing suspended.");
        return 0;
    }
    size_t date;
    if (argc == 1) {        // If no date is specified, remove the diary for today.
        std::time_t now = std::time(nullptr);
        std::tm local_time = *std::localtime(&now);
        int year = local_time.tm_year + 1900;
        int month = local_time.tm_mon + 1;
        int day = local_time.tm_mday;
        date = year * 10000 + month * 100 + day;
    } else {                // If a date is specified, remove the diary for that date.
        try {
            date = stodate(argv[1]);
        } catch (...) {
            std::stringstream ss;
            ss << "Invalid date format: " << argv[1] << ".";
            log.log(ss.str(), Log::Level::ERROR);
            return -1;
        }
    }
    // Remove the diary
    try {
        diary.remove(date);
    } catch (...) {
        std::stringstream ss;
        ss << "Remove the diary for " << date / 10000 << "-" << date / 100 % 100 << "-" << date % 100 << " failed!";
        log.log(ss.str(), Log::Level::ERROR);
        return -1;
    }
    // Save the diary
    try {
        diary.save();
    } catch (std::runtime_error& e) {
        log.log(e.what(), Log::Level::ERROR);
        return -1;
    }

    std::stringstream ss;
    ss << "Remove the diary for " << date / 10000 << "-" << date / 100 % 100 << "-" << date % 100 << " succeeded.";
    log.log(ss.str(), Log::Level::INFO);

    return 0;
}