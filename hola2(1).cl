char msg[13] = "Hola, Mundo!";

__kernel void hola(__global char* a) {
    int id = get_global_id(0);
    a[id] = msg[id];
}