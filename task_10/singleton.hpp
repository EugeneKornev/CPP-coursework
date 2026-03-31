#include <stdexcept>


class TooMuchInstances : public std::runtime_error {
    std::string message;
    
    public:
    
    TooMuchInstances(unsigned int amount) : std::runtime_error("") {
        message = "Too many instances have been created: " + std::to_string(amount + 1);
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
    
};

template <class Derived, unsigned int instancesCount>
struct LimitedAmountOfInstances {
    static unsigned int objects;

    LimitedAmountOfInstances() {
        if (++objects >= instancesCount) {
            throw TooMuchInstances(instancesCount);
        }
        objects++;
    }

    ~LimitedAmountOfInstances() {
        objects--;
    }

    LimitedAmountOfInstances(const LimitedAmountOfInstances& other) {
        if (objects == instancesCount) {
            throw TooMuchInstances(objects);
        }
        objects++;
    }

    LimitedAmountOfInstances(LimitedAmountOfInstances&& other) {
        objects++;
    }

    LimitedAmountOfInstances& operator=(const LimitedAmountOfInstances& other) {
        return *this;
    }

    LimitedAmountOfInstances& operator=(LimitedAmountOfInstances&& other) {
        return *this;
    }
};

template <class Derived, unsigned int instancesCount> unsigned int LimitedAmountOfInstances<Derived, instancesCount>::objects = 0;

template <class Derived>
using Singleton = LimitedAmountOfInstances<Derived, 1>;


template <typename T, typename U>
struct SingletonPair : Singleton<SingletonPair<T, U>> {
  T first;
  U second;

  SingletonPair(T f, U s) : first(f), second(s) {}
};

