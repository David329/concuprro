#include <iostream>
#include <iomanip>

using namespace std;

struct mat {
    float** M;
    int n, m;

    mat(int n, int m, float ini = 0.f) : n(n), m(m) {
        M = new float*[n];
        for (int i = 0; i < n; ++i) {
            M[i] = new float[m];
            for (int j = 0; j < m; ++j) {
                M[i][j] = ini;
            }
        }
    }
    ~mat() {
        for (int i = 0; i < n; ++i) {
            delete[] M[i];
        }
        delete[] M;
    }
};

mat* matmul(mat* a, mat* b) {
    mat* r = new mat(a->n, b->m);
    for (int i = 0; i < r->n; ++i) {
        for (int j = 0; j < r->m; ++j) {
            float temp = 0;
            for (int k = 0; k < a->m; ++k) {
                temp += a->M[i][k] * b->M[k][j];
            }
            r->M[i][j] = temp;
        }
    }
    return r;
}

ostream& operator << (ostream& os, mat* m) {
    for (int i = 0; i < m->n; ++i) {
        for (int j = 0; j < m->m; ++j) {
            os.precision(5);
            os << setw(6) << m->M[i][j];
        }
        os << endl;
    }
    return os;
}

int main() {
    mat* a = new mat(3, 4, 2.f);
    mat* b = new mat(4, 5, 3.f);
    mat* r = matmul(a, b);
    
    cout << a << endl;
    cout << b << endl;
    cout << r;
    
    delete a;
    delete b;
    delete r;
    return 0;
}