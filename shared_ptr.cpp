#include <iostream>

template <class T>
class SharedPtr {
private:
    T* ptr;
    int* refCount;

public:
    // Constructor
    explicit SharedPtr(T* p = nullptr) {
        ptr = p;
        refCount = new int(1);  // Initialize reference count
    }

    // Copy Constructor
    SharedPtr(const SharedPtr& other) {
        ptr = other.ptr;
        refCount = other.refCount;
        (*refCount)++;
    }

    // Copy Assignment
    SharedPtr& operator=(const SharedPtr& other) {
        if (this == &other) return *this; // Self-assignment check
        
        // Release old resource
        if (refCount && --(*refCount) == 0) {
            delete ptr;
            delete refCount;
        }

        // Copy new resource
        ptr = other.ptr;
        refCount = other.refCount;
        (*refCount)++;

        return *this;
    }

    // Move Constructor
    SharedPtr(SharedPtr&& other) noexcept {
        ptr = other.ptr;
        refCount = other.refCount;
        
        other.ptr = nullptr;
        other.refCount = nullptr;
    }

    // Move Assignment
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this == &other) return *this; // Self-assignment check

        // Release old resource
        if (refCount && --(*refCount) == 0) {
            delete ptr;
            delete refCount;
        }

        // Transfer ownership
        ptr = other.ptr;
        refCount = other.refCount;

        other.ptr = nullptr;
        other.refCount = nullptr;

        return *this;
    }

    // Destructor
    ~SharedPtr() {
        reset(); // Use reset to clean up
    }

    // Dereference Operator
    T& operator*() const {
        return *ptr;
    }

    // Arrow Operator
    T* operator->() const {
        return ptr;
    }

    // Get reference count
    int use_count() const {
        return refCount ? *refCount : 0;
    }

    // Get the raw pointer
    T* get() const {
        return ptr;
    }

    // Reset method (release old ownership and optionally assign new pointer)
    void reset(T* newPtr = nullptr) {
        if (ptr != newPtr) {  
            // Decrement reference count of current pointer
            if (refCount && --(*refCount) == 0) {
                delete ptr;
                delete refCount;
            }

            // Assign new pointer
            ptr = newPtr;
            refCount = newPtr ? new int(1) : nullptr;
        }
    }
};

// Example Usage
int main() {
    SharedPtr<int> sp1(new int(42));
    std::cout << "sp1 use count: " << sp1.use_count() << std::endl;

    SharedPtr<int> sp2 = std::move(sp1); // Move constructor
    std::cout << "After move, sp1 use count: " << sp1.use_count() << std::endl;
    std::cout << "sp2 use count: " << sp2.use_count() << std::endl;

    SharedPtr<int> sp3;
    sp3 = std::move(sp2); // Move assignment
    std::cout << "After move assignment, sp2 use count: " << sp2.use_count() << std::endl;
    std::cout << "sp3 use count: " << sp3.use_count() << std::endl;

    std::cout << "Raw pointer from sp3: " << sp3.get() << std::endl;

    sp3.reset(new int(99));  // Reset with a new pointer
    std::cout << "After reset, sp3 use count: " << sp3.use_count() << std::endl;
    std::cout << "Value in sp3: " << *sp3 << std::endl;

    sp3.reset();  // Reset to nullptr
    std::cout << "After reset to nullptr, sp3 use count: " << sp3.use_count() << std::endl;

    return 0;
}



#include <iostream>
#include <memory>

class Test {
public:
    void show() { std::cout << "Test::show() called\n"; }
};

int main() {
    std::shared_ptr<Test> sp = std::make_shared<Test>();
    sp->show();  // Works, calls operator->() then show()
    
    (*sp).show(); // Also works, expands to (*ptr).show()

    return 0;
}
