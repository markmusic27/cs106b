/* ONLY CONNECT PUZZLE:
 * Word: STNFRDNVRSTY
 * Hint: You attend it;
 */

#include "OnlyConnect.h"

#include "strlib.h"
using namespace std;

bool isVowel(char c);

string onlyConnectize(string phrase) {
    // Edge case: empty string
    if (phrase.empty()) {
        return "";
    }

    char firstCharacter = phrase.at(0);

    // Eliminates spaces
    if (firstCharacter == ' '){
        return onlyConnectize(phrase.substr(1));

    }

    bool vow = isVowel(firstCharacter);

    // Reduces input string by first letter
    string remaining = onlyConnectize(phrase.substr(1));

    return vow ? remaining : toUpperCase(firstCharacter) + remaining;
}

bool isVowel(char c) {
    char u = toUpperCase(c);

    Vector<char> vowels = {'A', 'E', 'I', 'O', 'U'};

    bool v = false;

    for (int i = 0; i < vowels.size(); i++){
        if (u == vowels[i]){
            v = true;
        }
    }

    return v;
}






/* * * * * * Provided Test Cases * * * * * */
#include "GUI/SimpleTest.h"

PROVIDED_TEST("Handles single-character inputs.") {
    EXPECT_EQUAL(onlyConnectize("A"), "");
    EXPECT_EQUAL(onlyConnectize("Q"), "Q");
}

PROVIDED_TEST("Converts lower-case to upper-case.") {
    EXPECT_EQUAL(onlyConnectize("lowercase"), "LWRCS");
    EXPECT_EQUAL(onlyConnectize("uppercase"), "PPRCS");
}

STUDENT_TEST("Handles spaces in string") {
    EXPECT_EQUAL(onlyConnectize("Catch Me if You Can"), "CTCHMFYCN");
    EXPECT_EQUAL(onlyConnectize("Beauty and The Beast"), "BTYNDTHBST");
}



/* TODO: You will need to add your own tests into this suite of test cases. Think about the sorts
 * of inputs we tested here, and, importantly, what sorts of inputs we *didn't* test here. Some
 * general rules of testing:
 *
 *    1. Try extreme cases. What are some very large cases to check? What are some very small cases?
 *
 *    2. Be diverse. There are a lot of possible inputs out there. Make sure you have tests that account
 *       for cases that aren't just variations of one another.
 *
 *    3. Be sneaky. Don't just try standard inputs. Try weird ones that you wouldn't expect anyone to
 *       actually enter, but which are still perfectly legal.
 *
 * Happy testing!
 */






