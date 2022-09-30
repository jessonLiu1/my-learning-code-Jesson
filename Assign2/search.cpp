// Build a mini search engine by reverse indexing
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;

//Returns a cleaning string
string cleanToken(string s)
{
    string result;
    bool haveAlpha = 0;
    int i = 0;
    int j = s.length() - 1;
    for (; i < s.length(); i++) {
        if (!ispunct(s[i])) {
            break;
        }
    }
    for (; j < s.length(); j--) {
        if (!ispunct(s[j])) {
            break;
        }
    }
    result = s.substr(i, j - i + 1);
    for (int k = 0; k < result.length(); k++) {
        if (isalpha(result[k])){
            result[k] = toLowerCase(result[k]);
            haveAlpha = 1;
        }
    }
    if (haveAlpha == 1){
        return result;
    }
    else return "";

}

//  Extracts all non-repeating strings from the body text and returns them as a Set.
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    Vector<string> textStr = stringSplit(text, " ");
    for (string &elem: textStr){
        string temp = cleanToken(elem);
        if (!tokens.contains(temp) && temp !=""){
             tokens += temp;
        }
    }
    return tokens;
}

//Reads content from a database file and processes it as a reverse index
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    ifstream in;

    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);

    Vector<string> lines;
    readEntireFile(in, lines);
    Set<string> word;
    int i = 1;
    for (; i < lines.size() ; i = i + 2){
        word = gatherTokens(lines[i]);
        for (string elem: word){
            index[elem] += lines[i - 1];
        }
    }
    return i / 2;
}

// find marched URL by given key word
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    // TODO: your code here
    Vector<string> queryStr = stringSplit(query, " ");
    for (string elem:queryStr){
        string temp = cleanToken(elem);
        if (elem[0] == '+'){
            result.intersect(index[temp]);
        }
        else if (elem[0] == '-'){
            result.difference(index[temp]);
        }
        else {
            result.unionWith(index[temp]);
        }
    }
    return result;
}

// build the search engine
void searchEngine(string dbfile)
{
    // TODO: your code here
    Map<string, Set<string>> index;
    cout << "Stand by while building index..." << endl;
    int pages = buildIndex(dbfile, index);
    cout << "Indexed " << pages << " pages containing "
         << index.size() << " unique terms" << endl;
    while (true) {
           string query = getLine("Enter query sentence (RETURN/ENTER to quit) ");
           if (query.empty()) {
               break;
           }
           Set<string> match = findQueryMatches(index, query);
           cout << "Found " << match.size() << " matching pages" << endl;
           cout << match << endl;
       }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here
STUDENT_TEST("TEST FOR cleanToken FUNCTION") {
    EXPECT_EQUAL(cleanToken(""), "");
    EXPECT_EQUAL(cleanToken("!!WO!!RLD!!"), "wo!!rld");
    EXPECT_EQUAL(cleanToken("!!10L!6!!"), "10l!6");
}

STUDENT_TEST("buildIndex from website.txt, 32 pages") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/website.txt", index);
    EXPECT_EQUAL(nPages, 32);
    EXPECT(index.containsKey("writes"));
    TIME_OPERATION("WEBSITE", buildIndex("res/website.txt", index));
}

STUDENT_TEST("test for the VALUE of KEY from res/tiny") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(index["fish"], {"www.shoppinglist.com", "www.dr.seuss.net", "www.bigbadwolf.com"});
}
