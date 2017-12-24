#include <iostream>
#include <vector>

using std::cin;
using std::cout;

template <typename T>
struct Node {
    Node(T e, Node* next) {
        elem = e;
        this->next = next;
    }

    T elem;
    Node* next;
};

template <typename T>
class Queue{
private:
    Node<T>* head, *tail;
    size_t size;

public:
    Queue(size_t _size = 0, Node<T>* _head = 0, Node<T>* _tail = 0) {
        size = _size;
        head = _head;
        tail = _tail;
    }

    Node<T>* begin() const {
        return head;
    }

    Node<T>* end() const {
        return 0;
    }

    void push_back(Node<T>* node) {
        ++size;
        if (head == 0) {
            head = tail = node;
            node->next = 0;
        }
        else {
            tail->next = node;
            tail = node;
            node->next = 0;
        }
    }

    Queue split() {
        Node<T>* center = begin(), *prev = 0;
        for (size_t i = 0; i != size / 2; ++i) {
            prev = center;
            center = center->next;
        }
        prev->next = 0;
        size_t another_size = size - size / 2;
        size /= 2;
        Node<T>* another_tail = tail;
        tail = prev;
        return Queue(another_size, center, another_tail);
    }

    size_t get_size() const {
        return size;
    }
};

template <typename T, typename Cmp>
Queue<T> merge_sort(Queue<T> queue, Cmp cmp) {
    if (queue.get_size() == 1)
        return queue;
    Queue<T> queue2 = queue.split();
    queue = merge_sort(queue, cmp);
    queue2 = merge_sort(queue2, cmp);
    Queue<T> res;
    Node<T>* p1 = queue.begin(), *p2 = queue2.begin();
    while (p1 != queue.end() && p2 != queue2.end()) {
        if (!cmp(p2->elem, p1->elem)) {
            Node<T>* t = p1->next;
            res.push_back(p1);
            p1 = t;
        }
        else {
            Node<T>* t = p2->next;
            res.push_back(p2);
            p2 = t;
        }
    }
    while (p1 != queue.end()) {
        Node<T>* t = p1->next;
        res.push_back(p1);
        p1 = t;
    }
    while (p2 != queue2.end()) {
        Node<T>* t = p2->next;
        res.push_back(p2);
        p2 = t;
    }
    return res;
}

int main() {
    int n;
    cin >> n;
    Queue<int> queue;
    for (int i = 0; i != n; ++i) {
        int a;
        cin >> a;
        queue.push_back(new Node<int>(a, 0));
    }
    queue = merge_sort(queue, [](int a, int b) -> bool {return a < b; });
    for (Node<int>* p = queue.begin(); p != queue.end(); ) {
        cout << p->elem << " ";
        Node<int>* t = p;
        p = p->next;
        delete t;
    }
    return 0;
}
