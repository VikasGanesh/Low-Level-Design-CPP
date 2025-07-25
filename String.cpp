#include <iostream>
#include <bits/stdc++.h>

//https://github.com/cppnuts-yt/CppNuts/blob/master/Write%20Your%20Own/own%20string%20class.cpp

using namespace std;

class String {
private:
    char* ptr;
public:
    String () {
        ptr = new char[1];
        ptr[0] = '\0';
    }
    
    String (const char* other) {
        ptr = new char[strlen(other) + 1];
        strcpy(ptr, other);
    }
    
    //copy constructor
    String (const String& other) {
        if (ptr) {
            delete[] ptr;
        }
        ptr = new char[strlen(other.ptr) + 1];
        strcpy(ptr, other.ptr);
    }
    
    //copy assignment constructor
    String& operator=(const String& other) {
        if (this == &other) {
            return *this;
        }
        if (ptr) {
            delete[] ptr;
        }
        ptr = new char[strlen(other.ptr) + 1];
        strcpy(ptr, other.ptr);
        return *this;
    }
    
    //move constructor
    String (String&& other) {
        ptr = other.ptr;
        other.ptr = NULL;
    }
    
    //move asisgnment constructor
    String& operator=(String&& other) {
        if (this == &other) {
            return *this;
        }
        if (ptr) {
            delete[] ptr;
        }
        ptr = other.ptr;
        other.ptr = NULL;
        return *this;
    }
    
    ~String () {
        delete[] ptr;
    }
    
    String operator+(const String& other) {
        char* newptr = new char[strlen(ptr) + strlen(other.ptr) + 1];
        strcpy(newptr, ptr);
        strcat(newptr, other.ptr);
        String result(newptr);
        delete[] newptr;
        return result;
    }
    
    char& operator[] (int index) {
        if (index >= strlen(ptr)) {
            throw std::out_of_range("Index out of range");
        }
        return ptr[index];
    }
    
    bool operator==(String& other){
        return strcmp(other.ptr, ptr) == 0;
    }
    
    size_t length() {
        return strlen(ptr);
    }
    
    const char* c_str() const {
        return ptr;
    }
    
    String substr(size_t index, size_t len) {
        if (index >= strlen(ptr) || index + len > strlen(ptr)) {
            throw std::out_of_range("Index out of range");
        }
        
        char* newptr = new char[len + 1];
        strncpy(newptr, ptr + index, len);
        newptr[len] = '\0';
        String result(newptr);
        delete[] newptr;
        return result;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const String& str) {
        os << (str.ptr) ? str.ptr : "";
        return os;
    }

    // Input stream overload
    friend std::istream& operator>>(std::istream& is, String& str) {
        char buffer[1000];  // Fixed buffer size
        is >> buffer;

        delete[] str.ptr;
        str.ptr = new char[strlen(buffer) + 1];
        strcpy(str.ptr, buffer);

        return is;
    }
};

int main() {
  String s;
  std::cout << s.length() << std::endl;

  String a("Hello");
  String b(" World");
  String c = a + b;

  std::cout << "Concatenated: " << c << "\n"; // Hello World

  std::cout << "Length: " << c.length() << "\n";         // 11
  std::cout << "C-String: " << c.c_str() << "\n";        // Hello World
  std::cout << "Substring (6,5): " << c.substr(6, 5) << "\n"; // World

  std::cout << "Char at index 0: " << c[0] << "\n";       // H

  String d("Hello World CppNuts");
  std::cout << "c == d: " << (c == d ? "True" : "False") << "\n"; // True

  String e = std::move(c);
  std::cout << "Moved string: " << e << "\n"; // Hello World
  std::cout << "Original after move: " << c << "\n"; // (empty or garbage-safe)

  return 0;
}
