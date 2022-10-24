//多路并归
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

//双路并归
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    /* TODO: Implement this function. */
    while (a.size() != 0 || b.size() != 0){
        int temp = -2147483648;
        if (!result.isEmpty())
            temp = result.peek();
        if (a.isEmpty())
            result.enqueue(b.dequeue());
        else if (b.isEmpty())
            result.enqueue(a.dequeue());
        else if (a.peek() <= b.peek())
            result.enqueue(a.dequeue());
        else
            result.enqueue(b.dequeue());
        if (result.peek() < temp)
            exit(EXIT_FAILURE);
    }
    return result;
}

/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

//多路并归
#define v2
#ifdef v2

void recMultiMergeRec(Vector<Queue<int>>& all, int begin, int end, Queue<int> &result) {
    if (end - begin == 1)
        result = binaryMerge(all[begin], all[end]);
    else if (end == begin)
        result = all[begin];
    else {
        Queue<int> left, right;
        recMultiMergeRec(all, begin, begin + (end - begin) / 2, left);
        recMultiMergeRec(all, begin + (end - begin) / 2 + 1, end, right);
        result = binaryMerge(left, right);
    }
}

Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    /* TODO: Implement this function. */
    recMultiMergeRec(all, 0, all.size() - 1, result);
    return result;
}
#endif

#ifdef v1
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    /* TODO: Implement this function. */
    int mid = all.size() / 2;
    if (all.size() == 2)
        result = binaryMerge(all[0], all[1]);
    else if (all.size() == 1)
        result = all[0];
    else {
        Vector<Queue<int>> left = all.subList(0, mid);
        Vector<Queue<int>> right = all.subList(mid);
        result = binaryMerge(recMultiMerge(left), recMultiMerge(right));
    }
    return result;
}
#endif



/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


STUDENT_TEST("naiveMultiMerge with empty queue") {
    Vector<Queue<int>> all = {{},
                             {},
                             {},
                             {}
                            };
    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}


