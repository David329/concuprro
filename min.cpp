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
    
#define MAXA    128
#define THREADS 16
#define LOCALS  8
    
using namespace std;
    
int main() {
    cl::Platform platform;
    voStmt(cl::Platform::get(&platform));
    
    vector<cl::Device> devices;
    voStmt(platform.getDevices(CL_DEVICE_TYPE_DEFAULT, &devices));
    cl::Device device = devices[0];
    
    ifstream fksrc("min.cl");
    if (!fksrc.is_open()) exit(3);
    
    string src((istreambuf_iterator<char>(fksrc)), istreambuf_iterator<char>());
    
    cl::Program::Sources sources;
    sources.push_back({src.c_str(), src.length()});
    
    cl::Context context({device});
    cl::Program program(context, sources);
    voStmt(program.build({device}));
    
    float a[MAXA];
    float mins[THREADS];
    
    for (int i = 0; i < MAXA; ++i) {
        a[i] = i + 100;
    }
    for (int i = 0; i < THREADS; ++i) {
        mins[i] = MAXA + 100;
    }
    a[46] = 10;
    
    cl::Buffer ba(context, CL_MEM_READ_WRITE, sizeof(float) * MAXA);
    cl::Buffer bm(context, CL_MEM_READ_WRITE, sizeof(float) * THREADS);
    
    cl::CommandQueue queue(context, device);
    
    voStmt(queue.enqueueWriteBuffer(ba, CL_TRUE, 0, sizeof(float) * MAXA, a));
    voStmt(queue.enqueueWriteBuffer(bm, CL_TRUE, 0, sizeof(float) * THREADS, mins));
    
    cl::Kernel minimum(program, "minimum");
    minimum.setArg(0, ba);
    minimum.setArg(1, bm);
    voStmt(queue.enqueueNDRangeKernel(minimum,
                                      cl::NullRange,
                                      cl::NDRange(MAXA),
                                      cl::NDRange(LOCALS)));
    
    voStmt(queue.enqueueReadBuffer(bm, CL_TRUE, 0, sizeof(float) * THREADS, mins));
    
    float min = mins[0];
    cout << "[ ";
    for (int i = 0; i < THREADS; ++i) {
        cout << mins[i] << " ";
        if (mins[i] < min) {
            min = mins[i];
        }
    }
    cout << "] -> " << min << endl;
    
    return 0;
}