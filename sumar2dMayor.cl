__kernel void sumar(__global const int* a,
                    __global const int* b,
                    __global int* c) {
    int id = get_global_id(1) * get_global_size(0) + get_global_id(0);
if(a[id]>b[id])c[id] = a[id];
else c[id] = b[id];
}