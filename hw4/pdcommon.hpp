#pragma once

#include<map>
#include<string>
#include<fstream>
#include<vector>

class Diary {
private:
    std::map<size_t, std::string> diarys;   // key: date, value: diary
    std::string filename;                   // diary file name
public:
    Diary();                                // default constructor
    Diary(const std::string&);              // constructor with file name
    void load();                            // load diary from file
    void save() const;                      // save diary to file
    bool contains(size_t) const;            // query whether a diary exists by date
    std::string query(size_t) const;        // query diary by date
    std::vector<std::pair<size_t, std::string>> query(size_t, size_t) const;
                                            // query diary from min date to max date
    void add(size_t, const std::string&);   // add diary by date
    void remove(size_t);                    // remove diary by date
};

class Log {
public:
    enum class Level {
        INFO,
        WARNING,
        ERROR
    };
    Log() = default;                        // default constructor
    void log(const std::string&, Level) const;
                                            // log message
};

Diary::Diary() : filename("diary.bin") {}

Diary::Diary(const std::string& filename) : filename(filename) {}

void Diary::save() const {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    // write diarys to file
    for (auto& [date, diary] : diarys) {
        size_t date_n = date;
        ofs.write(reinterpret_cast<char*>(&date_n), sizeof(date_n));
        size_t diary_size = diary.size();
        ofs.write(reinterpret_cast<char*>(&diary_size), sizeof(diary_size));
        ofs.write(diary.c_str(), diary.size());
    }
    ofs.close();
}

void Diary::load() {
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    // read diarys from file
    while (!ifs.eof()) {
        size_t date;
        ifs.read(reinterpret_cast<char*>(&date), sizeof(date));
        if (ifs.eof()) {
            break;
        }
        size_t diary_size;
        ifs.read(reinterpret_cast<char*>(&diary_size), sizeof(diary_size));
        char* diary = new char[diary_size + 1];
        ifs.read(diary, diary_size);
        diary[diary_size] = '\0';
        diarys[date] = diary;
        delete[] diary;
    }
    ifs.close();
}

void Diary::add(size_t date, const std::string& diary) {
    diarys[date] = diary;
}

void Diary::remove(size_t date) {
    if (!diarys.contains(date)) {
        throw std::runtime_error("No diary found on " + std::to_string(date));
    }
    diarys.erase(date);
}

bool Diary::contains(size_t date) const {
    return diarys.contains(date);
}

std::string Diary::query(size_t date) const {
    try {
        return diarys.at(date);
    } catch (...) {
        throw std::runtime_error("No diary found on " + std::to_string(date));
    }
}

std::vector<std::pair<size_t, std::string>> Diary::query(size_t min_date, size_t max_date) const {
    std::vector<std::pair<size_t, std::string>> res;
    // query diarys from min date to max date
    // std::map is sorted by key: date, no need to sort again
    for (auto& diary: diarys) {
        size_t date = diary.first;
        if (date >= min_date && date <= max_date) {
            res.push_back(diary);
        }
    }
    return res;
}

void Log::log(const std::string& message, Level level = Level::INFO) const {
    std::string level_str, color;
    switch (level) {
        case Level::INFO:       // Cyan color
            level_str = "INFO";
            color = "\033[36m";
            break;
        case Level::WARNING:    // Yellow color
            level_str = "WARNING";
            color = "\033[33m";
            break;
        case Level::ERROR:      // Red color
            level_str = "ERROR";
            color = "\033[31m";
            break;
    }
    std::cout << color << level_str << ": " << message << "\033[0m" << std::endl;
}

bool check_date(size_t year, size_t month, size_t day) {
    // check if month valid
    if (month < 1 || month > 12 || day < 1 || day > 31) {
        return false;
    }
    // check if day valid
    if (month == 2) {
        if ((year % 400 == 0) || (year % 100 != 0 && year % 4 == 0)) {
            if (day > 29) {
                return false;
            }
        } else {
            if (day > 28) {
                return false;
            }
        }
    } else if (month == 4 || month == 6 || month  == 9 || month == 11) {
        if (day > 30) {
            return false;
        }
    }
    return true;
}

bool check_date(size_t date) {
    return check_date(date / 10000, date / 100 % 100, date % 100);
}

// convert date string to date number
size_t stodate(const std::string& date_str) {

    size_t year, month, day, date;
    try {
        date = std::stoi(date_str);
        if (!check_date(date)) throw -1;
    } catch (...) {
        size_t pos1, pos2;
        try {
            year = std::stoi(date_str, &pos1);
            pos1 += 1;
            month = std::stoi(date_str.substr(pos1), &pos2);
            pos2 += pos1 + 1;
            day = std::stoi(date_str.substr(pos2));
            if (!check_date(year, month, day)) throw -1;
            date = year * 10000 + month * 100 + day;
        } catch (...) {
            throw std::runtime_error("Invalid date: " + date_str);
        }
    }

    return date;
}