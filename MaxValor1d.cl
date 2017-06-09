__global int Max=0;
__kernel void sumar(__global const int* a,
                    __global const int* b,
                    __global int* c) {
    int id = get_global_id(0);
    if(a[id]>Max){Max=a[id];c[0]=Max;}
}