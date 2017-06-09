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
    
#define MAXLEN 64
    
using namespace std;
    
int main() {
    cl::Platform platform;
    voStmt(cl::Platform::get(&platform));
    
    vector<cl::Device> devices;
    voStmt(platform.getDevices(CL_DEVICE_TYPE_DEFAULT, &devices));
    cl::Device device = devices[0];
    
    ifstream fksrc("ids.cl");
    if (!fksrc.is_open()) exit(3);
    
    string src((istreambuf_iterator<char>(fksrc)), istreambuf_iterator<char>());
    
    cl::Program::Sources sources;
    sources.push_back({src.c_str(), src.length()});
    
    cl::Context context({device});
    cl::Program program(context, sources);
    voStmt(program.build({device}));
    
    int globIds[MAXLEN];
    int grouIds[MAXLEN];
    int localIds[MAXLEN];
    
    cl::Buffer bGlobIds(context, CL_MEM_READ_WRITE, sizeof(int) * MAXLEN);
    cl::Buffer bGrouIds(context, CL_MEM_READ_WRITE, sizeof(int) * MAXLEN);
    cl::Buffer bLocalIds(context, CL_MEM_READ_WRITE, sizeof(int) * MAXLEN);
    
    cl::CommandQueue queue(context, device);
        
    cl::Kernel getids(program, "getids");
    getids.setArg(0, bGlobIds);
    getids.setArg(1, bGrouIds);
    getids.setArg(2, bLocalIds);
    voStmt(queue.enqueueNDRangeKernel(getids, cl::NullRange, cl::NDRange(MAXLEN), cl::NullRange));
    
    voStmt(queue.enqueueReadBuffer(bGlobIds, CL_TRUE, 0, sizeof(int) * MAXLEN, globIds));
    voStmt(queue.enqueueReadBuffer(bGrouIds, CL_TRUE, 0, sizeof(int) * MAXLEN, grouIds));
    voStmt(queue.enqueueReadBuffer(bLocalIds, CL_TRUE, 0, sizeof(int) * MAXLEN, localIds));
    
    int step = MAXLEN;
    for (int i = 0; i < MAXLEN; ++i) {
        cout << globIds[i] << ", " << grouIds[i] <<  ", " << localIds[i] << "\n";
    }
    
    return 0;
}