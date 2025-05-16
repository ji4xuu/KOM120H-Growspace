#ifndef QUEUE_H
#define QUEUE_H

#include<vector>
#include<stdexcept>

template <typename T>
class Queue{
    private:
        std::vector<T> elements;
    
    public:
        void enqueue(const T& item){
            elements.push_back(item);
        }

        void dequeue(){
            if (isEmpty()) {
                throw std::runtime_error("Antrian kosong");
            }
            elements.erase(elements.begin()); // hapus elemen paling depan
        }

        T front() const {
            if(isEmpty()){
                throw std::runtime_error("Antrian kosong");
            }
            return elements[0];
        }

        bool isEmpty() const {
            return elements.empty();
        }

        size_t size() const {
            return elements.size();
        }
};

#endif // QUEUE_H