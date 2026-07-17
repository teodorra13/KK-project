// 1. Obican mrtav store
int dead_store_simple()
{
    int x;

    x = 10;

    return 0;
}


// 2. Store koji se koristi
int live_store()
{
    int x;

    x = 10;

    return x;
}


// 3. Dva store-a bez load-a
// prvi treba da se ukloni
int overwrite_store()
{
    int x;

    x = 10;
    x = 20;

    return x;
}


// 4. Vise uzastopnih store-ova
// samo poslednji ostaje
int multiple_overwrite()
{
    int x;

    x = 1;
    x = 2;
    x = 3;

    return x;
}


// 5. Store pre call-a
// ne sme da se ukloni
// jer funkcija moze da cita x
void unknown_function();


int store_before_call()
{
    int x;

    x = 10;

    unknown_function();

    return 0;
}


// 6. Store posle call-a
// prvi store posle call-a ostaje
int store_after_call()
{
    int x;

    unknown_function();

    x = 10;

    return 0;
}


// 7. Store pre call-a i novi store posle call-a
// x=10 pre call-a treba da ostane
int overwrite_after_call()
{
    int x;

    x = 10;

    unknown_function();

    x = 20;

    return x;
}


// 8. Dva store-a posle call-a
// prvi posle call-a treba da se obrise
int two_store_after_call()
{
    int x;

    unknown_function();

    x = 10;
    x = 20;

    return x;
}


// 9. Vise promenljivih posle call-a
// proverava DefinedAfterCall za vise lokacija
int multiple_variables_after_call()
{
    int x;
    int y;

    unknown_function();

    x = 10;
    y = 20;

    x = 30;

    return y;
}


// 10. Load izmedju store-ova
// prvi store NE SME da se ukloni
int load_between_stores()
{
    int x;
    int y;

    unknown_function();

    x = 10;

    y = x;

    x = 20;

    return y;
}


// 11. Dva call-a
// proverava reset DefinedAfterCall
int multiple_calls()
{
    int x;

    x = 10;

    unknown_function();

    x = 20;

    unknown_function();

    x = 30;

    return 0;
}


// 12. Call koji se samo deklarise
// da LLVM ne optimizuje unapred
int declare_call_test()
{
    unknown_function();
    return 0;
}