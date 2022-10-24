//Judge the matching of brackets in a string
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;
int g_iNum = 0;

//clean the string
string operatorsFrom(string str) {

    Set<char> brackets = {'(',')','{','}','[',']'};
    if (str.size() == 0)
        return "";
    if (brackets.contains(str.at(str.size() - 1)))
        return operatorsFrom(str.substr(0, str.size() - 1)) + str.at(str.size() - 1);
    else
        return operatorsFrom(str.substr(0, str.size() - 1));
}

//judge the matching of brackets
#define v2

#ifdef v2
bool operatorsAreMatched(string ops) {

    if (ops.empty())
        return true;

    int middleBrackets = ops.find("[]");
    if (middleBrackets != string::npos){
        ops.erase(middleBrackets, 2);
        return operatorsAreMatched(ops);
    }

    int littleBrackets = ops.find("()");
    if (littleBrackets != string::npos){
        ops.erase(littleBrackets, 2);
        return operatorsAreMatched(ops);
    }

    int largeBrackets = ops.find("{}");
    if (largeBrackets != string::npos){
        ops.erase(largeBrackets, 2);
        return operatorsAreMatched(ops);
    }

    return false;

}
#endif

#ifdef v1
bool operatorsAreMatched(string ops) {
    Set<char> rightBrackets = {')','}',']'};
    if (ops.empty()){
        g_iNum = 0;
        return true;
    }
    else if (ops.size() % 2 == 1 || rightBrackets.contains(ops.at(g_iNum))){
        g_iNum = 0;
        return false;
    }
    else if (ops.at(g_iNum + 1) - ops.at(g_iNum) != 1 && ops.at(g_iNum + 1) - ops.at(g_iNum) != 2){ // judge the matching of brackets by ascii code
        g_iNum += 1;
        return operatorsAreMatched(ops);
    }
    else{
        ops.erase(g_iNum, 2);
        if (g_iNum != 0)
            g_iNum -= 1;
        return operatorsAreMatched(ops);
    }
}
#endif



/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}

STUDENT_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom(""), "");
    EXPECT_EQUAL(operatorsFrom(" [SSX]./;'["), "[][");
    EXPECT_EQUAL(operatorsFrom(" [SSX]./;{'[ "), "[]{[");
}

STUDENT_TEST("operatorsAreMatched on conplex example") {
    EXPECT(operatorsAreMatched("{[()][]}"));
    EXPECT(!operatorsAreMatched("{[[(])][]}"));
}
