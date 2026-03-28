#include <optional>
#include <assert.h>
#include <math.h>
#include <iostream>

const double eps = 1e-5;

struct Point{

    double x_;
    double y_;


    Point(double x = 0.0, double y = 0.0) {
        x_ = x;
        y_ = y;
    }

    ~Point() {};

    bool eps_neighbourhood(const Point& other) const {
        return sqrt(pow(x_ - other.x_, 2) + pow(y_ - other.y_, 2)) < eps;
    } 
};

class Line{
    double A_;
    double B_;
    double C_;

    Line(const Point& p1, const Point& p2) {
        A_ = p2.y_ - p1.y_;
        B_ = p1.x_ - p2.x_;
        C_ = - p1.y_ * B_ - p1.x_ * A_;
    }

    Line(double a, double b, double c) {
        A_ = a;
        B_ = b;
        C_ = c;
    }


public:

    void print() {
        std::cout << A_ << " " << B_ << " " << C_ << std::endl;
    }

    static std::optional<Line> createLine(const Point& p1, const Point& p2) {
        if (p1.eps_neighbourhood(p2)) {
            return std::nullopt;
        }
        return Line{p1, p2};
    }

    static std::optional<Line> createLine(double a, double b, double c) {
        if (a == 0 && b == 0) { 
            return std::nullopt;
        }
        return Line{a, b, c};
    }
    
    std::optional<Point> intersection(const Line& other) const {
        double delta = A_ * other.B_ - B_ * other.A_;
        if (std::abs(delta) < eps) {
            return std::nullopt;
        }
        return Point{(- C_ * other.B_ + B_ * other.C_) / delta,
         ( - A_ * other.C_ + C_ * other.A_) / delta};
    }

    Line normal_at(const Point& p) const {
        return Line{-B_, A_, B_ * p.x_ - A_ * p.y_};
    }

    bool equal(const Line& other) {
        double n1 = std::sqrt(std::pow(A_, 2) + std::pow(B_, 2));
        double n2 = std::sqrt(std::pow(other.A_, 2) + std::pow(other.B_, 2));
        if (A_ * other.A_ < 0 || B_ * other.B_ < 0) {
            return (abs(A_ / n1 + other.A_ / n2) < eps && abs(B_ / n1 + other.B_ / n2) < eps && abs(C_ / n1 + other.C_ / n2) < eps); // Line{this} == -Line{other}
        }
        return (abs(A_ / n1 - other.A_ / n2) < eps && abs(B_ / n1 - other.B_ / n2) < eps && abs(C_ / n1 - other.C_ / n2) < eps);
    }
};


void parallel_case_coef() {
    std::optional<Line> l1 = Line::createLine(1, 2, 3);
    std::optional<Line> l2 = Line::createLine(1, 2, 4);
    auto p = l1.value().intersection(l2.value());
    assert(!p.has_value());
}

void parallel_case_point() {
    std::optional<Line> l1 = Line::createLine(Point{-3, 0}, Point{0, -1.5});
    std::optional<Line> l2 = Line::createLine(Point{-4, 0}, Point{0, -2});
    auto p = l2.value().intersection(l2.value());
    assert(!p.has_value());
}


void intersect_case_coef() {
    std::optional<Line> l1 = Line::createLine(2, -1, -10);
    std::optional<Line> l2 = Line::createLine(1, 2, -11);
    auto p = l1.value().intersection(l2.value());
    assert(p.has_value());
    Point s = Point{6.2, 2.4};
    assert(p.value().eps_neighbourhood(s));
}

void intersect_case_point() {
    std::optional<Line> l1 = Line::createLine(Point{0, -10}, Point{5, 0});
    std::optional<Line> l2 = Line::createLine(Point{0, 5.5}, Point{11, 0});
    auto p = l1.value().intersection(l2.value());
    assert(p.has_value());
    Point s{6.2, 2.4};
    assert(p.value().eps_neighbourhood(s));
    
}

void normal_case_coef() {
    std::optional<Line> l1 = Line::createLine(2, -1, -10);
    Point p{6.2, 2.4};
    std::optional<Line> l2 = Line::createLine(1, 2, -11);
    Line lr = l1.value().normal_at(p);
    assert(lr.equal(l2.value()));
}

void normal_case_point() {
    std::optional<Line> l1 = Line::createLine(Point{5, 0}, Point{0, -10});
    Point p{6.2, 2.4};
    std::optional<Line> l2 = Line::createLine(Point{11, 0}, Point{0, 5.5});
    Line lr = l1.value().normal_at(p);
    assert(lr.equal(l2.value()));
}

int main() {
    parallel_case_coef();
    parallel_case_point();
    intersect_case_coef();
    intersect_case_point();
    normal_case_coef();
    normal_case_point();
}