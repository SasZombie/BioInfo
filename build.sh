DIRECTORY="build"

if [ ! -d "$DIRECTORY" ]; then
    mkdir "$DIRECTORY"
fi

g++ TenthLab.cpp -o build/main -O3 -std=c++23 -Wall -Wextra -Wformat-nonliteral -Wcast-align -Wpointer-arith -Wmissing-declarations -Winline -Wundef -Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter -Wfloat-equal -pedantic -fsanitize=address -fsanitize=leak
./build/main