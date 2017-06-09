__kernel void hola(__global char* a) {
    a[0] = 'H';
    a[1] = 'o';
    a[2] = 'l';
    a[3] = 'a';
    a[4] = ',';
    a[5] = ' ';
    a[6] = 'M';
    a[7] = 'u';
    a[8] = 'n';
    a[9] = 'd';
    a[10] = 'o';
    a[11] = '!';
    a[12] = '\0';
}
