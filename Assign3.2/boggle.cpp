//拼字游戏
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

//确定该单词对应的分数
int points(string str) {
    /* TODO: Implement this function. */
    return str.length() >= 4 ?  str.length() - 3 : 0;
}

//确定不同位置可访问的邻居集合
void findNeighbor(const GridLocation &loc, const Grid<char>& board, Set<GridLocation> &nei){
    GridLocation north = {loc.row - 1, loc.col};
    GridLocation sourth = {loc.row + 1, loc.col};
    GridLocation west = {loc.row, loc.col - 1};
    GridLocation east = {loc.row, loc.col + 1};
    GridLocation northWestern = {loc.row - 1, loc.col - 1};
    GridLocation northEastern = {loc.row - 1, loc.col + 1};
    GridLocation sourthWestern = {loc.row + 1, loc.col - 1};
    GridLocation sourthEastern = {loc.row + 1, loc.col + 1};
    Queue<GridLocation> adjPosition = {north, sourth, west, east, northWestern, northEastern, sourthWestern, sourthEastern};

    while (!adjPosition.isEmpty()){
        if (adjPosition.peek().col >= 0 && adjPosition.peek().row >= 0 && adjPosition.peek().col < board.numCols()
                && adjPosition.peek().row < board.numRows()){
            nei += adjPosition.dequeue();
        }
        else
            adjPosition.dequeue();
    }
}

//递归所有单词连接可能
void scoreBoardRec(Grid<char>& board, Lexicon& lex, string str, int &score, GridLocation position, Set<GridLocation> &visitedLoc, Set<string> &word){

    str += board.get(position);
    if (!lex.containsPrefix(str))
        return;
    else if (lex.contains(str)){
        if (str.length() >= 4 && !word.contains(str)){
            word += str;
            score += points(str);
        }
    }
    Set<GridLocation> neigbor;
    findNeighbor(position, board, neigbor);
    visitedLoc += position;
    neigbor.difference(visitedLoc);
    for (GridLocation nei : neigbor){
        scoreBoardRec(board, lex, str, score, nei, visitedLoc, word);
    }
    visitedLoc.remove(position);
}

//包装函数
int scoreBoard(Grid<char>& board, Lexicon& lex) {
    /* TODO: Implement this function. */
    int score = 0;
    Set<string> word;
    Set<GridLocation> visitedLoc;
    for (GridLocation loc: board.locations()){
        scoreBoardRec(board, lex, "", score, loc, visitedLoc, word);
    }
    return score;
}



/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}
PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}


PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}
