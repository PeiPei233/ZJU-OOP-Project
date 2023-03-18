#include<iostream>
#include<iomanip>
#include<map>
#include<set>
#include<string>
#include<vector>

// Store the information of a student
struct Student {
    std::string name;
    size_t no;
    std::map<std::string, int> scores;
    int tot_score;
    int num_courses;
    Student(const std::string& name, size_t no) : name(name), no(no), tot_score(0), num_courses(0), scores({}) {}
    double average() const {
        return tot_score / static_cast<double>(num_courses);
    }
};

// Store the table of scores
struct ScoreTable {
    std::map<std::string, Student> students;    // Map from student name to student
    std::set<std::string> courses;              // Set of courses
    std::map<size_t, std::string> Snos;         // Map from student number to student name
    void addStudent(const std::string& name) {
        students.insert({name, Student(name, students.size() + 1)});
        Snos.insert({students.size(), name});
    }
    void addScore(const std::string& name, const std::string& course, const int score) {
        if (students.count(name) == 0) {
            addStudent(name);
        }
        // If the student has already taken the course, update the score
        if (students.at(name).scores.count(course) != 0) {
            students.at(name).tot_score -= students.at(name).scores.at(course);
            students.at(name).scores.at(course) = score;
            students.at(name).tot_score += score;
            std::cout << "Student " << name << " has already taken course " 
                      << course << " and the score now will be updated to " << score << std::endl;
        } else {
            students.at(name).scores.insert({course, score});
            students.at(name).tot_score += score;
            students.at(name).num_courses++;
        }
        courses.insert(course);        
    }
};

// Store the information of a course
struct CourseInfo {
    int tot_score;
    int num_students;
    int min_score;
    int max_score;
    CourseInfo() : tot_score(0), num_students(0), min_score(5), max_score(0) {}
    double average() const {
        return tot_score / static_cast<double>(num_students);
    }
};

// Print the table of scores
std::ostream& operator<<(std::ostream& os, const ScoreTable& table) {
    std::map<std::string, CourseInfo> courses;
    // Print the header
    os << std::left << std::setw(8) << "no"
       << std::setw(8) << "name";
    for (const auto& c : table.courses) {
        os << std::setw(8) << c.substr(0, 7);
    }
    os << std::setw(8) << "average" << std::endl;

    // Print the scores sorted by student number
    for (size_t i = 0; i < table.students.size(); i++) {
        auto name = table.Snos.at(i + 1);
        os << std::setw(8) << i + 1
           << std::setw(8) << name.substr(0, 7);
        for (const auto& c : table.courses) {
            // If the student has not taken the course, print "-"
            if (table.students.at(name).scores.count(c) == 0) {
                os << std::setw(8) << "-";
            } else {
                os << std::setw(8) << table.students.at(name).scores.at(c);
                // Update the information of the course
                courses[c].tot_score += table.students.at(name).scores.at(c);
                courses[c].num_students++;
                courses[c].min_score = std::min(courses[c].min_score, table.students.at(name).scores.at(c));
                courses[c].max_score = std::max(courses[c].max_score, table.students.at(name).scores.at(c));
            }
        }
        os << std::setw(8) << table.students.at(name).average() << std::endl;
    }
    
    // Print the average, min, and max scores of each course
    os << std::setw(8) << ""
       << std::setw(8) << "average";
    for (const auto& c : table.courses) {
        os << std::setw(8) << courses[c].average();
    }
    os << std::endl;

    os << std::setw(8) << ""
       << std::setw(8) << "min";
    for (const auto& c : table.courses) {
        os << std::setw(8) << courses[c].min_score;
    }
    os << std::endl;

    os << std::setw(8) << ""
       << std::setw(8) << "max";
    for (const auto& c : table.courses) {
        os << std::setw(8) << courses[c].max_score;
    }
    os << std::endl;

    return os;
}

// Split a string into a vector of strings
std::vector<std::string> split(const std::string& str) {
    std::vector<std::string> res;
    size_t start = 0;
    size_t end = str.find_first_of(' ');
    while (end != std::string::npos) {
        if (end != start) res.push_back(str.substr(start, end - start));
        start = end + 1;
        end = std::min(str.find_first_of(' ', start), str.find_first_of('\t', start));
    }
    res.push_back(str.substr(start, end - start));
    return res;
}

/*
Input Sample:
Alice Algorithms 5 Math 4 DataStructures 2
Alice DataStructures
Bob DataStructures 3 Algorithms 4 History 4
Charlie English 5 Algorithms 3 Physics 4
David Biology 4 History 5 Math 4
Emma History 4 Biology 4 English 4
Frank Math 4 English 5 Biology 3
Grace DataStructures 5 Physics 4 Algorithms 5
Henry Biology 3 Physics 5 History 4
Ivy English 3 DataStructures 3 Physics 3
John DataStructures A+ Algorithms 2 English 2
John DataStructures 3 Algorithms 1
*/

int main() {

    std::string line;
    ScoreTable table;
    // Read the input
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        auto info = split(line);
        table.addStudent(info[0]);
        for (size_t i = 1; i < info.size(); i += 2) {
            if (i + 1 >= info.size()) {
                std::cout << "Lack of score for course " << info[i] << " taken by " << info[0] << std::endl;
            } else {
                try {
                    table.addScore(info[0], info[i], std::stoi(info[i + 1]));
                } catch (const std::invalid_argument& e) {
                    std::cout << "Invalid score " << info[i + 1] << " for course " << info[i] << " taken by " << info[0] << std::endl;
                    continue;
                }
            }
        }
    }

    std::cout << table << std::endl;

    return 0;
}