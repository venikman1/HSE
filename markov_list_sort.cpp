#include <iostream>
#include <algorithm>

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
class List {
private:
    Node<T>* head, *tail;
    size_t size;

public:
    List(size_t _size = 0, Node<T>* _head = 0, Node<T>* _tail = 0) {
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

    List split() {
        Node<T>* fast_pointer = begin(), *slow_pointer = begin(), *prev = 0;
        while (fast_pointer != 0 && fast_pointer != tail) {
            prev = slow_pointer;
            slow_pointer = slow_pointer->next;
            fast_pointer = fast_pointer->next->next;
        }
        prev->next = 0;
        size_t another_size = (size + 1) / 2;
        size /= 2;
        Node<T>* another_tail = tail;
        tail = prev;
        return List(another_size, slow_pointer, another_tail);
    }

    size_t get_size() const {
        return size;
    }
};

template <typename T, typename Cmp>
List<T> merge_sort(List<T> queue, Cmp cmp) {
    if (queue.get_size() == 1)
        return queue;
    List<T> queue2 = queue.split();
    queue = merge_sort(queue, cmp);
    queue2 = merge_sort(queue2, cmp);
    List<T> res;
    Node<T>* p1 = queue.begin(), *p2 = queue2.begin();
    while (p1 != 0 || p2 != 0) {
        if (p1 == 0 || (p2 != 0 && cmp(p2->elem, p1->elem))) {
            std::swap(p1, p2);
        }
        Node<T>* t = p1->next;
        res.push_back(p1);
        p1 = t;
    }
    return res;
}

int main() {
    int n;
    cin >> n;
    List<int> queue;
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
