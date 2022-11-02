/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/*
 * operatorsFoam 从字符串中提取括号
 * @param str
 * @return
 */
string operatorsFrom(string str) {
    /* TODO: Implement this function. */
    // 1. Base Case
    if(str.empty()){
        return "";
    } else {
        // 2. reducing
        string first = str.substr(0, 1);
        string remains = str.substr(1);
        // 3. Recursive Case
        if (stringContains("(){}[]", first))
            return first + operatorsFrom(remains);
        else
            return operatorsFrom(remains);
    }
}

/*
 * operatorsAreMatched判断括号字符串是否配对
 * @param ops
 * @return
 */
bool operatorsAreMatched(string ops) {
    /* TODO: Implement this function. */
    // 1. Base Case
    if (ops.empty()){
        return true;
    } else {
        // 2. Reducing
        if (stringContains(ops, "()")){
            stringReplaceInPlace(ops, "()", "");
        } else if (stringContains(ops, "[]")){
            stringReplaceInPlace(ops, "[]", "");
        } else if (stringContains(ops, "{}")){
            stringReplaceInPlace(ops, "{}", "");
        } else {
            return false;
        }
        // 3. Recursive Case
    }
    return operatorsAreMatched(ops);
}

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
    string ops = operatorsFrom(str);    // 提取括号
    return operatorsAreMatched(ops);    // 检查配对
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
