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
        if (objects >= instancesCount) {
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
        if (objects > instancesCount) {
            throw TooMuchInstances(objects);
        }
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


template <typename T, typename U, unsigned int countOfInstances>
struct nInstancesPair : public LimitedAmountOfInstances<nInstancesPair<T, U, countOfInstances>, countOfInstances> {
    T first;
    U second;

    nInstancesPair(T f, U s) : first(f), second(s) {}
};


template <typename T, typename U>
using SingletonPair = nInstancesPair<T, U, 1>;

template <typename T, typename U>
using DoubletonPair = nInstancesPair<T, U, 2>;

