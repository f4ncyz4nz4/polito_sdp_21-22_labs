#include "Queue.h"
#include <iostream>
#include <vector>

Queue::Queue() {
    _size = 0;
    _priority = 0;
    _postal = 0;
    _money = 0;
    _queue = nullptr;
}

enum Client Queue::pop() {
    enum Client val;
    enum Client *oldQueue = _queue;
    val = oldQueue[_size - 1];
    enum Client *newQueue = new enum Client[_size - 1];
    for (int i = 0; i < _size - 1; i++)
        newQueue[i] = oldQueue[i];
    switch (val) {
        case 1:
            _priority--;
            break;
        case 2:
            _postal--;
            break;
        case 3:
            _money--;
            break;
        default:
            break;
    }
    _size--;
    _queue = newQueue;
    delete[] oldQueue;
    return val;
}

bool Queue::empty() {
    if (_size == 0) return true;
    else
        return false;
}

void Queue::_pushAt(enum Client value, int position) {
    enum Client *oldQueue = _queue;
    enum Client *newQueue = new enum Client[_size + 1];
    int postal = 0;
    if (value != 2) {
        for (int i = 0; i < (_size - position); i++)
            newQueue[i] = oldQueue[i];
        newQueue[_size - position] = value;
        for (int i = (_size - position) + 1; i < _size + 1; i++)
            newQueue[i] = oldQueue[i - 1];
    } else {
        for (int i = _size; i > 0; i--) {
            newQueue[i] = oldQueue[i-1];
            if (newQueue[i] == 3) {
                postal++;
                if (postal == 3) {
                    newQueue[--i] = value;
                    for (i = i - 1; i >= 0; i--)
                        newQueue[i] = oldQueue[i];
                    break;
                }
            }
        }
    }
    _size++;
    _queue = newQueue;
    delete[]
            oldQueue;
}

void Queue::printQueue() {
    for (int i = _size - 1; i >= 0; i--) {
        std::cout << _queue[i] << std::endl;
    }
}

int Queue::push(enum Client client) {
    switch (client) {
        case 1:
            _pushAt(client, 0);
            _priority++;
            break;
        case 2:
            _pushAt(client,0);
            _postal++;
            break;
        case 3:
            _pushAt(client, _size);
            _money++;
            break;
    }
    return 1;
}

//copy operator
Queue &Queue::operator=(const Queue &copyQueue) {
    if (this == &copyQueue)
        return *this;
    delete[] _queue;
    _size = copyQueue._size;
    for (int i = 0; i < _size; i++)
        _queue[i] = copyQueue._queue[i];
    return *this;

}



