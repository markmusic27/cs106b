#include "SpeakingRecursian.h"
#include "strlib.h"
#include <cmath>

using namespace std;

// Define vowels and consonants since they will be reused

const Vector<char> vowels = {'e', 'i', 'u'};
const Vector<char> consonants = {'b', 'k', 'n', 'r', 's', '\''};

// Initialize all helper function

bool isCharVowel(char ch);

Vector<string> getAllSyllables();

Vector<string> allRecursianWords(int numSyllables) {

    // Will only be called when allRecursianWords(0) is called explicitly (never called in recursive stack but is called in tests)
    if (numSyllables == 0) {
        return {""};
    }

    if (numSyllables < 0){
        error("There is no negative syllables!");
    }

    // Extra edge case where you first obtain all of the syllables with helper function below
    if (numSyllables == 1){
        Vector<string> baseSyllables = getAllSyllables();

        for (char v : vowels) {
            baseSyllables.add(charToString(v));
        }

        return baseSyllables;
    }
    Vector<string> smaller = allRecursianWords(numSyllables - 1);
    Vector<string> current = {};
    Vector<string> syllables = getAllSyllables();


    for (string s : smaller) {
        // Add every syllable to the end of smaller words
        for (string syll : syllables) {
            current.add(s + syll);
        }
    }

    return current;
}

bool isCharVowel(char ch) {
    return ch == 'e' || ch == 'i' || ch == 'u';
}

Vector<string> getAllSyllables(){

    Vector<string> syllables = {};

    for (char vow : vowels) {
        for (char cons : consonants) {
            syllables.add(string(1, cons) + string(1, vow));
        }
    }

    return syllables;
}


/* * * * * Test Cases Below This Point * * * * */
#include "GUI/SimpleTest.h"
#include "set.h"

PROVIDED_TEST("allRecursianWords works in simple cases.") {
    EXPECT_EQUAL(allRecursianWords(0), {""});
    EXPECT_ERROR(allRecursianWords(-1));
    EXPECT_ERROR(allRecursianWords(-137));
}

PROVIDED_TEST("allRecursianWords works for length 1.") {
    auto wordsUnsorted = allRecursianWords(1);

    /* Sort the words using a set. This will also eliminate duplicates. */
    Set<string> words;
    for (string word: wordsUnsorted) {
        /* All one-syllable words have length one or two. */
        EXPECT(word.length() == 1 || word.length() == 2);

        words += word;
    }

    /* Should be the same number of words once we sort them. */
    EXPECT_EQUAL(wordsUnsorted.size(), words.size());

    Set<string> expected = {
        "'e", "'i", "'u", "be", "bi", "bu", "e", "i",
        "ke", "ki", "ku", "ne", "ni", "nu", "re", "ri",
        "ru", "se", "si", "su", "u"
    };

    EXPECT_EQUAL(words, expected);
}

PROVIDED_TEST("allRecursianWords has the right quantities of words.") {
    EXPECT_EQUAL(allRecursianWords(0).size(), 1);
    EXPECT_EQUAL(allRecursianWords(1).size(), 21);
    EXPECT_EQUAL(allRecursianWords(2).size(), 378);
    EXPECT_EQUAL(allRecursianWords(3).size(), 6804);
    EXPECT_EQUAL(allRecursianWords(4).size(), 122472);
}

namespace {
    bool isConsonant(char ch) {
        return ch == 'b' || ch == 'k' || ch == 'n' || ch == 'r' || ch == 's' || ch == '\'';
    }
    bool isVowel(char ch) {
        return ch == 'e' || ch == 'i' || ch == 'u';
    }
}

PROVIDED_TEST("allRecursianWords produces words consisting of consonants and vowels.") {
    /* There are too many words here to check the answer against a reference - and it's
     * infeasible to manually create a reference. Instead, we will generate a list of
     * words and confirm that each one is either a consonant or a vowel. If we see
     * anything else, we know the answer is wrong.
     *
     * This won't catch everything, though. It's entirely possible to have a word made
     * entirely of vowels pass, or a word of way too many consonants pass. You will
     * need to write your own test cases to cover cases not detected here.
     */
    auto words = allRecursianWords(4);
    EXPECT_NOT_EQUAL(words.size(), 0);

    for (string word: words) {
        for (char ch: word) {
            EXPECT(isConsonant(ch) || isVowel(ch));
        }
    }
}
