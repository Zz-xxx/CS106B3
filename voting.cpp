#include "map.h"
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * @brief sumOf - 计算向量中所有元素总和
 * @param vec
 * @return
 */
int sumOf(const Vector<int> &vec){
    int total = 0;
    for(int value : vec) {
        total += value;
    }
    return total;
}

#define btr

#ifdef btr
/*
 * @brief computePowerIndexesRec - 使用 backtracking 计算权力值
 * @param blocks
 * @param targetBlock - 所选目标块 index
 * @param targetPower - 记录目标块权重
 * @param totalVotes - 记录联盟票数
 * @param index
 * @param swing - 关键票数量
 */
void computePowerIndexesRec(const Vector<int> &blocks, int &targetBlock, int &targetPower, int totalVotes, int index, int &swing){
    if(index == targetBlock)    // 跳过target block
        index++;
    if(totalVotes > swing)  // 利用pruning优化无效分支
        return;
    // 1. Current coalition: two questions about the target block
    if (totalVotes <= swing && totalVotes + blocks[targetBlock] > swing) targetPower++;
    // 2. Backtracking: Choose/Explore/Unchoose pattern
    for (int i = index; i < blocks.size(); ++i) {
        if(i == targetBlock)     // 跳过target block
            continue;
        computePowerIndexesRec(blocks, targetBlock, targetPower, totalVotes + blocks[i], i + 1, swing);
    }
}
#endif

#ifdef dfs
/*
 * @brief computePowerIndexesRec - 使用 dfs backtracking 计算权力值
 * @param blocks
 * @param targetBlock - 所选目标块 index
 * @param targetPower - 记录目标块权重
 * @param totalVotes - 记录联盟票数
 * @param index
 * @param swing - 关键票数量
 */
void computePowerIndexesRec(const Vector<int> &blocks, int &targetBlock, int &targetPower, int totalVotes, int index, int &swing){
    if (index == targetBlock)    // 跳过target block
        index++;
    if(totalVotes > swing)  // 利用pruning优化无效分支
        return;
    // 1. Base case: two questions about the target block
    if (index == blocks.size()){
        if (totalVotes <= swing && totalVotes + blocks[targetBlock] > swing) targetPower++;
    }
    // 2. Recursive Case: Include/Exclude Pattern
    else {
        // 2.1 Include
        computePowerIndexesRec(blocks, targetBlock, targetPower, totalVotes + blocks[index], index + 1, swing);
        // 2.2 Exclude
        computePowerIndexesRec(blocks, targetBlock, targetPower, totalVotes, index + 1, swing);
    }
}
#endif

/*
 * @brief computePowerIndexes - 计算给定区块 Banzhaf权力指数
 * @param blocks
 * @return
 */
Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    // 1. 递归计算权力值
    Vector<int> result(blocks.size(), 0);
    int swingVotes = sumOf(blocks)/2;

    Map<int, int> cachedResult; // 利用Memorization优化重复项
    for (int i = 0; i < blocks.size(); ++i) {
        int power = 0;
        if (cachedResult.containsKey(blocks[i])){
            result[i] = cachedResult[blocks[i]];
        } else {
            computePowerIndexesRec(blocks, i, power, 0, 0, swingVotes);
            cachedResult[blocks[i]] = power;
            result[i] = power;
        }
    }
    // 2. 生成Banzhaf Power Index
    double total = sumOf(result);
    for (int i = 0; i < result.size(); ++i) {
        result[i] = result[i] / total * 100;
    }
    return result;
}

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

PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 15; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}
