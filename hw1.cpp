#include<iostream>
#include<string>
#include<vector>
#include<iomanip>

struct Student {
    std::string name;
    int score1, score2, score3;
    double average;
    Student(const std::string_view name, int score1, int score2, int score3) : 
        name(name), 
        score1(score1), 
        score2(score2), 
        score3(score3),
        average((score1 + score2 + score3) / 3.) {}
};

struct ScoreTable {
    std::vector<Student> v;
    void insert(const std::string_view name, int score1, int score2, int score3) {
        v.push_back(Student(name, score1, score2, score3));
    }
};

std::ostream& operator<<(std::ostream& os, const ScoreTable& st) {
    os << std::left << std::setw(8) << "no"
       << std::setw(8) << "name"
       << std::setw(8) << "score1"
       << std::setw(8) << "score2"
       << std::setw(8) << "score3"
       << std::setw(8) << "average" << std::endl;
    double average1{}, average2{}, average3{};
    int min1{5}, min2{5}, min3{5};
    int max1{}, max2{}, max3{};
    for (int i{}; i < st.v.size(); i++) {
        os << std::left << std::setw(8) << i + 1
           << std::setw(8) << st.v[i].name.substr(0, 7)
           << std::setw(8) << st.v[i].score1
           << std::setw(8) << st.v[i].score2
           << std::setw(8) << st.v[i].score3
           << std::setw(8) << st.v[i].average << std::endl;
        average1 += st.v[i].score1;
        average2 += st.v[i].score2;
        average3 += st.v[i].score3;
        if (st.v[i].score1 < min1) min1 = st.v[i].score1;
        if (st.v[i].score2 < min2) min2 = st.v[i].score2;
        if (st.v[i].score3 < min3) min3 = st.v[i].score3;
        if (st.v[i].score1 > max1) max1 = st.v[i].score1;
        if (st.v[i].score2 > max2) max2 = st.v[i].score2;
        if (st.v[i].score3 > max3) max3 = st.v[i].score3;
    }
    average1 /= st.v.size();
    average2 /= st.v.size();
    average3 /= st.v.size();
    os << std::left << std::setw(8) << ""
       << std::setw(8) << "average"
       << std::setw(8) << average1
       << std::setw(8) << average2
       << std::setw(8) << average3
       << std::setw(8) << "" << std::endl;
    os << std::left << std::setw(8) << ""
       << std::setw(8) << "min"
       << std::setw(8) << min1
       << std::setw(8) << min2
       << std::setw(8) << min3
       << std::setw(8) << "" << std::endl;
    os << std::left << std::setw(8) << ""
       << std::setw(8) << "max"
       << std::setw(8) << max1
       << std::setw(8) << max2
       << std::setw(8) << max3
       << std::setw(8) << "" << std::endl;
    return os;
}

/*
Input Sample:

K.Weng  5       5       5
T.Dixon 4       3       2
V.Chu   3       4       5
L.Tson  4       3       4
L.Lee   3       4       3
I.Young 4       2       5
K.Hiro  4       2       1
G.Ping  4       4       4
H.Gu    2       3       4
J.Jon   5       4       3

*/

int main() {

    ScoreTable st{};
    while (true) {
        std::string name;
        int score1, score2, score3;
        std::cin >> name >> score1 >> score2 >> score3;
        if (std::cin.eof()) break;
        else if (std::cin.fail()) {
            std::cin.clear();
            std::cout << "Invalid input! Please check again :-)" << std::endl;
            continue;
        }
        st.insert(name, score1, score2, score3);
    }
    std::cout << st;

    return 0;
}