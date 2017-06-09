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
    
#define MAXLEN 3
    
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
    
    int a[MAXLEN][MAXLEN] = { {12, 22, 53}, {224, 53, 46}, {17, 8, 59}};
    int b[MAXLEN][MAXLEN] = { {14, 12, 31}, {334, 55, 446}, {7, 48, 9}};
    int c[MAXLEN][MAXLEN];
    
    cl::Buffer bA(context, CL_MEM_READ_WRITE, sizeof(int) * MAXLEN* MAXLEN);
    cl::Buffer bB(context, CL_MEM_READ_WRITE, sizeof(int) * MAXLEN* MAXLEN);
    cl::Buffer bC(context, CL_MEM_READ_WRITE, sizeof(int) * MAXLEN* MAXLEN);
    
    cl::CommandQueue queue(context, device);
    
    voStmt(queue.enqueueWriteBuffer(bA, CL_TRUE, 0, sizeof(int) * MAXLEN* MAXLEN, a));
    voStmt(queue.enqueueWriteBuffer(bB, CL_TRUE, 0, sizeof(int) * MAXLEN* MAXLEN, b));
    
    cl::Kernel hola(program, "sumar");
    hola.setArg(0, bA);
    hola.setArg(1, bB);
    hola.setArg(2, bC);
    voStmt(queue.enqueueNDRangeKernel(hola, cl::NullRange, cl::NDRange(MAXLEN,MAXLEN), cl::NullRange));
    
    voStmt(queue.enqueueReadBuffer(bC, CL_TRUE, 0, sizeof(int) * MAXLEN*MAXLEN, c));
    
    cout << "[ ";
    for (int i = 0; i < MAXLEN; ++i) {
        for(int j=0;j<MAXLEN;++j){
        cout << c[i][j] << " ";
        }
    }
    cout << "]\n";
    
    return 0;
}