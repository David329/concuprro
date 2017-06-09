#include <iostream>
#include <iomanip>
#include <vector>

#ifdef __APPLE__
    #include <OpenCL/cl.hpp>
#else
    #include <CL/cl.hpp>
#endif
    
    
using namespace std;

int main() {
    vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    cout << "Número de plataformas: " << platforms.size() << endl;
    
    int idPlat = 0;
    for (cl::Platform p : platforms) {
        cout << "Plataforma " << idPlat++ << endl;
        cout << setw(15) << "Perfil: " << p.getInfo<CL_PLATFORM_PROFILE>() << endl;
        cout << setw(15) << "Version: " << p.getInfo<CL_PLATFORM_VERSION>() << endl;
        cout << setw(15) << "Nombre: " << p.getInfo<CL_PLATFORM_NAME>() << endl;
        cout << setw(15) << "Fabricante: " << p.getInfo<CL_PLATFORM_VENDOR>() << endl;
        cout << setw(15) << "Extensiones: " << p.getInfo<CL_PLATFORM_EXTENSIONS>() << endl;
        
        vector<cl::Device> devices;
        p.getDevices(CL_DEVICE_TYPE_ALL, &devices);
        
        int idDev = 0;
        for (cl::Device d : devices) {
            cout << setw(15) << "Dispositivo: " << idDev++ << endl;
            cout << setw(18) << "Memoria " << d.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() << endl;
            cout << setw(18) << "Tipo mem: " << d.getInfo<CL_DEVICE_LOCAL_MEM_TYPE>() << endl;
            cout << setw(18) << "Frecuencia: " << d.getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>() << endl;
            cout << setw(18) << "Nombre: " << d.getInfo<CL_DEVICE_NAME>() << endl;
            cout << setw(18) << "Plataforma: " << d.getInfo<CL_DEVICE_PLATFORM>() << endl;
            cout << setw(18) << "Perfil: " << d.getInfo<CL_DEVICE_PROFILE>() << endl;
            cout << setw(18) << "Tipo: " << d.getInfo<CL_DEVICE_TYPE>() << endl;
            cout << setw(18) << "Fab. Id: " << d.getInfo<CL_DEVICE_VENDOR_ID>() << endl;
            cout << setw(18) << "Fabricante: " << d.getInfo<CL_DEVICE_VENDOR>() << endl;
            cout << setw(18) << "Version: " << d.getInfo<CL_DEVICE_VERSION>() << endl;
            cout << setw(18) << "Ver. Drvr: " << d.getInfo<CL_DRIVER_VERSION>() << endl;       
        }
    }
    
    return 0;
}