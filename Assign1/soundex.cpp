/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 *
 * Applicable to the case which the input is, "", " ", "123456" or the first few characters are not letters, such as "123Alpha"
 * by fix the bug.
 */

string removeNonLetters(string s) {
    if (s.length() != 0){
        if (!isalpha(s.at(0))){
            return removeNonLetters(s.substr(1,s.length()-1));
        }
        string result = charToString(s[0]);
        for (int i = 1; i < s.length(); i++) {
            if (isalpha(s[i])) {
                result += s[i];
            }
        }
        return result;
    }
    else return "";
}

//encode the input string.
string encodeString(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        switch(tolower(s[i])){
        case 'a': case 'e': case 'i': case 'o': case 'u': case 'h': case 'w': case 'y':
            result += "0";
            break;
        case 'b': case 'f': case 'p': case 'v':
            result += "1";
            break;
        case 'c': case 'g': case 'j': case 'k': case 'q': case 's': case 'x': case 'z':
            result += "2";
            break;
        case 'd': case 't':
            result += "3";
            break;
        case 'l':
            result += "4";
            break;
        case 'm': case 'n':
            result += "5";
            break;
        case 'r':
            result += "6";
            break;
        }
    }
    return result;
}

//Merge the code.
string mergeCode(string s){
    if (s.length() == 1)
        return s;
    string result = charToString(s[0]);
    for (int i = 1; i < s.length(); i++) {
        if (s.at(i) != s.at(i-1))
            result += s[i];
    }
    return result;
}

//Replace the first number.
string replaceFirstNum(string s, char c){
    s.at(0) = toupper(c);
    return s;
}

//Remove 0 from string.
string removeZero(string s){
    return stringReplace(s, "0", "");
}

//Fill or cut of the code.
string fillOrCutCode(string s){
    string result = charToString(s[0]);
    for (int i = 1; i < 4; i++){
        if (s.length() <= i)
            result += "0";
        else {
            result += s[i];
        }
    }
    return result;
}

// The implement of the Soundex algorithm.
string soundex(string s) {
    /* TODO: Fill in this function. */
    if (removeNonLetters(s) == "")
        return "";
    string str = removeNonLetters(s);
    str = encodeString(str);
    str = mergeCode(str);
    str = replaceFirstNum(str, removeNonLetters(s).at(0));
    str = removeZero(str);
    str = fillOrCutCode(str);
    return str;
}

// Retrieve Soundex codes based on an existing database of surnames.

void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames

    /* TODO: Fill in the remainder of this function. */
    string name;
    string temp;
    Vector<string> matchResult;
    while(1){
        matchResult.clear();
        name = getLine("Enter a surname (type Enter to quit):");
        if (removeNonLetters(name) == "")
        {
            if (name == ""){
                cout << "All done" <<endl;
                return;
            }
            else//对应输入为"123456", "  "等情况
                cout << "你TM劈我程序是吧？再给你个机会" <<endl;//开个玩笑，无冒犯之意
        }
        temp = soundex(name);
        cout << "Soundex code is " << temp << endl;
        for (int i = 0; i < databaseNames.size(); i++){
            if (temp == soundex(databaseNames[i]))
                matchResult.add(databaseNames[i]);
        }
        matchResult.sort();
        cout << "Matches from database:" << matchResult << endl;
    }
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}

PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here
STUDENT_TEST("Test removing puntuation, digits, and spaces"){
    string s = "";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
    s = "12345";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
    s = "22bAd22";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "bAd");
}

STUDENT_TEST("Test the conrectness of soundex funtion"){
    EXPECT_EQUAL(soundex(" "), "");
    EXPECT_EQUAL(soundex("123456"), "");
    EXPECT_EQUAL(soundex("1"), "");
    EXPECT_EQUAL(soundex("  Jackson  "), "J250");
    EXPECT_EQUAL(soundex("123jackson321"), "J250");
    EXPECT_EQUAL(soundex("VAN NIEKERK"), "V526");
}


