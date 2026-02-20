#include <optional>
#include <assert.h>
#include <math.h>

using namespace std;

const double eps = 1e-5;

struct Point{
    double x_;
    double y_;

    Point(double x = 0.0, double y = 0.0) {
        x_ = x;
        y_ = y;
    }

    ~Point() {};

    bool eps_neighbourhood(const Point& other) {
        if (sqrt(pow(x_ - other.x_, 2) + pow(y_ - other.y_, 2)) < eps) {
            return true;
        }
        return false;
    } 
};

class Line{
    double A_;
    double B_;
    double C_;

public:
    Line(const Point& p1, const Point& p2) {
        A_ = -(p2.x_ - p1.x_);
        B_ = p2.y_ - p1.y_;
        C_ = B_ * p1.x_ + A_ * p1.y_;
    }

    Line(double a, double b, double c) {
        A_ = a;
        B_ = b;
        C_ = c;
    }

    optional<Point> intersection(const Line& other) const {
        double delta = A_ * other.B_ - B_ * other.A_;
        if (delta < eps) {
            return nullopt;
        }
        return Point{(- C_ * other.B_ + B_ * other.C_) / delta,
         (- A_ * other.C_ + C_ * other.A_) / delta};
    }

    Line normal_at(const Point& p) const {
        return Line{-B_, A_, B_ * p.x_ - A_ * p.y_};
    }

    bool equal(const Line& other) {
        return ((A_ - other.A_) < eps && (B_ - other.B_) < eps && (C_ - other.C_) < eps);
    }
};


void parallel_case() {
    Line l1{1, 2, 3};
    Line l2{1, 2, 4};
    auto p = l1.intersection(l2);
    assert(!p.has_value());
}


void intersect_case() {
    Line l1{2, -1, -10};
    Line l2{1, 2, -11};
    auto p = l1.intersection(l2);
    assert(p.has_value());
    Point s = Point{6.2, 2.4};
    assert(s.eps_neighbourhood(p.value()));
}

void normal_case() {
    Line l1{2, -1, -10};
    Point p{6.2, 2.4};
    Line l2{1, 2, -11};
    Line lr = l1.normal_at(p);
    assert(lr.equal(l2));
}

int main() {
    parallel_case();
    intersect_case();
    normal_case();
    return 0;
}