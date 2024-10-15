
#include <iostream>
#include <optional>
#include <vector>
#include "src/ECS/utilities/Zipper.hpp"

using std::nullopt;

template<typename T>
using sparse = std::vector<std::optional<T>>;

int main()
{
    sparse<std::string> stringVec = {
        "hello", 
        nullopt, 
        "zz", 
        "Fav:", 
        nullopt,
        "",
        "finish",
        nullopt,
        nullopt,
    };
    sparse<int> intVec = {
        15,
        2,
        nullopt,
        48,
        nullopt,
        nullopt,
        5
    };

    std::vector<std::optional<char>> empty{};

    for (auto &&[str, num, z] : Zipper(stringVec, intVec, empty)) {
        std::cout << str << " " << num << std::endl;
    }
}
