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
    
using namespace std;
    
int main() {
    vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    if (platforms.size() == 0) exit(1);
    
    vector<cl::Device> devices;
    platforms[0].getDevices(CL_DEVICE_TYPE_ALL, &devices);
    if (devices.size() == 0) exit(2);
    
    ifstream fksrc("hola.cl");
    if (!fksrc.is_open()) exit(3);
    
    string src((istreambuf_iterator<char>(fksrc)), istreambuf_iterator<char>());
    
    cl::Program::Sources sources;
    sources.push_back({src.c_str(), src.length()});
    
    cl::Context context({devices[0]});
    cl::Program program(context, sources);
    if (program.build({devices[0]}) != CL_SUCCESS) exit(4);
    
    char msg[13];
    cl::Buffer bmsg(context, CL_MEM_READ_WRITE, sizeof(msg));
    
    cl::CommandQueue queue(context, devices[0]);
    
    cl::Kernel hola(program, "hola");
    hola.setArg(0, bmsg);
    queue.enqueueNDRangeKernel(hola, cl::NullRange, cl::NDRange(1), cl::NullRange);
    
    queue.enqueueReadBuffer(bmsg, CL_TRUE, 0, sizeof(msg), msg);
    
    cout << msg << endl;
    
    return 0;
}