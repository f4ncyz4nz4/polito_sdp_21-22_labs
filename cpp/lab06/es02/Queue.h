#ifndef ES02LAB06_QUEUE_H
#define ES02LAB06_QUEUE_H


#include <iosfwd>

enum Client {PRIORITY=1, POSTAL=2, MONEY=3};

class Queue {
public:
    Queue();
    enum Client pop();
    int push(enum Client);
    bool empty();
    void printQueue();
private:
    void _pushAt(enum Client, int);
    enum Client* _queue;
    int _size;
};

class QueueEmptyException {
public:
    QueueEmptyException()
            : message( "The queue is empty!" ) { }
    const char *what() const { return message; }
private:
    const char *message;
};



#endif //ES02LAB06_QUEUE_H
