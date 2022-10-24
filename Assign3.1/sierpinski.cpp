//Sierpinski fractal
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

//Determine the coordinates of the points to be drawn
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    /* TODO: Implement this function. */
    if (order == 0){
       fillBlackTriangle(window, one, two, three);
       return 1;
    }
    GPoint newOne = {(one.x + two.x)/2, (one.y + two.y)/2};
    GPoint newTwo = {(three.x + two.x)/2, (three.y + two.y)/2};
    GPoint newThree = {(three.x + one.x)/2, (three.y + one.y)/2};
    int numLeft = drawSierpinskiTriangle(window, newOne, newTwo, two, order - 1);
    int numRight = drawSierpinskiTriangle(window, newThree, newTwo, three, order - 1);
    int numTop = drawSierpinskiTriangle(window, newThree, newOne, one, order - 1);
    return numLeft + numRight + numTop;
}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

