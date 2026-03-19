#include <vector>
#include <span>

typedef unsigned int uint;

class Matrix {
    uint size = 0;
    double** data = nullptr;

    void allocate();

    void deallocate();

    double& operator()(uint x, uint y);

    void check_size(const Matrix&);

    void zero();

public:
    Matrix(uint size_);

    Matrix(std::vector<double>);
    
    ~Matrix();
    
    Matrix(const Matrix&);

    Matrix(Matrix&&);

    Matrix& operator=(const Matrix&);

    Matrix& operator=(Matrix&&);

    Matrix operator+(const Matrix&);

    Matrix& operator+=(const Matrix&);

    Matrix operator*(const Matrix&);

    Matrix& operator*=(const Matrix&);

    Matrix operator*(double);

    Matrix& operator*=(double);

    bool operator==(const Matrix&);

    bool operator!=(const Matrix&);

    Matrix operator-();

    Matrix operator-(const Matrix&);

    Matrix& operator-=(const Matrix&);

    explicit operator double() const;

    std::span<double> operator[](uint);
};
