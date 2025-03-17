#include "WinSumLoseSum.h"
using namespace std;

// This file includes

/* This function takes the set of integers and returns the set of integers that
 * Add up to the target. If there are none, then the function returns Nothing as it is optional
*/
Optional<Set<int>> makeTarget(const Set<int>& elems, int target) {
    // Base case: If the target is exactly 0, return an empty set (valid subset).
    if (target == 0) {
        return Set<int>();
    }

    // Base case: If the set is empty and target isn't 0, there's no solution.
    if (elems.isEmpty()) {
        return Nothing;
    }

    int first = elems.first();
    Set<int> rest = elems - first;

    // Recursive case: Try including the first element
    Optional<Set<int>> withFirst = makeTarget(rest, target - first);
    if (withFirst != Nothing) {
        return withFirst.value() + first;
    }

    // Recursive case: Try excluding the first element
    return makeTarget(rest, target);
}


/* * * * * Test Cases Below This Point * * * * */
#include "GUI/SimpleTest.h"

// Test using Pythagorean Theorem with Random Number
STUDENT_TEST("Works for prime numbers using Pythagorean Theorem") {

    // Use a^2 + b^2 = c^2
    int a = randomInteger(1, 9);
    int b = randomInteger(1, 9);

    int aSqr = a*a;
    int bSqr = b*b;
    int cSqr = aSqr + bSqr;

    EXPECT_EQUAL(makeTarget({ aSqr, bSqr },  cSqr), { aSqr, bSqr });
}



/* * * * * Provided Tests Below This Point * * * * */
PROVIDED_TEST("Works for an empty set of numbers.") {
    /* Can make 0, but not others. */
    EXPECT_EQUAL(makeTarget({ },  0), {});
    EXPECT_EQUAL(makeTarget({ },  1), Nothing);
    EXPECT_EQUAL(makeTarget({ }, -1), Nothing);
}

PROVIDED_TEST("Works for a one-element (singleton) set.") {
    /* Can make 0 and 137, but not others. */
    EXPECT_EQUAL(makeTarget({ 137 }, 0),   {});
    EXPECT_EQUAL(makeTarget({ 137 }, 137), {137});
    EXPECT_EQUAL(makeTarget({ 137 }, 1),   Nothing);
    EXPECT_EQUAL(makeTarget({ 137 }, -1),  Nothing);
}

PROVIDED_TEST("Works for a two-element (doubleton) set.") {
    EXPECT_EQUAL(makeTarget({ 1, 2 }, -1), Nothing);
    EXPECT_EQUAL(makeTarget({ 1, 2 }, 0), {});
    EXPECT_EQUAL(makeTarget({ 1, 2 }, 1), {1});
    EXPECT_EQUAL(makeTarget({ 1, 2 }, 2), {2});
    EXPECT_EQUAL(makeTarget({ 1, 2 }, 3), {1, 2});
    EXPECT_EQUAL(makeTarget({ 1, 2 }, 4), Nothing);
}

PROVIDED_TEST("Works for a three-element set.") {

    /* Can make 0, 1, 3, 4, 5, 7, and 8, but not others. */
    EXPECT_EQUAL(makeTarget({ 1, 3, 4 }, -1), Nothing);
    EXPECT_EQUAL(makeTarget({ 1, 3, 4 },  0), {});
    EXPECT_EQUAL(makeTarget({ 1, 3, 4 },  1), {1});
    EXPECT_EQUAL(makeTarget({ 1, 3, 4 },  2), Nothing);
    EXPECT_EQUAL(makeTarget({ 1, 3, 4 },  3), {3});
    EXPECT_NOT_EQUAL(makeTarget({ 1, 3, 4 }, 4), Nothing);
    EXPECT_EQUAL(makeTarget({ 1, 3, 4 },  5), {1, 4});
    EXPECT_EQUAL(makeTarget({ 1, 3, 4 },  6), Nothing);
    EXPECT_EQUAL(makeTarget({ 1, 3, 4 },  7), {3, 4});
    EXPECT_EQUAL(makeTarget({ 1, 3, 4 },  8), {1, 3, 4});
    EXPECT_EQUAL(makeTarget({ 1, 3, 4 },  9), Nothing);
}
