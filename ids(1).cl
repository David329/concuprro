__kernel void getids(__global int* globalids,
                    __global int* groupids,
                    __global int* localids) {
    int id = get_global_id(0);
    globalids[id] = id;
    groupids[id] = get_group_id(0);
    localids[id] = get_local_id(0);
}