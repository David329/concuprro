__kernel void sumar(__global const int* a,
                    __global const int* b,
                    __global int* c) {
    int id = get_global_id(1) * get_global_size(0) + get_global_id(0);
c[0]= a[get_global_size(0)*get_global_size(1)-1];
}
