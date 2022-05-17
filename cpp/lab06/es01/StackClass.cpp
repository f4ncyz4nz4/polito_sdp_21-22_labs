#include "StackClass.h"
#include <iosfwd>
#include <iostream>
#include <exception>

template<typename T>
StackClass<T>::StackClass() {
    this->_size = 0;
    this->_dataContainer = new T;
}

template<typename T>
void StackClass<T>::push(const T &val) {
    T *newT = new T[this->_size + 1];
    newT = this->_dataContainer;
    newT[this->_size++] = val;
    this->_dataContainer = newT;
}


template<typename T>
T StackClass<T>::pop() {
    if (this->empty())
        throw StackEmptyException();
    T *newT = new T[this->_size - 1];
    newT = this->_dataContainer;
    T val = newT[--this->_size];
    this->_dataContainer = newT;
    return val;
}

template<typename T>
bool StackClass<T>::empty() const {
    if (this->_size == 0) return true;
    else
        return false;
}

template<typename T>
T *StackClass<T>::getDataContainer() const {
    return this->_dataContainer;
}

template<typename T>
std::vector<T> StackClass<T>::getStackAsVector() {
    std::vector<T> dataVector;

    // INSERT CODE

    return dataVector;
}

template<typename T>
int StackClass<T>::getSize() const {
    return this->_size;
}

template<typename T>
StackClass<T>::~StackClass() {
    // INSERT CODE
}

//copy constructor
template<typename T>
StackClass<T>::StackClass(const StackClass<T> &copyStack) {
    // INSERT CODE
}

//copy operator
template<typename T>
StackClass<T> &StackClass<T>::operator=(const StackClass<T> &copyStack) {
    // INSERT CODE
    return *this;
}

//move constructor
template<typename T>
StackClass<T>::StackClass(StackClass<T> &&other) noexcept {
    // INSERT CODE
}

//move operator
template<typename T>
StackClass<T> &StackClass<T>::operator=(StackClass<T> &&other) noexcept {
    // INSERT CODE
    return *this;
}

template<typename T>
void StackClass<T>::reverse() {
    // INSERT CODE
}

template<typename T>
StackClass<T> StackClass<T>::operator+(const StackClass<T> &toAdd) {
    StackClass<T> output;
    // INSERT CODE
    return output;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const StackClass<T> &stack) {
    for (int i = 0; i < stack.getSize(); ++i) {
        os << stack.getDataContainer()[i] << std::endl;
    }
    return os;
}
