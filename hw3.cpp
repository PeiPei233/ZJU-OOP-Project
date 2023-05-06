#include<iostream>
#include<chrono>
#include<random>
#include<vector>
#include<map>

// class to represent a room in the castle
struct Room {
    Room* north;
    Room* south;
    Room* west;
    Room* east;
    int id;
    Room(int id) : north(nullptr), south(nullptr), west(nullptr), east(nullptr), id(id) {}
};

// enum class to represent the direction
enum class Direction {
    North,
    South,
    West,
    East
};

// class to represent the castle
struct Castle {
private:
    int n;              // number of rooms
    Room** rooms;       // array of rooms
    int currentRoom;    // current room
    int princessRoom;   // room with the princess
    int monsterRoom;    // room with the monster
public:
    // constructor to create a castle with n rooms and a random seed
    Castle(int n, long long seed = -1) : n(n) {
        rooms = new Room*[n];
        for (int i = 0; i < n; ++i) {
            rooms[i] = new Room(i);
        }
        // if seed is -1, use the current time as the seed
        if (seed == -1) {
            seed = std::chrono::system_clock::now().time_since_epoch().count();
        }
        // print the seed
        std::cout << "Seed: " << seed << std::endl;
        std::mt19937 gen(seed);
        std::uniform_int_distribution<int> r(0, n - 1);
        // shuffle the rooms
        int *a = new int[n];
        for (int i = 0; i < n; ++i) {
            a[i] = i;
        }
        for (int i = 0; i < n; ++i) {
            int j = r(gen);
            std::swap(a[i], a[j]);
        }

        std::map<std::pair<int, int>, Room*> point2room;    // map from a point to a room
        std::map<int, std::pair<int, int>> room2point;      // map from a room to a point
        point2room[{0, 0}] = rooms[a[0]];                   // the first room is at (0, 0)
        room2point[a[0]] = {0, 0};
        for (int i = 1; i < n; i++) {
            std::uniform_int_distribution<int> rj(0, i - 1);
            do {
                // randomly choose a room that has been placed
                int j = rj(gen);
                std::pair<int, int> p = room2point[a[j]];
                std::uniform_int_distribution<int> rd(0, 3);
                // randomly choose a direction to place the room
                int d = rd(gen);
                // if no room has not been placed in the chosen direction of the chosen room, place it
                if (d == 0) {
                    if (point2room.count({p.first, p.second + 1}) == 0) {
                        point2room[{p.first, p.second + 1}] = rooms[a[i]];
                        room2point[a[i]] = {p.first, p.second + 1};
                        break;
                    }
                } else if (d == 1) {
                    if (point2room.count({p.first, p.second - 1}) == 0) {
                        point2room[{p.first, p.second - 1}] = rooms[a[i]];
                        room2point[a[i]] = {p.first, p.second - 1};
                        break;
                    }
                } else if (d == 2) {
                    if (point2room.count({p.first - 1, p.second}) == 0) {
                        point2room[{p.first - 1, p.second}] = rooms[a[i]];
                        room2point[a[i]] = {p.first - 1, p.second};
                        break;
                    }
                } else if (d == 3) {
                    if (point2room.count({p.first + 1, p.second}) == 0) {
                        point2room[{p.first + 1, p.second}] = rooms[a[i]];
                        room2point[a[i]] = {p.first + 1, p.second};
                        break;
                    }
                }           
            } while (true);            
        }
        // Turn the map into a graph
        for (auto& r : point2room) {
            auto p = r.first;
            auto room = r.second;
            if (point2room.count({p.first, p.second + 1}) != 0) {
                room->north = point2room[{p.first, p.second + 1}];
            }
            if (point2room.count({p.first, p.second - 1}) != 0) {
                room->south = point2room[{p.first, p.second - 1}];
            }
            if (point2room.count({p.first - 1, p.second}) != 0) {
                room->west = point2room[{p.first - 1, p.second}];
            }
            if (point2room.count({p.first + 1, p.second}) != 0) {
                room->east = point2room[{p.first + 1, p.second}];
            }
        }
        // randomly choose the room with the princess and the room with the monster
        currentRoom = 0;
        do {
            princessRoom = r(gen);
        } while (princessRoom == currentRoom);
        do {
            monsterRoom = r(gen);
        } while (monsterRoom == currentRoom || monsterRoom == princessRoom);
    }
    ~Castle() {
        for (int i = 0; i < n; ++i) {
            delete rooms[i];
        }
        delete[] rooms;
    }
    // move to the room in the given direction
    bool go(Direction d) {
        switch (d) {
            case Direction::North:
                if (rooms[currentRoom]->north != nullptr) {
                    currentRoom = rooms[currentRoom]->north->id;
                    return true;
                }
                break;
            case Direction::South:
                if (rooms[currentRoom]->south != nullptr) {
                    currentRoom = rooms[currentRoom]->south->id;
                    return true;
                }
                break;
            case Direction::East:
                if (rooms[currentRoom]->east != nullptr) {
                    currentRoom = rooms[currentRoom]->east->id;
                    return true;
                }
                break;
            case Direction::West:
                if (rooms[currentRoom]->west != nullptr) {
                    currentRoom = rooms[currentRoom]->west->id;
                    return true;
                }
                break;
        }
        return false;
    }
    bool isPrincessRoom() {
        return currentRoom == princessRoom;
    }
    bool isMonsterRoom() {
        return currentRoom == monsterRoom;
    }
    bool isLobby() {
        return currentRoom == 0;
    }
    // get the name of the current room
    std::string getCurrentRoom() {
        if (currentRoom == 0) {
            return "the lobby";
        } else if (currentRoom == princessRoom) {
            return "princess room";
        } else if (currentRoom == monsterRoom) {
            return "the hell";
        } else {
            return "Room" + std::to_string(currentRoom);
        }
    }
    // get the description of the exit of current room
    int getCurrentExit() {
        int ret = 0;
        if (rooms[currentRoom]->north != nullptr) {
            ret |= 1;
        }
        if (rooms[currentRoom]->south != nullptr) {
            ret |= 2;
        }
        if (rooms[currentRoom]->west != nullptr) {
            ret |= 4;
        }
        if (rooms[currentRoom]->east != nullptr) {
            ret |= 8;
        }
        return ret;
    }
};

// get the description of the exit of current room
auto RoomInfo(int currentExit) {
    int numRoom = 0;
    std::string direction;
    if (currentExit & 1) {
        numRoom++;
        direction = "up";
    }
    if (currentExit & 2) {
        numRoom++;
        if (numRoom == 2) {
            direction = "down and " + direction;
        } else if (numRoom == 1) {
            direction = "down";
        } else {
            direction = "down, " + direction;
        }
    }
    if (currentExit & 4) {
        numRoom++;
        if (numRoom == 2) {
            direction = "west and " + direction;
        } else if (numRoom == 1) {
            direction = "west";
        } else {
            direction = "west, " + direction;
        }
    }
    if (currentExit & 8) {
        numRoom++;
        if (numRoom == 2) {
            direction = "east and " + direction;
        } else if (numRoom == 1) {
            direction = "east";
        } else {
            direction = "east, " + direction;
        }
    }
    return std::pair<int, std::string> {numRoom, direction};
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
    if (res[0] == "") res.erase(res.begin());
    return res;
}

int main() {

    std::cout << "Please enter the seed (-1 caused new random every time): ";
    long long seed;
    std::cin >> seed;
    std::cout << "Please enter the number of rooms: ";
    int n;
    std::cin >> n;
    Castle castle(n, seed);
    std::cout << std::endl;
    bool princessFound = false;
    while (true) {
        if (castle.isPrincessRoom() && !princessFound) {    // if the princess is found, the player can go to lobby
            princessFound = true;
            std::cout << "Welcome to " << castle.getCurrentRoom() << "." << std::endl;
            std::cout << "I know that you will find me, sweetie! Now let's go to lobby!" << std::endl;
            auto [numRoom, direction] = RoomInfo(castle.getCurrentExit());
            std::cout << "My room have " << numRoom << " exits: " << direction << "." << std::endl;
        } else if (castle.isMonsterRoom()) {                // if the player is in the monster room, the player is dead
            std::cout << "Welcome to " << castle.getCurrentRoom() << "." << std::endl;
            std::cout << "You are dead!" << std::endl;
            break;
        } else if (princessFound && castle.isLobby()) {     // if the player is in the lobby and have found the princess, the player wins
            std::cout << "Welcome to " << castle.getCurrentRoom() << "." << std::endl;
            std::cout << "We get it!" << std::endl;
            break;
        } else {
            std::cout << "Welcome to " << castle.getCurrentRoom() << ". ";
            auto [numRoom, direction] = RoomInfo(castle.getCurrentExit());
            std::cout << "There are " << numRoom << " exits: " << direction << "." << std::endl;
        }
        std::cout << "Enter your command: ";
        do {
            // get the command
            std::string com;
            std::getline(std::cin, com);
            auto coms = split(com);
            if (coms.size() == 0) continue;
            if (coms.size() != 2 || coms[0] != "go") {
                std::cout << "Wrong command, please retry: ";
            } else {
                if (coms[1] == "up") {
                    if (castle.go(Direction::North)){
                        std::cout << std::endl;
                        break;
                    } else {
                        std::cout << "There is no exit up, please retry: ";
                    }
                } else if (coms[1] == "down") {
                    if (castle.go(Direction::South)){
                        std::cout << std::endl;
                        break;
                    } else {
                        std::cout << "There is no exit down, please retry: ";
                    }
                } else if (coms[1] == "west") {
                    if (castle.go(Direction::West)){
                        std::cout << std::endl;
                        break;
                    } else {
                        std::cout << "There is no exit in the west, please retry: ";
                    }
                } else if (coms[1] == "east") {
                    if (castle.go(Direction::East)){
                        std::cout << std::endl;
                        break;
                    } else {
                        std::cout << "There is no exit in the east, please retry: ";
                    }
                } else {
                    std::cout << "Wrong command, please retry: ";
                }
            }
        } while (true);
    }

    

    return 0;
}