// 1. Jednostavna mrtva operacija
int dead_add(int a, int b) {
    int x = a + b;   // treba da se obrise
    return 0;
}


// 2. Koriscen rezultat - ne sme da obrise
int used_add(int a, int b) {
    int x = a + b;   // mora da ostane
    return x;
}


// 3. Lanac mrtvih instrukcija
int dead_chain(int a, int b) {
    int x = a + b;
    int y = x * 10;
    int z = y - 5;

    return 0;
}


// 4. Mrtav icmp
int dead_compare(int a, int b) {
    int result = (a < b);   // treba da se obrise
    return 10;
}


// 5. Koriscen icmp
int used_compare(int a, int b) {
    int result = (a < b);   // mora da ostane

    if (result)
        return 1;

    return 0;
}


// 6. Vise operacija
int mixed(int a, int b) {
    int x = a + b;       // mrtvo
    int y = a - b;       // mrtvo
    int z = a * b;       // koristi se

    return z;
}


// 7. Store ne sme da se obrise
int store_test() {
    int x;

    x = 5;

    return x;
}


// 8. Više basic blokova
int multiple_blocks(int a) {

    int x = a + 10;  // mrtvo

    if (a > 0) {
        int y = a * 2;
        return y;
    }

    return 0;
}