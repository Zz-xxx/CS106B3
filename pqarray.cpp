#include "pqarray.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

// program constant
static const int INITIAL_CAPACITY = 10;

/*
 * 构造函数初始化PQArray类实例所需的所有成员变量
 * 分配的容量被初始化为起始常数，并分配该大小的动态数组
 * 填充的插槽数最初为零
 */
PQArray::PQArray() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated](); // allocated zero'd memory
    _numFilled = 0;
}

/* 析构函数负责清理PQArray类的此实例使用的任何资源
 * 此处删除为PQArray分配的阵列内存
 */
PQArray::~PQArray() {
    delete[] _elements;
}

/*
 * @brief PQArray::enqueue - 无辅助函数版
 * @param elem
 */
void PQArray::enqueue(DataPoint elem) {
    // Edge Case: 没有足够空间
    if(_numFilled == _numAllocated){
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
    // 2. 排序
    for (int i = _numFilled; i > 0; --i) {
        if(_elements[i].priority > _elements[i - 1].priority){
            swap(i, i-1);
        }
    }
    // 3. 更新计数器
    _numFilled++;
}

/*
 * The count of enqueued elements is tracked in the
 * member variable 成员变量 _numFilled.
 */
int PQArray::size() const {
    return _numFilled;
}

/*
 * 数组元素按优先级的降序存储
 * 索引0处的元素最不紧急（优先级值最大）
 * 最后填充的索引中的元素是最紧急的（优先级值最小），位于最前面
 * peek返回最前面的元素（最紧急、最小优先级值）
 */
DataPoint PQArray::peek() const {
    if (isEmpty()) {
        error("PQueue is empty!");
    }
    return _elements[size() - 1];
}

/*
 * 此函数返回最前面元素的值并将其从队列中删除
 * 因为最前面的元素位于最后一个填充的索引，所以减少填充计数就足以将其删除
 */
DataPoint PQArray::dequeue() {
    DataPoint front = peek();
    _numFilled--;
    return front;
}

/*
 * 如果队列中没有元素，则返回true，否则返回false
 */
bool PQArray::isEmpty() const {
    return size() == 0;
}

/*
 * 更新内部状态以反映队列为空，例如，已填充插槽的计数重置为零
 * 阵列内存保持当前容量分配。不需要清除先前存储的元素
 * 当其他元素排队时，槽将被覆盖，但作为一种防御性编程技术，我们用一个哨兵值标记每个槽，作为调试辅助
 */
void PQArray::clear() {
    _numFilled = 0;
}

/*
 * Private member function. 将indexA处的元素与indexB处的元素交换
 */
void PQArray::swap(int indexA, int indexB) {
    DataPoint tmp = _elements[indexA];
    _elements[indexA] = _elements[indexB];
    _elements[indexB] = tmp;
}

/*
 * 打印内部数组的内容以用于调试
 */
void PQArray::printDebugInfo(string msg) const {
    cout << msg << endl;
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << _elements[i] << endl;
    }
}

/*
 * Confirm the internal state of member variables appears valid.
 * 检查数组中的元素是否按优先级顺序存储
 */
void PQArray::validateInternalState() const {
    /*
     * If there are more elements than spots in the array, we have a problem.
     */
    if (_numFilled > _numAllocated) error("Too many elements in not enough space!");

    /* Loop over the elements in the array and compare priority of
     * neighboring elements. If current element has larger priority
     * than the previous, array elements are out of order with respect
     * to our intention. Use error to report this problem.
     */
    for (int i = 1; i < size(); i++) {
        if (_elements[i].priority > _elements[i-1].priority) {
            printDebugInfo("validateInternalState");
            error("PQArray has elements out of order at index " + integerToString(i));
        }
    }
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Add your own custom tests here! */




/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQArray example from writeup") {
    PQArray pq;

    pq.enqueue( { "Zoe", -3 } );
    pq.enqueue( { "Elmo", 10 } );
    pq.enqueue( { "Bert", 6 } );
    EXPECT_EQUAL(pq.size(), 3);
    pq.printDebugInfo("After enqueue 3 elements");

    pq.enqueue( { "Kermit", 5 } );
    EXPECT_EQUAL(pq.size(), 4);
    pq.printDebugInfo("After enqueue one more");

    DataPoint removed = pq.dequeue();
    DataPoint expected = { "Zoe", -3 };
    EXPECT_EQUAL(removed, expected);
    pq.printDebugInfo("After dequeue one");
}

PROVIDED_TEST("PQArray, enqueue only, validate at every step") {
    PQArray pq;

    pq.enqueue({ "e", 2.718 });
    pq.validateInternalState();
    pq.enqueue({ "pi", 3.14 });
    pq.validateInternalState();
    pq.enqueue({ "phi", 1.618 });
    pq.validateInternalState();
    EXPECT_EQUAL(pq.size(), 3);
}

PROVIDED_TEST("PQArray: operations size/isEmpty/clear") {
    PQArray pq;

    EXPECT(pq.isEmpty());
    pq.clear();
    EXPECT_EQUAL(pq.isEmpty(), pq.size() == 0);
    pq.enqueue({ "", 7 });
    EXPECT_EQUAL(pq.size(), 1);
    pq.enqueue({ "", 5 });
    EXPECT_EQUAL(pq.size(), 2);
    pq.enqueue({ "", 5 });
    EXPECT_EQUAL(pq.size(), 3);
    pq.clear();
    pq.validateInternalState();
    EXPECT(pq.isEmpty());
    EXPECT_EQUAL(pq.size(), 0);
}

PROVIDED_TEST("PQArray: dequeue or peek on empty queue raises error") {
    PQArray pq;
    DataPoint point = { "Programming Abstractions", 106 };

    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());

    pq.enqueue(point);
    pq.dequeue();
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());

    pq.enqueue(point);
    pq.clear();
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

PROVIDED_TEST("PQArray, dequeue, validate at every step") {
    PQArray pq;

    pq.enqueue({ "e", 2.718 });
    pq.enqueue({ "pi", 3.14 });
    pq.enqueue({ "phi", 1.618 });

    for (int i = 0; i < 3; i++) {
        pq.dequeue();
        pq.validateInternalState();
    }
}

PROVIDED_TEST("PQArray, test enlarge array memory") {
    for (int size = 5; size <= 500; size *= 5) {
        PQArray pq;

        for (int i = 1; i <= size; i++) {
            pq.enqueue({"", double(i) });
        }
        pq.validateInternalState();

        for (int i = 1; i <= size; i++) {
            DataPoint expected = {"", double(i) };
            EXPECT_EQUAL(pq.dequeue(), expected);
        }
    }
}

PROVIDED_TEST("PQArray, sequence of mixed operations") {
    PQArray pq;
    int size = 30;
    double val = 0;

    for (int i = 0; i < size; i++) {
        pq.enqueue({"", --val });
    }
    val = 0;
    for (int i = 0; i < pq.size(); i++) {
        DataPoint front = pq.peek();
        EXPECT_EQUAL(pq.dequeue(), front);
        pq.enqueue( { "", ++val });
    }
    EXPECT_EQUAL(pq.size(), size);
    val = 0;
    while (!pq.isEmpty()) {
        DataPoint expected = { "", ++val };
        EXPECT_EQUAL(pq.dequeue(), expected);
    }
}

PROVIDED_TEST("PQArray stress test, cycle many random elements in and out") {
    PQArray pq;
    int n = 0, maxEnqueues = 1000;
    double sumEnqueued = 0, sumDequeued = 0;

    setRandomSeed(42); // make test behavior deterministic

    DataPoint mostUrgent = { "", 0 };
    pq.enqueue(mostUrgent);
    while (true) {
        if (++n < maxEnqueues && randomChance(0.9)) {
            DataPoint elem = { "", randomInteger(-10, 10) + 0.5 };
            if (elem.priority < mostUrgent.priority) {
                mostUrgent = elem;
            }
            sumEnqueued += elem.priority;
            pq.enqueue(elem);
        } else {
            DataPoint elem = pq.dequeue();
            sumDequeued += elem.priority;
            EXPECT_EQUAL(elem, mostUrgent);
            if (pq.isEmpty()) break;
            mostUrgent = pq.peek();
        }
    }
    EXPECT_EQUAL(sumEnqueued, sumDequeued);
}

void fillQueue(PQArray& pq, int n) {
    pq.clear(); // start with empty queue
    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", randomReal(0, 10) });
    }
}

void emptyQueue(PQArray& pq, int n) {
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
}

PROVIDED_TEST("PQArray timing test, fillQueue and emptyQueue") {
    PQArray pq;
    int n = 20000;

    TIME_OPERATION(n, fillQueue(pq, n));
    TIME_OPERATION(n, emptyQueue(pq, n));
}
