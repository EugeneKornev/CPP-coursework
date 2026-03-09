#include <utility>

template<typename T>
class ScopedPointer{
    T* pointer;

public:

    ScopedPointer(T* raw) {
        pointer = raw;
    }

    ~ScopedPointer() {
        delete pointer;
    }

    ScopedPointer(const ScopedPointer& other) {
        if (other.pointer) {
            pointer = new T(*other.pointer);
        } else {
            pointer = nullptr;
        }
    }

    ScopedPointer(ScopedPointer&& other) {
        pointer = other.pointer;
        other.pointer = nullptr;
    }

    ScopedPointer &operator=(const ScopedPointer &other) {
      if (this != &other) {
        delete pointer;
        if (other.pointer) {
          pointer = new T(*other.pointer);
        } else {
          pointer = nullptr;
        }
      }
      return *this;
    }

    ScopedPointer& operator=(ScopedPointer&& other) {
        if (this != &other) {
            delete pointer;
            pointer = other.pointer;
            other.pointer = nullptr;
        }
        return *this;
    }

    T& operator*() { return *pointer; }
    
    const T& operator*() const { return *pointer; }
    
    T* operator->() { return pointer; }
        
    const T* operator->() const { return pointer; }
};


struct Triple{
    int x, y, z;
};

int bar() {
    return 125;
}

int baz() {
    ScopedPointer sp{new Triple{13, 42, 1}};
    
    int val = bar();
    if (val > 13) {
        return 0;
    }

    ScopedPointer sp2 = std::move(sp);
    ScopedPointer sp3 = sp2;
    int s = sp2->x + sp2->y + sp2->z;
    return val + sp2->x + sp2->z;
}

int main() {
    baz();
}