//Solve maze problem
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;

//Find all adjacent positions of the current position
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    GridLocation north = {cur.row - 1, cur.col};
    GridLocation sourth = {cur.row + 1, cur.col};
    GridLocation west = {cur.row, cur.col - 1};
    GridLocation east = {cur.row, cur.col + 1};
    Queue<GridLocation> adjPosition = {north, sourth, west, east};
    while (!adjPosition.isEmpty()){
        if (adjPosition.peek().col >= 0 && adjPosition.peek().row >= 0 && adjPosition.peek().col < maze.numCols()
                && adjPosition.peek().row < maze.numRows() && maze.get(adjPosition.peek())){
            neighbors += adjPosition.dequeue();
        }
        else
            adjPosition.dequeue();
    }
    return neighbors;
}

// Confirm that the path is correct
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }
    if (!maze.get(mazeExit) || !maze[0][0]){
        error("Maze does not conform to the design rules");
    }
    Set<GridLocation> traveledPath;
    while(path.size() > 1){
        traveledPath += path.peek();
        if (!generateValidMoves(maze, path.pop()).contains(path.peek())){
            error("Route does not conform to the rules");
        }
        if (traveledPath.contains(path.peek())){
            error("There is a loop in the path");
        }
    }
    if (path.pop() != GridLocation(0, 0)){
        error("Path does not begin at maze entrance");
    }
}

//BFS algorithm to find the shortest path of the maze
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;
    /* TODO: Fill in the remainder of this function. */
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    Set<GridLocation> nextStep;
    Queue<Stack<GridLocation>> bfsTrail;
    bfsTrail.enqueue({{0, 0}});
    while (!bfsTrail.isEmpty()){
        if (bfsTrail.peek().peek() == mazeExit){
            path = bfsTrail.dequeue();
            break;
        }
        else {
            nextStep = generateValidMoves(maze, bfsTrail.peek().peek());
            Stack<GridLocation> tempForSet = bfsTrail.peek();
            Set<GridLocation> travelRoute;
            while (!tempForSet.isEmpty()){
                travelRoute += tempForSet.pop();
            }
            nextStep.difference(travelRoute);
            Stack<GridLocation> temp = bfsTrail.dequeue();
            for (GridLocation elem : nextStep){
                temp.push(elem);
                bfsTrail.enqueue(temp);
                temp.pop();
            }
        }
        MazeGraphics::highlightPath(bfsTrail.peek(), "red", 1);
    }
    return path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);
    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

// TODO: add your test cases here
STUDENT_TEST("generateValidMoves on location in the center of 1 times 1 grid with no walls") {
    Grid<bool> maze = {{true}};
    GridLocation center = {0, 0};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

STUDENT_TEST("generateValidMoves on location in the boundary of 2 times 2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation boundary = {1, 1};
    Set<GridLocation> expected = {{1,0}};

    EXPECT_EQUAL(generateValidMoves(maze, boundary), expected);
}

STUDENT_TEST("generateValidMoves on location on the boundary of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation boundary = {1, 2};
    Set<GridLocation> expected = {{0,2}, {2,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, boundary), expected);
}

STUDENT_TEST("validatePath on correct solution loaded from file for anathor maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/17x37.maze", maze);
    readSolutionFile("res/17x37.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("validatePath on 1 times 1 maze") {
    Grid<bool> maze = {{true}};
    Stack<GridLocation> soln = {{0 ,0}};

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{false, false},
                       {true, true}};
    Stack<GridLocation> mazeError = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, mazeError));

}

STUDENT_TEST("solveMaze on file 2x2") {
    Grid<bool> maze;
    readMazeFile("res/2x2.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 33x41") {
    Grid<bool> maze;
    readMazeFile("res/33x41.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

