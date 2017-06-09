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
    
using namespace std;
    
int main() {
    cl::Platform platform;
    voStmt(cl::Platform::get(&platform));
    
    vector<cl::Device> devices;
    voStmt(platform.getDevices(CL_DEVICE_TYPE_DEFAULT, &devices));
    cl::Device device = devices[0];
    
    ifstream fksrc("hola2.cl");
    if (!fksrc.is_open()) exit(3);
    
    string src((istreambuf_iterator<char>(fksrc)), istreambuf_iterator<char>());
    
    cl::Program::Sources sources;
    sources.push_back({src.c_str(), src.length()});
    
    cl::Context context({device});
    cl::Program program(context, sources);
    voStmt(program.build({device}));
    
    char msg[13];
    cl::Buffer bmsg(context, CL_MEM_READ_WRITE, sizeof(msg));
    
    cl::CommandQueue queue(context, device);
    
    cl::Kernel hola(program, "hola");
    hola.setArg(0, bmsg);
    voStmt(queue.enqueueNDRangeKernel(hola, cl::NullRange, cl::NDRange(sizeof(msg) + 1), cl::NullRange));
    
    voStmt(queue.enqueueReadBuffer(bmsg, CL_TRUE, 0, sizeof(msg), msg));
    
    cout << msg << endl;
    
    return 0;
}