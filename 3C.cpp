#include <iostream> 
#include <vector> 
#include <string> 
#include <cmath>
#include <float.h> 
#include <algorithm>

using std::vector;
using std::cin;
using std::cout;

const double PI = 3.14159265359;
 
struct point {
    double x;
    double y;

    point() {
        x = 0;
        y = 0;
    }
    point(double a, double b) {
        x = a;
        y = b;
    }
};

struct vec {
    double phi;
    double x;
    double y;
    vec(point a, point b) {
        x = b.x - a.x;
        y = b.y - a.y;
        phi = atan2(y, x);
        if(phi < 0) {
            phi += 2 * PI;
        }
    }
};

bool pointIsLess(point a, point b) {
    if(a.y < b.y) {
        return true;
    } else if(a.y == b.y && a.x < b.x) {
        return true;
    } else {
        return false;
    }
}

point vecSum(point a, point b) {
    point res;
    res.x = a.x + b.x;
    res.y = a.y + b.y;
    return res;
}


double roundTo(double a, int power) {
    return round(a * pow(10, power)) / pow(10, power);
}

bool pointIsInsidePolygon(vector<point> &poly, point p) {
    int n = poly.size();
    int j = n-1;
    bool c = false;
    for(int i = 0; i < n; ++i) {
        point left = poly[j];
        point right = poly[i];
        if((left.y < p.y) != (right.y < p.y) && 
                (p.x < (p.y - left.y) * (left.x - right.x) 
                 / (left.y - right.y) + left.x)) {
            c = !c;
        }
        j = i;
    }
    return c;
}

vector<point> readPolygon(int count, bool inverted) {
    vector<point> polygon;
    int sign = inverted ? -1 : 1;
    double vert_x, vert_y;
    for(int i = 0; i < count; ++i) {
        cin >> vert_x >> vert_y;
        polygon.push_back(point(vert_x * sign, vert_y * sign));
    }
    return polygon;
}

vector<point> rearrangePolygonVector(vector<point> poly) {
    int count = poly.size();
    reverse(poly.begin(), poly.end());
    point min_point = point(DBL_MAX, DBL_MAX);
    int min_index = -1;
    for(int i = 0; i < count; ++i) {
        if(pointIsLess(poly[i], min_point)) {
            min_point = poly[i];
            min_index = i;
        }
    }
    vector<point> rearranged_poly;
    for(int i = min_index; i < count; ++i) {
        rearranged_poly.push_back(poly[i]);
    }
    for(int i = 0; i < min_index; ++i) { 
        rearranged_poly.push_back(poly[i]);
    }
    return rearranged_poly;
}

int main() {
    int n, m;
    cin >> n;
    vector<point> poly1, poly2;
    poly1 = rearrangePolygonVector(readPolygon(n, false));
    cin >> m;
    poly2 = rearrangePolygonVector(readPolygon(m, true));
    

    poly1.push_back(poly1[0]);
    poly1.push_back(poly1[1]);
    poly2.push_back(poly2[0]);
    poly2.push_back(poly2[1]);
    
    int i = 0;
    int j = 0;
    vector<point> minkowsky_sum;

    while(i <= n && j <= m) {
        minkowsky_sum.push_back(vecSum(poly1[i], poly2[j]));
        vec v1(poly1[i], poly1[i + 1]);
        vec v2(poly2[j], poly2[j + 1]);
        v1.phi = roundTo(v1.phi, 8);
        v2.phi = roundTo(v2.phi, 8);

        if(v1.phi < v2.phi) {
            i++;
        } else if (v1.phi > v2.phi) {
            j++;
        } else {
            i++;
            j++;
        }
    }

    point start(0, 0);
    if(pointIsInsidePolygon(minkowsky_sum, start)) {
        cout << "YES";
    } else {
        cout << "NO";
    }

    return 0;
}
