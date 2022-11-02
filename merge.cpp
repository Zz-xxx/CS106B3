/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

/* binaryMerge二路并归 排序
 * @param a
 * @param b
 * @return
 */
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    /* TODO: Implement this function. */
    int back;   //用于记录result后端值
    int cur;    // 存储当前待插入的值

    // 处理公共部分
    while (!a.isEmpty() && !b.isEmpty()){
        cur = a.peek() < b.peek() ? a.dequeue() : b.dequeue();
        if (!result.isEmpty() && back > cur) error("输入数据顺序错误！");
        result.enqueue(cur);
        back = cur;
    }
    // 处理剩余部分
    while (!a.isEmpty()){
        cur = a.dequeue();
        if (!result.isEmpty() && back >cur) error("输入数据顺序错误！");
        result.enqueue(cur);
        back = cur;
    }
    while (!b.isEmpty()){
        cur = b.dequeue();
        if (!result.isEmpty() && back >cur) error("输入数据顺序错误！");
        result.enqueue(cur);
        back = cur;
    }
    return result;
}

STUDENT_TEST("binaryMerge 两个全为空"){
    Queue<int> a = {};
    Queue<int> b = {};
    EXPECT_EQUAL(binaryMerge(a, b), {});
    EXPECT_EQUAL(binaryMerge(b, a), {});
}

STUDENT_TEST("binaryMerge 其中一个为空"){
    Queue<int> a = {};
    Queue<int> b = {1};
    EXPECT_EQUAL(binaryMerge(a, b), {1});
    EXPECT_EQUAL(binaryMerge(b, a), {1});
}

STUDENT_TEST("binaryMerge 输入没有排序"){
    Queue<int> a = {2, 1, 5};   // 等长
    Queue<int> b = {1, 3, 5};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));
    a = {2, 3, 5, 6, 2};    // 不等长
    b = {1, 3, 3};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));
}

STUDENT_TEST("binaryMerge 一个为空一个不排序"){
    Queue<int> a = {};
    Queue<int> b = {1, 3, 5};
//    EXPECT_ERROR(binaryMerge(a, b));
//    EXPECT_ERROR(binaryMerge(b, a));
    EXPECT_NO_ERROR(binaryMerge(a, b));
    EXPECT_NO_ERROR(binaryMerge(b, a));
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

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Queue<int> recMultiMergeRec(Vector<Queue<int>> &all, int start, int end){
    if (start +1 == end){
        return all[start];
    } else {
        int mid = ((unsigned)start + (unsigned)end) >> 1; // 正数范围内避免溢出
        return binaryMerge(recMultiMergeRec(all, start, mid), recMultiMergeRec(all, mid, end));
    }
}

Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    return recMultiMergeRec(all, 0, all.size());
}

/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

STUDENT_TEST("recMultiMerge时间测试"){
    int n = 90000;
    int k = n/10;
    for (int i = 0; i < 5; ++i) {
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), recMultiMerge(all));
        n *= 2;
    }
}


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
