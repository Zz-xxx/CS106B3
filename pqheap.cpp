#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 8;
const int NONE = -1; // used as sentinel index 哨兵索引

PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated]();
    _numFilled = 0;
}

PQHeap::~PQHeap() {
    delete[] _elements;
}

void PQHeap::enqueue(DataPoint elem) {
    // Edge Case: 没有足够空间
    if (_numFilled == _numAllocated){
        // a. 备份旧数组
        DataPoint* _oldElements = _elements;
        // b. 开辟新的数组空间
        _numAllocated *= 2;
        _elements = new DataPoint[_numAllocated]();
        // c. 转移数据
        for (int i = 0; i < _numFilled; ++i) {
            _elements[i] = _oldElements[i];
        }
        // d. 删除旧数组
        delete[] _oldElements;
    }
    // 1. 插入元素
    _elements[_numFilled] = elem;
    // 2. Bubble up
    int childIndex = _numFilled;
    while (childIndex > 0) {
        int parentIndex = getParentIndex(childIndex);
        if (_elements[parentIndex].priority > _elements[childIndex].priority ){
            swap(childIndex, parentIndex);  // 交换
        }
        childIndex = parentIndex;
    }
    // 3. 更新计数器
    _numFilled++;
}

DataPoint PQHeap::peek() const {
    if(isEmpty()){
        error("PQueue is empty!");
    }
    return _elements[0];
}

DataPoint PQHeap::dequeue() {
    DataPoint front = peek();
    // 1. 替换根元素
    _elements[0] = _elements[--_numFilled];
    // 2. Bubble Down 复杂度 O(logN)
    // bubleDownItr(0);
    bubleDownRec(0);
    return front;
}

void PQHeap::bubleDownRec(int parent){
    int left = getLeftChildIndex(parent);
    int right = getRightChildIndex(parent);
    // No child node
    if(left == NONE){
        return;
    }
    // Only left child node
    else if(right == NONE){
        if(_elements[parent].priority > _elements[left].priority){
            swap(parent, left);
        }
        return;
    }
    // Two child nodes
    int minChild = (_elements[left].priority < _elements[right].priority) ? left : right;
    if(_elements[parent].priority > _elements[minChild].priority){
        swap(parent, minChild);
    }
    bubleDownRec(minChild);
}

void PQHeap::bubleDownItr(int parent){
    while (true) {
        // No child node
        int left = getLeftChildIndex(parent);
        if(left == NONE){
            break;
        }
        // Only left child node
        int right = getRightChildIndex(parent);
        if (right == NONE){
            if(_elements[parent].priority > _elements[left].priority){
                swap(parent, left);
            }
            break;
        }
        // Two child nodes
        int minChild = (_elements[left].priority < _elements[right].priority) ? left : right;
        if(_elements[parent].priority > _elements[minChild].priority){
            swap(parent, minChild);
            parent = minChild;
        } else {
            break;
        }
    }
}

bool PQHeap::isEmpty() const {
    return size() == 0;
}

int PQHeap::size() const {
    return _numFilled;
}

void PQHeap::clear() {
    _numFilled = 0;
}

void PQHeap::swap(int indexA, int indexB){
    DataPoint tmp = _elements[indexA];
    _elements[indexA] = _elements[indexB];
    _elements[indexB] = tmp;
}

void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
    for (int i = 0; i < size(); ++i) {
        cout << "[" << i << "] = " << _elements[i] << endl;
    }
}

/*
 * @brief PQHeap::validateInternalState
 */
void PQHeap::validateInternalState() const {
    if(_numFilled > _numAllocated)
        error("Too many elements in not enough space!");
    validateHelper(0);
}

/*
 * @brief PQHeap::validateHelper
 * @param parent
 * @return
 */
void PQHeap::validateHelper(int parent) const {
    int left = getLeftChildIndex(parent);
    int right = getRightChildIndex(parent);
    // No child node
    if (left == NONE){
        return;
    }
    // only left child node
    if(right == NONE){
        if(_elements[parent].priority > _elements[left].priority){
            printDebugInfo("validateInternalState");
            error("PQHeap is not valid at index" + integerToString(parent));
        }
        return;
    } else {
        // Two child nodes
        if (_elements[parent].priority > _elements[left].priority ||
                _elements[parent].priority > _elements[right].priority){
            printDebugInfo("validateInternalState");
            error("PQHeap is not valid at index" + integerToString(parent));
        }
        validateHelper(left);
        validateHelper(right);
    }
}

// 随机交换一个节点，用于验证 validateInternalState接口
void PQHeap::validateHelperTest(){
    while (true) {
        int parent = randomInteger(0, this -> size());
        int minChild = randomBool() ? getLeftChildIndex(parent) : getRightChildIndex(parent);
        if (minChild != NONE && _elements[parent].priority < _elements[minChild].priority){
            swap(parent, minChild);
            break;
        }
    }
}

/*
 * 找不到返回-1
 */
int PQHeap::getParentIndex(int child) const {
    if(child < 1)
        return NONE;
    return (child - 1)/2;   // 3 or 4 -> 1
}

int PQHeap::getLeftChildIndex(int parent) const {
    if(2 * parent + 1 >= _numFilled)
        return NONE;
    return 2* parent + 1;   // 1 -> 3
}

int PQHeap::getRightChildIndex(int parent) const {
    if(2 * parent + 2 >= _numFilled)
        return NONE;
    return 2 * parent +2;   // 1 -> 4
}

void fillVector(PQHeap& pq, int n){
    pq.clear();
    for (int i = 0; i < n; ++i) {
        DataPoint pt = {"", (double)randomInteger(0, 100)};
        pq.enqueue(pt);
    }
}
/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Add your own custom tests here! */



/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}
