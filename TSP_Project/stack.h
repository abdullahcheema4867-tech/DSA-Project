#ifndef STACK_H
#define STACK_H

template <typename T>
class Stack
{
private:
    static const int MAX_SIZE = 100;
    T elements[MAX_SIZE];
    int top;

public:
    Stack() : top(-1) {}

    void push(T value)
    {
        if (top < MAX_SIZE - 1) {
            elements[++top] = value;
        }
    }

    T pop()
    {
        if (top >= 0) {
            return elements[top--];
        }
        return T();
    }

    T peek()
    {
        if (top >= 0) {
            return elements[top];
        }
        return T();
    }

    bool isEmpty()
    {
        return top == -1;
    }

    bool isFull()
    {
        return top == MAX_SIZE - 1;
    }

    int size()
    {
        return top + 1;
    }
};

#endif // STACK_H
