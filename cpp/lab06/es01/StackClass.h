#ifndef ES01LAB06_STACKCLASS_H
#define ES01LAB06_STACKCLASS_H

#include <vector>
#include <iosfwd>


//these two lines needed for the definition of the friend operator
template<typename T>
class StackClass;

template<typename T>
std::ostream &operator<<(std::ostream &, const StackClass<T> &);

template<typename T>
class StackClass {
public:
    StackClass(); // Pre-allocation constructor

    ~StackClass(); // destructor

    //copy constructor
    StackClass(const StackClass<T> &);

    // copy assignment
    StackClass<T> &operator=(const StackClass<T> &);

    //move constructor
    StackClass(StackClass<T> &&) noexcept;

    //move assignment
    StackClass<T> &operator=(StackClass<T> &&) noexcept;

    void push(const T &val);

    T pop();

    bool empty() const;

    T *getDataContainer() const;

    int getSize() const;

    std::vector<T> getStackAsVector();

    void reverse();

    friend std::ostream &operator
    <<<T>(std::ostream &, const StackClass<T> &);

    StackClass<T> operator+(const StackClass<T> &);

private:
    T *_dataContainer;
    int _size;
};

template<class T>
std::ostream &operator<<(std::ostream &, const StackClass<T> &);

class StackEmptyException {
public:
    StackEmptyException()
            : message("The stack is empty!") {}

    const char *what() const { return message; }

private:
    const char *message;
};


#endif //ES01LAB06_STACKCLASS_H
