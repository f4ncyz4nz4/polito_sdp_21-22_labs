#include "StackClass.h"
#include <iosfwd>
#include <iostream>
#include <vector>
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
    for (int i = this->_size - 1; i >= 0; i--) {
        dataVector.push_back(this->_dataContainer[i]);
    }
    return dataVector;
}

template<typename T>
int StackClass<T>::getSize() const {
    return this->_size;
}

template<typename T>
StackClass<T>::~StackClass() {
    delete[] _dataContainer;
    _dataContainer = nullptr;
    _size = 0;
}

//copy constructor
template<typename T>
StackClass<T>::StackClass(const StackClass<T> &copyStack) {
    this->_size = copyStack._size;
    this->_dataContainer = new T[this->_size];
    for (int i = 0; i < this->_size; ++i)
        this->_dataContainer[i] = copyStack._dataContainer[i];
}

//copy operator
template<typename T>
StackClass<T> &StackClass<T>::operator=(const StackClass<T> &copyStack) {
    this->_size = copyStack._size;
    delete[] this->_dataContainer;
    this->_dataContainer = new T[this->_size];
    for (int i = 0; i < this->_size; ++i)
        this->_dataContainer[i] = copyStack._dataContainer[i];

    return *this;
}

//move constructor
template<typename T>
StackClass<T>::StackClass(StackClass<T> &&other) noexcept {
    this->_size = other._size;
    this->_dataContainer = other._dataContainer;
    other._size = 0;
    other._dataContainer = nullptr;
}

//move operator
template<typename T>
StackClass<T> &StackClass<T>::operator=(StackClass<T> &&other) noexcept {
    this->_size = other._size;
    delete[] this->_dataContainer;
    this->_dataContainer = new T[this->_size];
    for (int i = 0; i < this->_size; ++i)
        this->_dataContainer[i] = other._dataContainer[i];
    other._size = 0;
    other._dataContainer = nullptr;

    return *this;
}

template<typename T>
void StackClass<T>::reverse() {
    T *reversedT = new T[this->_size];
    for (int i = 0; i < this->_size; ++i) {
        reversedT[i] = this->_dataContainer[this->_size - i - 1];
    }
    this->_dataContainer = reversedT;
}

template<typename T>
StackClass<T> StackClass<T>::operator+(const StackClass<T> &toAdd) {
    StackClass<T> output;
    output._dataContainer = new T[this->_size + toAdd.getSize()];
    for (int i = 0; i < toAdd.getSize(); i++)
        output._dataContainer[i] = toAdd._dataContainer[i];
    for (int i = toAdd._size; i < this->_size + toAdd._size; i++)
        output._dataContainer[i] = this->_dataContainer[i - toAdd._size];
    output._size = this->getSize() + toAdd.getSize();
    return output;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const StackClass<T> &stack) {
    for (int i = stack.getSize() - 1; i >= 0; i--) {
        os << stack.getDataContainer()[i] << std::endl;
    }
    return os;
}
