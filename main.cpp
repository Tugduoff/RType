
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

    for (auto &&[str, num] : Zipper(stringVec, intVec)) {
        if (!str || !num) {
            std::cerr << "Oops, zipper failed" << std::endl;
        }
        std::cout << *str << " " << *num << std::endl;
    }
}
