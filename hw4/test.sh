echo -e "\033[32mShell: compile all the cpp file\033[0m"
clang++ ./pdadd.cpp -o pdadd -std=c++20
clang++ ./pdlist.cpp -o pdlist -std=c++20
clang++ ./pdremove.cpp -o pdremove -std=c++20
clang++ ./pdshow.cpp -o pdshow -std=c++20

echo -e "\033[32mShell: do some test\033[0m"
echo -e "\033[32mShell: test when data not created\033[0m"
./pdshow 2022-2-2
./pdremove 2022-2-2
./pdlist

echo -e "\033[32mShell: add diary for today:\033[0m"
./pdadd
echo -e "\033[32mShell: add diary for 2022/2/2\033[0m"
./pdadd 2022/2/2
echo -e "\033[32mShell: overwrite diary for 2022/2/2\033[0m"
./pdadd 2022-2.2

echo -e "\033[32mShell: invalid date input test\033[0m"
./pdadd 202202222
./pdadd 2022..2.//22
./pdadd 19788791
./pdadd 20220229
echo -e "\033[32mShell: add diary for 2022-2-29\033[0m"
./pdadd 20200229

echo -e "\033[32mShell: show diary for certain date\033[0m"
./pdshow 2022-2-2
echo -e "\033[32mShell: show diary for non-exists date\033[0m"
./pdshow 2023-2-2

echo -e "\033[32mShell: list all the diary\033[0m"
./pdlist

echo -e "\033[32mShell: list the diary from date to date\033[0m"
./pdlist 2020.1-1 2023-1-1

echo -e "\033[32mShell: list the diary from non-exists date\033[0m"
./pdlist 2024.1-1 2025-1-1


echo -e "\033[32mShell: remove diary for exists date\033[0m"
./pdremove 2022-2-2

echo -e "\033[32mShell: remove diary for non-exists date\033[0m"
./pdremove 2023-2-2

echo -e "\033[32mShell: list all the diary\033[0m"
./pdlist

echo -e "\033[32mShell: Clear compiled and bin file\033[0m"
rm diary.bin
rm pdadd
rm pdlist
rm pdremove
rm pdshow