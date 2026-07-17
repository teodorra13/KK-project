
int dead_store_simple()
{
    int x;

    x = 10;

    return 0;
}


int live_store()
{
    int x;

    x = 10;

    return x;
}


int overwrite_store()
{
    int x;

    x = 10;
    x = 20;

    return x;
}


int multiple_overwrite()
{
    int x;

    x = 1;
    x = 2;
    x = 3;

    return x;
}


void unknown_function();


int store_before_call()
{
    int x;

    x = 10;

    unknown_function();

    return 0;
}


int store_after_call()
{
    int x;

    unknown_function();

    x = 10;

    return 0;
}


int overwrite_after_call()
{
    int x;

    x = 10;

    unknown_function();

    x = 20;

    return x;
}


int two_store_after_call()
{
    int x;

    unknown_function();

    x = 10;
    x = 20;

    return x;
}

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


int declare_call_test()
{
    unknown_function();
    return 0;
}
