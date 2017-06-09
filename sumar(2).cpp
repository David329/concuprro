#include <iostream>
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
    
#define MAXLEN 10
    
using namespace std;
    
int main() {
    cl::Platform platform;
    voStmt(cl::Platform::get(&platform));
    
    vector<cl::Device> devices;
    voStmt(platform.getDevices(CL_DEVICE_TYPE_DEFAULT, &devices));
    cl::Device device = devices[0];
    
    ifstream fksrc("sumar.cl");
    if (!fksrc.is_open()) exit(3);
    
    string src((istreambuf_iterator<char>(fksrc)), istreambuf_iterator<char>());
    
    cl::Program::Sources sources;
    sources.push_back({src.c_str(), src.length()});
    
    cl::Context context({device});
    cl::Program program(context, sources);
    voStmt(program.build({device}));
    
    int a[MAXLEN] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int b[MAXLEN] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
    int c[MAXLEN];
    
    cl::Buffer bA(context, CL_MEM_READ_WRITE, sizeof(int) * MAXLEN);
    cl::Buffer bB(context, CL_MEM_READ_WRITE, sizeof(int) * MAXLEN);
    cl::Buffer bC(context, CL_MEM_READ_WRITE, sizeof(int) * MAXLEN);
    
    cl::CommandQueue queue(context, device);
    
    voStmt(queue.enqueueWriteBuffer(bA, CL_TRUE, 0, sizeof(int) * MAXLEN, a));
    voStmt(queue.enqueueWriteBuffer(bB, CL_TRUE, 0, sizeof(int) * MAXLEN, b));
    
    cl::Kernel hola(program, "sumar");
    hola.setArg(0, bA);
    hola.setArg(1, bB);
    hola.setArg(2, bC);
    voStmt(queue.enqueueNDRangeKernel(hola, cl::NullRange, cl::NDRange(MAXLEN), cl::NullRange));
    
    voStmt(queue.enqueueReadBuffer(bC, CL_TRUE, 0, sizeof(int) * MAXLEN, c));
    
    cout << "[ ";
    for (int i = 0; i < MAXLEN; ++i) {
        cout << c[i] << " ";
    }
    cout << "]\n";
    
    return 0;
}