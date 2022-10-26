//投票系统
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"
#include "testing/SimpleTest.h"
using namespace std;

// 对vector中的元素求和
int sumOfVotes(Vector<int>& blocks){
    int result = 0;
    for (int &elem : blocks){
        result += elem;
    }
    return result;
}

#define v2

//计算影响力指数，计算用时约 11 s
#ifdef v2
void computePowerIndexesRec(Vector<int>& blocks, int votes,int block, int &time, int index, int winVotes){
    if (votes > winVotes)
        return;
    if (index == blocks.size() && votes + block > winVotes && votes <= winVotes){
        time++;
    }
    if (index == blocks.size())
        return;
    computePowerIndexesRec(blocks, votes + blocks[index], block, time, index + 1, winVotes);
    computePowerIndexesRec(blocks, votes, block, time, index + 1, winVotes);
}

Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    Vector<int> result;
    // TODO your code here
    Vector<int> times;
    int votes = 0;
    int time = 0, sum = 0, mark = 0;
    int winVotes = sumOfVotes(blocks) / 2;
    for (int i = 0; i < blocks.size(); i++){
        int block = blocks[i];
        //判断是否有票数相同的区块
        for (int j = 0; j < i; j++){
            if (block == blocks[j]){
                times.add(times[j]);
                mark = 1;
                break;
            }
        }
        //不存在相同票数的区块时
        if (mark != 1){
            blocks.remove(i);
            computePowerIndexesRec(blocks, votes, block, time, 0, winVotes);
            blocks.insert(i, block);
            times.add(time);
        }
        mark = 0;
        time = 0;
        votes = 0;
    }
    //计算权重
    cout << times << endl;
    sum = sumOfVotes(times);
    if (sum != 0){
        for (int i = 0; i < times.size(); i++){
            result.add(times[i] * 100 / sum);
        }
    }
    return result;
}
#endif

#ifdef v1
// 计算得非常慢，由于在每次递归中进行了额外的求和
void computePowerIndexesRec(Vector<int>& blocks, Vector<int>& soFar,int block, int &time, int index, int winVotes){
    int sumOfSoFar = sumOfVotes(soFar);
    if (sumOfSoFar > winVotes)
        return;
    if (index == blocks.size() && sumOfSoFar + block > winVotes && sumOfSoFar <= winVotes){
        time++;
    }
    if (index == blocks.size())
        return;
    soFar.add(blocks[index]);
    computePowerIndexesRec(blocks, soFar, block, time, index + 1, winVotes);
    soFar.remove(soFar.size() - 1);
    computePowerIndexesRec(blocks, soFar, block, time, index + 1, winVotes);
}

Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    Vector<int> result;
    // TODO your code here
    Vector<int> times;
    Vector<int> soFar;
    int time = 0, sum = 0, mark = 0;
    int winVotes = sumOfVotes(blocks) / 2;
    for (int i = 0; i < blocks.size(); i++){
        int block = blocks[i];
        //判断是否有票数相同的区块
        for (int j = 0; j < i; j++){
            if (block == blocks[j]){
                times.add(times[j]);
                mark = 1;
                break;
            }
        }
        //不存在相同票数的区块时
        if (mark != 1){
            blocks.remove(i);
            computePowerIndexesRec(blocks, soFar, block, time, 0, winVotes);
            blocks.insert(i, block);
            times.add(time);
        }
        mark = 0;
        time = 0;
        soFar.clear();
    }
    //计算权重
    cout << times << endl;
    sum = sumOfVotes(times);
    if (sum != 0){
        for (int i = 0; i < times.size(); i++){
            result.add(times[i] * 100 / sum);
        }
    }
    return result;
}
#endif

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}



PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 20; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}

PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}



