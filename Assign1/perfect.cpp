/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include <iostream>
#include "testing/SimpleTest.h"
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */

long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* TODO: Caculate the sum of all proper divisors of `n`
 * excluding itself by a smarter way.
 */
long smarterSum(long double n) {
    /* TODO: Fill in this function. */
    long int smartN =sqrt(n);
    long total = 1;
    for (long divisor = 2; divisor <= smartN; divisor++) {
        long double quot = n / divisor;
        if (quot - int(quot) == 0) {
            total = total + divisor + quot;
        }
    }
    if ((sqrt(n) - smartN) == 0){
        total -= smartN;
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */

void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}



/* TODO: Determine if a number is perfect based on
 * the smarterSum function.
 */
bool isPerfectSmarter(long n) {
    /* TODO: Fill in this function. */
    return (n != 0) && (n == smarterSum(n));
}

//TODO: Find perfect numbers in range of 1 to variable 'stop' by a smarter way.

void findPerfectsSmarter(long stop) {
     /* TODO: Fill in this function. */
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

//TODO: Find perfect numbers by Euclidean method for faster computation
long findNthPerfectEuclid(long n) {
    /* TODO: Fill in this function. */
    int num = 1;
    int k = 1;
    long perfectNum = 0;
    while(num <= n){
        long m = pow(2,k) - 1;
        if (!(smarterSum(m) - 1)){
            perfectNum = pow(2,k - 1) * (pow(2,k) - 1);
            cout << "Found perfect number: " << perfectNum << endl;
            num++;
        }
        k++;
    }
    return perfectNum;
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10000, findPerfects(10000));
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
}

// TODO: add your student test cases here
/*STUDENT_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(25000, findPerfects(25000));
    TIME_OPERATION(50000, findPerfects(50000));
    TIME_OPERATION(100000, findPerfects(100000));
    TIME_OPERATION(200000, findPerfects(200000));
}
STUDENT_TEST("Test for negative number") {
    EXPECT(isPerfect(-1));
    EXPECT(isPerfect(-100));
}
STUDENT_TEST("Confirm smarterSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(9), 4);
    EXPECT_EQUAL(divisorSum(12), 16);
}
STUDENT_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}
STUDENT_TEST("Test for the conrectness of findNthPerfectEuclid function") {
    EXPECT(isPerfect(findNthPerfectEuclid(5)));
    TIME_OPERATION(5, findNthPerfectEuclid(5));
    EXPECT_EQUAL(findNthPerfectEuclid(2), 28);
}
*/
