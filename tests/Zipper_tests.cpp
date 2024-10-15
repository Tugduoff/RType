#include <array>
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <functional>
#include <optional>
#include <vector>
#include "Zipper/Zipper.hpp"

using std::nullopt;

Test(zipper_tests, basic_zipper_test)
{
    std::vector<std::optional<int>> v1 = {1, nullopt, 2, 84, nullopt, nullopt, 3};
    std::vector<std::optional<unsigned>> v2 = {1, 84, 2, nullopt, 84, nullopt, 3};

    std::array<std::function<void(int, unsigned)>, 4> expected = {
        ([](int x1, unsigned x2) { cr_assert(x1 == x2 && x1 == 1); }),
        ([](int x1, unsigned x2) { cr_assert(x1 == x2 && x1 == 2); }),
        ([](int x1, unsigned x2) { cr_assert(x1 == x2 && x1 == 3); }),
        ([](int, unsigned) { cr_assert_fail("Zipper out of range"); }),
    };

    unsigned i = 0;
    for (auto &&[x1, x2] : Zipper(v1, v2)) {
        expected[i](x1, x2);
        i++;
    }
}

// Test(zipper_tests, basic_zipper_test)
// {
//     std::vector<std::optional<int>> v1 = {1, nullopt, 2, 84, nullopt, nullopt, 3};
//     std::vector<std::optional<unsigned>> v2 = {1, 84, 2, nullopt, 84, nullopt, 3};

//     std::array<std::function<void(int, unsigned)>, 4> expected = {
//         ([](int x1, unsigned x2) { cr_assert(x1 == x2 && x1 == 1); }),
//         ([](int x1, unsigned x2) { cr_assert(x1 == x2 && x1 == 2); }),
//         ([](int x1, unsigned x2) { cr_assert(x1 == x2 && x1 == 3); }),
//         ([](int, unsigned) { cr_assert_fail("Zipper out of range"); }),
//     };

//     unsigned i = 0;
//     for (auto &&[x1, x2] : Zipper(v1, v2)) {
//         expected[i](x1, x2);
//         i++;
//     }
// }
