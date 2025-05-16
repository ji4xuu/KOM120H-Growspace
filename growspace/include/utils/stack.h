#ifndef STACK_H
#define STACK_H

#include<vector>
#include<stdexcept>

template <typename T>
class Stack{
    private:
        std::vector<T> elements;
    
    public:
        void push(const T& item){
            elements.push_back(item);
        }

        void pop(){
            if(isEmpty()){
                throw std::runtime_error("Riwayat kosong");
            }
            elements.pop_back();
        }

        T top() const{
            if(isEmpty()){
                throw std::runtime_error("Riwayat kosong");
            }
            return elements.back();
        }

        bool isEmpty() const{
            return elements.empty();
        }

        size_t size() const{
            return elements.size();
        }
};

#endif // STACK_H