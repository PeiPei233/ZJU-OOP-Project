#include<iostream>
#include<vector>
#include<cassert>

#include "Vector.hpp"

// check whether std::vector and Vector are the same
template<class T>
void check(std::vector<T> v1, Vector<T> v2) {
    assert(v1.size() == v2.size());
    assert(v1.capacity() == v2.capacity());
    for (size_t i = 0; i < v1.size(); i++) {
        assert(v1[i] == v2[i] && v1.at(i) == v2.at(i));
    }
}

struct Point {
    int x, y;
    Point() = default;
    Point(int x, int y) : x(x), y(y) {}
    Point(const Point& r) : x(r.x), y(r.y) {}
    bool operator==(const Point& r) const {
        return x == r.x && y == r.y;
    }
};

int main() {
    
    // check constructor with one argument
    std::vector<int> v1(3);
    Vector<int> v2(3);
    check(v1, v2);
    // check at
    v1.at(2) = 3;   v2.at(2) = 3;
    check(v1, v2);
    // check out of range
    try {
        v2.at(-1) = 4;
    } catch (std::out_of_range& e) {
        std::cout << e.what() << std::endl;
    }
    try {
        v2.at(4) = 4;
    } catch (std::out_of_range& e) {
        std::cout << e.what() << std::endl;
    }
    check(v1, v2);

    // check push back with inflate
    for (int i = 0; i < 1e4; i++) {
        v1.push_back(i);    v2.push_back(i);
        check(v1, v2);
    }

    // check empty and clear
    assert(v1.empty() == false && v2.empty() == false);
    v1.clear();    v2.clear();
    check(v1, v2);
    assert(v1.empty() == true && v2.empty() == true);

    // check vector with user-defined type
    std::vector<Point> vp1;
    Vector<Point> vp2;
    check(vp1, vp2);
    vp1.push_back(Point(1, 2)); vp2.push_back(Point(1, 2));
    vp1.push_back(Point(2, 2)); vp2.push_back(Point(2, 2));
    vp1.push_back(Point(1, 3)); vp2.push_back(Point(1, 3));
    vp1.push_back(Point(1, 4)); vp2.push_back(Point(1, 4));
    vp1.push_back(Point(2, 1)); vp2.push_back(Point(2, 1));
    vp1.push_back(Point(1, 0)); vp2.push_back(Point(1, 0));
    check(vp1, vp2);

    // check copy constructor
    std::vector<Point> vp3 = vp1;
    Vector<Point> vp4 = vp2;
    check(vp3, vp4);
    vp3.clear();    vp4.clear();
    check(vp3, vp4);

    return 0;
}