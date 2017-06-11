__kernel void minimum(__global float* a, __global float* mins) {
    int gid = get_global_id(0);
    int i = gid % 16;
    if (a[gid] < mins[i]) {
        mins[i] = a[gid];
    }
}
