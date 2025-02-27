#include <iostream>

template <class T>
class SharedPtr;  // Forward declaration

template <class T>
class WeakPtr {
private:
    T* ptr;
    int* refCount;
    int* weakCount;

public:
    WeakPtr() : ptr(nullptr), refCount(nullptr), weakCount(nullptr) {}

    // Construct from a SharedPtr
    WeakPtr(const SharedPtr<T>& shared) : ptr(shared.ptr), refCount(shared.refCount), weakCount(shared.weakCount) {
        if (weakCount) {
            (*weakCount)++;
        }
    }

    // Copy Constructor
    WeakPtr(const WeakPtr& other) : ptr(other.ptr), refCount(other.refCount), weakCount(other.weakCount) {
        if (weakCount) {
            (*weakCount)++;
        }
    }

    // Move Constructor
    WeakPtr(WeakPtr&& other) noexcept : ptr(other.ptr), refCount(other.refCount), weakCount(other.weakCount) {
        other.ptr = nullptr;
        other.refCount = nullptr;
        other.weakCount = nullptr;
    }

    // Copy Assignment
    WeakPtr& operator=(const WeakPtr& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            refCount = other.refCount;
            weakCount = other.weakCount;
            if (weakCount) {
                (*weakCount)++;
            }
        }
        return *this;
    }

    // Move Assignment
    WeakPtr& operator=(WeakPtr&& other) noexcept {
        if (this != &other) {
            release();
            ptr = other.ptr;
            refCount = other.refCount;
            weakCount = other.weakCount;
            other.ptr = nullptr;
            other.refCount = nullptr;
            other.weakCount = nullptr;
        }
        return *this;
    }

    // Destructor
    ~WeakPtr() {
        release();
    }

    // Check if the object is still valid
    bool expired() const {
        return !refCount || *refCount == 0;
    }

    // Convert WeakPtr to SharedPtr
    SharedPtr<T> lock() const {
        return (expired()) ? SharedPtr<T>() : SharedPtr<T>(*this);
    }

private:
    void release() {
        if (weakCount && --(*weakCount) == 0) {
            delete weakCount;
        }
    }

    // Grant access to SharedPtr
    friend class SharedPtr<T>;
};

template <class T>
class SharedPtr {
private:
    T* ptr;
    int* refCount;
    int* weakCount;

public:
    explicit SharedPtr(T* ptr = nullptr) : ptr(ptr), refCount(new int(1)), weakCount(new int(0)) {}

    // Copy Constructor
    SharedPtr(const SharedPtr& other) : ptr(other.ptr), refCount(other.refCount), weakCount(other.weakCount) {
        if (refCount) {
            (*refCount)++;
        }
    }

    // Move Constructor
    SharedPtr(SharedPtr&& other) noexcept : ptr(other.ptr), refCount(other.refCount), weakCount(other.weakCount) {
        other.ptr = nullptr;
        other.refCount = nullptr;
        other.weakCount = nullptr;
    }

    // Copy Assignment
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            refCount = other.refCount;
            weakCount = other.weakCount;
            if (refCount) {
                (*refCount)++;
            }
        }
        return *this;
    }

    // Move Assignment
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            release();
            ptr = other.ptr;
            refCount = other.refCount;
            weakCount = other.weakCount;
            other.ptr = nullptr;
            other.refCount = nullptr;
            other.weakCount = nullptr;
        }
        return *this;
    }

    // Destructor
    ~SharedPtr() {
        release();
    }

    // Dereference operator
    T& operator*() const {
        return *ptr;
    }

    // Arrow operator
    T* operator->() const {
        return ptr;
    }

private:
    void release() {
        if (refCount && --(*refCount) == 0) {
            delete ptr;
            if (*weakCount == 0) {
                delete refCount;
                delete weakCount;
            }
        }
    }

    // Grant access to WeakPtr
    friend class WeakPtr<T>;
};

int main() {
    SharedPtr<int> sp1(new int(42));
    WeakPtr<int> wp1(sp1);

    std::cout << "WeakPtr expired? " << wp1.expired() << std::endl;  // Output: 0 (false)
    
    if (auto sp2 = wp1.lock()) {  // Convert WeakPtr to SharedPtr
        std::cout << "SharedPtr value: " << *sp2 << std::endl;
    }

    sp1 = nullptr;  // Destroy the shared pointer

    std::cout << "WeakPtr expired after SharedPtr reset? " << wp1.expired() << std::endl;  // Output: 1 (true)

    return 0;
}
