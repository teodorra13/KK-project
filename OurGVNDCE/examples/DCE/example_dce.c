int dead_add(int a, int b) {
    int x = a + b; 
    return 0;
}


int used_add(int a, int b) {
    int x = a + b;   
    return x;
}


int dead_chain(int a, int b) {
    int x = a + b;
    int y = x * 10;
    int z = y - 5;

    return 0;
}



int dead_compare(int a, int b) {
    int result = (a < b);   
    return 10;
}



int used_compare(int a, int b) {
    int result = (a < b);  

    if (result)
        return 1;

    return 0;
}



int mixed(int a, int b) {
    int x = a + b;      
    int y = a - b;      
    int z = a * b;       

    return z;
}


int store_test() {
    int x;

    x = 5;

    return x;
}


int multiple_blocks(int a) {

    int x = a + 10;  // mrtvo

    if (a > 0) {
        int y = a * 2;
        return y;
    }

    return 0;
}
