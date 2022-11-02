/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * @brief points - 根据字符串长度返回分数
 * @param str
 * @return
 */
int points(string str) {
    return (str.size() <= 3 ? 0 : (str.size()) - 3);
}

/*
 * @brief vecToStr - 把点位向量转变为单词字符串
 * @param board
 * @param sofar
 * @return
 */
string vecToStr(Grid<char>& board, Vector<GridLocation>& sofar){
    string result = "";
    for (const GridLocation& loc : sofar){
        result.append(charToString(board.get(loc)));
    }
    return result;
}

/*
 * @brief nextLocations - 生成周边点位
 * @param board
 * @param sofar
 * @return
 */
Set<GridLocation> nextLocations(Grid<char>& board, Vector<GridLocation>& mark){
    Set<GridLocation> neighbors;
    GridLocation cur = mark[mark.size() - 1];
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            GridLocation next = GridLocation(cur.row + i, cur.col + j);
            if (board.inBounds(next)){
                bool exist = false;
                for (const GridLocation& loc : mark){
                    if (loc == next) exist = true;
                }
                if(!exist) neighbors.add(next);
            }
        }
    }
    return neighbors;
}

/*
 * @brief scoreBoardHelper
 * @param board 棋盘，包含待遍历的字符
 * @param lex字典，包含所有合法的单词
 * @return
 */
int scoreBoardHelper(Grid<char>& board, Lexicon& lex, Vector<GridLocation>& mark, Set<string>& visited){
    string str = vecToStr(board, mark);
    // 1. Base Case: 无此前缀，消除无效分支
    if(!lex.containsPrefix(str)){
        return 0;
    }
    // 2. Base Case: 无更多位置，做最终判断
    Set<GridLocation> neighbors = nextLocations(board, mark);
    if (neighbors.isEmpty()){
        if (lex.contains(str) && !visited.contains(str)){
            visited.add(str);
            return points(str);
        }
        return 0;
    } else {
        int point = 0;
        // 3. Special Case: 中间过程可以构成计分单词
        if (str.size() > 3 && lex.contains(str) && !visited.contains(str)){
            visited.add(str);
            point += points(str);
        }
        // 4. Recursive Case: 继续探索更多分支
        for (const GridLocation& loc : neighbors){
            mark.add(loc);
            point += scoreBoardHelper(board, lex, mark, visited);
            mark.remove(mark.size() - 1);
        }
        return point;
    }
}

/*
 * @brief scoreBoard
 * @param board
 * @param lex
 * @return
 */
int scoreBoard(Grid<char>& board, Lexicon& lex) {
    /* TODO: Implement this function. */
    int total = 0;
    Set<string> visited;

    for (int row = 0; row < board.numRows(); ++row) {
        for (int col = 0; col < board.numCols(); ++col) {
            Vector<GridLocation> mark = {{row, col}};
            total += scoreBoardHelper(board, lex, mark, visited);
        }
    }
    return total;
}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

STUDENT_TEST("Test scoreBoard, stress test of score 49"){
    Grid<char> board = {{'I', 'S', 'D', 'R'},
                        {'W', 'Y', 'H', 'N'},
                        {'G', 'C', 'E', 'T'},
                        {'A', 'E', 'E', 'H'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 49);
}


STUDENT_TEST("Test scoreBoard, stress test of score 247"){
    Grid<char> board = {{'O', 'A', 'E', 'T', 'M'},
                        {'T', 'T', 'O', 'I', 'S'},
                        {'F', 'S', 'N', 'W', 'D'},
                        {'E', 'Y', 'I', 'Z', 'H'},
                        {'T', 'S', 'O', 'I', 'E'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 247);
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

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
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

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
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
