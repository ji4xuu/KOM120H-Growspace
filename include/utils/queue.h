
#ifndef QUEUE_H
#define QUEUE_H

#include <stdexcept>

template <typename T>
class Queue {
private:
    // 1. Definisikan `Node` sebagai building block dari Linked List
    struct Node {
        T data;
        Node* next;
        Node(const T& item) : data(item), next(nullptr) {}
    };

    // 2. Gunakan pointer head & tail, bukan vector
    Node* head;
    Node* tail;
    size_t current_size;

public:
    // Constructor: Inisialisasi antrean kosong
    Queue() : head(nullptr), tail(nullptr), current_size(0) {}

    // Destructor: Pastikan tidak ada memory leak
    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    // 3. Implementasi enqueue (menambah di akhir) - O(1)
    void enqueue(const T& item) {
        Node* newNode = new Node(item);
        if (isEmpty()) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        current_size++;
    }

    // Implementasi tambahan yang emang butuh
    void enqueueFront(const T& item) {
        Node* newNode = new Node(item);
        if (isEmpty()) {
            head = tail = newNode;
        } else {
            newNode->next = head; // Node baru menunjuk ke head lama
            head = newNode;       // Head sekarang adalah node baru
        }
        current_size++;
    }

    // 4. Implementasi dequeue (menghapus di awal) - O(1)
    void dequeue() {
        if (isEmpty()) {
            throw std::runtime_error("Antrean kosong");
        }
        Node* temp = head;
        head = head->next;
        delete temp;
        current_size--;

        if (isEmpty()) {
            tail = nullptr;
        }
    }

    // Mengambil data paling depan
    T front() const {
        if (isEmpty()) {
            throw std::runtime_error("Antrean kosong");
        }
        return head->data;
    }

    // Cek apakah antrean kosong
    bool isEmpty() const {
        return head == nullptr;
    }

    // Mengambil ukuran antrean
    size_t size() const {
        return current_size;
    }
};

#endif // QUEUE_H