#include <array>
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <functional>
#include <memory>
#include <optional>
#include <vector>
#include "Zipper/Zipper.hpp"

using std::nullopt;

static const std::array<std::function<void(int, char)>, 4> expected1 = {
    ([](int x1, char x2) { cr_assert(x1 == x2 && x1 == 1); }),
    ([](int x1, char x2) { cr_assert(x1 == x2 && x1 == 2); }),
    ([](int x1, char x2) { cr_assert(x1 == x2 && x1 == 3); }),
    ([](int, char) { cr_assert_fail("Zipper out of range"); }),
};

Test(zipper_tests, basic_zipper_test)
{
    std::vector<std::optional<int>> v1 = {1, nullopt, 2, 84, nullopt, nullopt, 3};
    std::vector<std::optional<char>> v2 = {1, 84, 2, nullopt, 84, nullopt, 3};

    unsigned i = 0;
    for (auto &&[x1, x2] : Zipper(v1, v2)) {
        expected1[i](x1, x2);
        i++;
    }
}

Test(zipper_tests, unique_zipper)
{
    std::array<int *, 7> vals1 = {new int(1), nullptr, new int(2), new int(84), nullptr, nullptr, new int(3),};
    std::array<char *, 7> vals2 = {new char(1), new char(84), new char(2), nullptr, new char(84), nullptr, new char(3),};

    std::vector<std::unique_ptr<int>> v1;
    std::vector<std::unique_ptr<char>> v2;

    for (auto x : vals1) {
        v1.push_back(std::unique_ptr<int>(x));
    }
    for (auto x : vals2) {
        v2.push_back(std::unique_ptr<char>(x));
    }

    unsigned i = 0;
    for (auto &&[x1, x2] : Zipper(v1, v2)) {
        expected1[i](x1, x2);
        i++;
    }
}

Test(zipper_tests, zipper_with_first_invalid)
{
    std::vector<std::optional<int>> v1 = {nullopt, 1, nullopt, 2, 84, nullopt, nullopt, 3};
    std::vector<std::optional<char>> v2 = {84, 1, 84, 2, nullopt, 84, nullopt, 3};

    unsigned i = 0;
    for (auto &&[x1, x2] : Zipper(v1, v2)) {
        expected1[i](x1, x2);
        i++;
    }
}
