#include <iostream>

class A {
public:
    int data = 42;
};

template <class T>
class UniquePtr {
private:
    T* ptr;

public:
    // Explicit constructor to prevent implicit conversions
    explicit UniquePtr(T* ptr = nullptr) : ptr(ptr) {}

    // Delete copy constructor and copy assignment
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // Move constructor
    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    // Move assignment operator
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this == &other) return *this;

        delete ptr;  // Clean up existing resource

        ptr = other.ptr;
        other.ptr = nullptr;

        return *this;
    }

    // Destructor
    ~UniquePtr() { delete ptr; }

    // Overloaded operators
    T* operator->() { return ptr; }
    T& operator*() { return *ptr; }
};

int main() {
    UniquePtr<A> uptr(new A());  // Correct way to initialize UniquePtr
    std::cout << (*uptr).data << std::endl;  // Access member
    return 0;
}
