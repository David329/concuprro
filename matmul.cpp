#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#ifdef __APPLE__
    #include <OpenCL/cl.hpp>
#else
    #include <CL/cl.hpp>
#endif
    
#include "utilCL.h"
    
#define A_ROWS 3
#define A_COLS 4
#define B_ROWS 4
#define B_COLS 5
    
using namespace std;
    
int main() {
    cl::Platform platform;
    voStmt(cl::Platform::get(&platform));
    
    vector<cl::Device> devices;
    voStmt(platform.getDevices(CL_DEVICE_TYPE_DEFAULT, &devices));
    cl::Device device = devices[0];
    
    ifstream fksrc("matmul.cl");
    if (!fksrc.is_open()) exit(3);
    
    string src((istreambuf_iterator<char>(fksrc)), istreambuf_iterator<char>());
    
    cl::Program::Sources sources;
    sources.push_back({src.c_str(), src.length()});
    
    cl::Context context({device});
    cl::Program program(context, sources);
    voStmt(program.build({device}));
    
    float a[A_ROWS][A_COLS] = {
        {2,2,2,2},
        {2,2,2,2},
        {2,2,2,2}};
    float b[B_ROWS][B_COLS] = {
        {3,3,3,3,3},
        {3,3,3,3,3},
        {3,3,3,3,3},
        {3,3,3,3,3}};
    float c[A_ROWS][B_COLS];
    
    cl::Buffer bA(context, CL_MEM_READ_WRITE, sizeof(float) * A_ROWS*A_COLS);
    cl::Buffer bB(context, CL_MEM_READ_WRITE, sizeof(float) * B_ROWS*B_COLS);
    cl::Buffer bC(context, CL_MEM_READ_WRITE, sizeof(float) * A_ROWS*B_COLS);
    
    cl::CommandQueue queue(context, device);
    
    voStmt(queue.enqueueWriteBuffer(bA, CL_TRUE, 0, sizeof(float) * A_ROWS*A_COLS, a));
    voStmt(queue.enqueueWriteBuffer(bB, CL_TRUE, 0, sizeof(float) * B_ROWS*B_COLS, b));
    
    cl::Kernel matmul(program, "matmul");
    int narg = 0;
    matmul.setArg(narg++, bA);
    matmul.setArg(narg++, bB);
    matmul.setArg(narg++, bC);
    matmul.setArg(narg++, A_ROWS);
    matmul.setArg(narg++, B_COLS);
    matmul.setArg(narg++, A_COLS);
    voStmt(queue.enqueueNDRangeKernel(matmul,
                                      cl::NullRange,
                                      cl::NDRange(A_ROWS, B_COLS),
                                      cl::NullRange));
    
    voStmt(queue.enqueueReadBuffer(bC, CL_TRUE, 0, sizeof(float) * A_ROWS*B_COLS, c));
    
    for (int i = 0; i < A_ROWS; ++i) {
        for (int j = 0; j < B_COLS; ++j) {
            cout.precision(5);
            cout << setw(6) << c[i][j];
        }
        cout << endl;
    }
    
    return 0;
}