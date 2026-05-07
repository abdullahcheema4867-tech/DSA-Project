#ifndef QUEUE_H
#define QUEUE_H

template <typename T>
class Queue
{
private:
    static const int MAX_SIZE = 100;
    T elements[MAX_SIZE];
    int front;
    int rear;
    int count;

public:
    Queue() : front(0), rear(-1), count(0) {}

    void enqueue(T value)
    {
        if (count < MAX_SIZE) {
            rear = (rear + 1) % MAX_SIZE;
            elements[rear] = value;
            count++;
        }
    }

    T dequeue()
    {
        if (count > 0) {
            T value = elements[front];
            front = (front + 1) % MAX_SIZE;
            count--;
            return value;
        }
        return T();
    }

    T peek()
    {
        if (count > 0) {
            return elements[front];
        }
        return T();
    }

    bool isEmpty()
    {
        return count == 0;
    }

    bool isFull()
    {
        return count == MAX_SIZE;
    }

    int size()
    {
        return count;
    }
};

#endif // QUEUE_H
