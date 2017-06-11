__kernel void matmul(__global float* a,
                     __global float* b,
                     __global float* r,
                     int rows,
                     int cols,
                     int a_cols) {
    int i = get_global_id(0);
    int j = get_global_id(1);
    float temp = 0;
    for (int k = 0; k < a_cols; ++k) {
        temp += a[i * a_cols + k] * b[k * cols + j];
    }
    r[i * cols + j] = temp;
}
