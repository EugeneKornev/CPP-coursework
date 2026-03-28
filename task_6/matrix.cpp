#include "matrix.hpp"
#include <stdexcept>
#include <span>
#include <iostream>

void Matrix::allocate() {
    this->data = new double*[size];
    for (int i = 0; i < size; i++) {
        data[i] = new double[size];
    }
}

void Matrix::deallocate() {
    for (uint i = 0; i < size; i++) {
        delete[] data[i];
    }
    delete[] data;
}

void Matrix::check_size(const Matrix& other) {
    if (size != other.size) {
        throw std::runtime_error("Matrix sizes don't match!");
    }
}


void Matrix::zero() {
    for (uint i = 0; i < size; i++) {
        for (uint j = 0; j < size; j++) {
            data[i][j] = 0.0;
        }
    }
}

Matrix::~Matrix() {
    deallocate();
}


Matrix::Matrix(std::vector<double> src) : size(src.size()) {
    data = new double*[size];
    for (uint i = 0; i < size; i++) {
        data[i] = new double[size];
    }
    zero();
    for (uint i = 0; i < size; i++) {
        data[i][i] = src[i];
    }
}


Matrix::Matrix(uint size_) : size(size_) {
    allocate();
    zero();
}


double& Matrix::operator()(uint x, uint y) {
    return data[x][y];
}


Matrix::Matrix(const Matrix& other) : size(other.size) {
    //std::cout << "Copy ctr" << std::endl;
    allocate();
    for (uint i = 0; i < size; i++) {
        for (uint j = 0; j < size; j++) {
            this->data[i][j] = other.data[i][j];
        }
    }
}


Matrix::Matrix(Matrix&& other) : size(other.size) {
    //std::cout << "Move ctr" << std::endl;
    data = other.data;
    size = other.size;
    other.size = 0;
    other.data = nullptr;
}


Matrix& Matrix::operator=(Matrix&& other) {
    //std::cout << "Move assign" << std::endl;
    if (this != &other) {
        if (this->data != nullptr) {
            deallocate();
        }
        size = other.size;
        data = other.data;
        other.data = nullptr;
        other.size = 0;
    }
    return *this;
}


Matrix::operator double() const {
    double sum = 0.0;
    for (uint i = 0; i < size; i++) {
        for (uint j = 0; j < size; j ++) {
            sum += data[i][j];
        }
    }
    return sum;
}


Matrix& Matrix::operator=(const Matrix& other) {
    //std::cout << "Copy assign" << std::endl;
    if (this != &other) {
        deallocate();
        size = other.size;
        allocate();
        for (uint i = 0; i < size; i++) {
            for (uint j = 0; j < size; j++) {
                data[i][j] = other.data[i][j];
            }
        }
    }
    return *this;
}


Matrix& Matrix::operator+=(const Matrix& other) {
    check_size(other);
    for (uint i = 0; i < size; i++) {
        for (uint j = 0; j < size; j++) {
            data[i][j] += other.data[i][j];
        }
    }
    return *this;
}


Matrix Matrix::operator+(const Matrix& other) {
    Matrix temp(*this);
    return (temp += other);
}


Matrix& Matrix::operator-=(const Matrix& other) {
    check_size(other);
    for (uint i = 0; i < size; i++) {
        for (uint j = 0; j < size; j++) {
            data[i][j] -= other.data[i][j];
        }
    }
    return *this;

}


Matrix Matrix::operator-(const Matrix& other) {
    Matrix temp(*this);
    return (temp -= other);
}


Matrix& Matrix::operator*=(const Matrix& other) {
    check_size(other);
    Matrix temp(size);
    temp.zero();
    for (uint i = 0; i < size; i++) {
        for (uint j = 0; j < size; j++) {
            for (uint k = 0; k < size; k++) {
                temp.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return (*this = temp);
}


Matrix Matrix::operator*(const Matrix& other) {
    Matrix temp(*this);
    return (temp *= other);
}


Matrix& Matrix::operator*=(double n) {
    for (uint i = 0; i < size; i++) {
        for (uint j = 0; j < size; j++) {
            data[i][j] *= n;
        }
    }
    return *this;
}


Matrix Matrix::operator*(double n) {
    Matrix temp(*this);
    return (temp *= n);
}


Matrix Matrix::operator-() {
    Matrix temp(size);
    temp -= *this;
    return temp;
}


bool Matrix::operator==(const Matrix& other) {
    if (size != other.size) {
        return false;
    }
    for (uint i = 0; i < size; i++) {
        for (uint j = 0; j < size; j++) {
            if (data[i][j] != other.data[i][j]) {
                return false;
            }
        }
    }
    return true;
}


bool Matrix::operator!=(const Matrix& other) {
    return !(*this == other);
}

std::span<double> Matrix::operator[](uint index) {
    if (index >= size) {
        throw std::runtime_error("Index out of bound!");
    }
    return std::span<double>(data[index], size);
}