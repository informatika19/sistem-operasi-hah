int mod(int a, int b);
int div(int a, int b);
int strlen(char *string);

int mod(int a, int b)
{
    return (a - b * (a / b));
}

int div(int a, int b)
{
    int is_negative = 0;
    if (a < 0 && b < 0){
        a = a * (-1);
        b = b * (-1);
    }
    else if ((a < 0) && (b > 0)){
        a = a * (-1);
        is_negative = 1;
    }
    else if ((a > 0) && (b < 0)){
        b = b * (-1);
        is_negative = 1;
    }

    if (!is_negative){
        if (b > a){
            return 0;
        }
        else{
            return (1 + div(a - b, b));
        }
    }
    else if (is_negative){
        if (b > a){
            return (-1);
        }
        else{
            return ((-1) * (2 + div(a - b, b)));
        }
    }    
}

int strlen(char *string)
{
    char *a = string;
    int ans = 0;
    while (*a != 0x0)
    {
        a++;
        ans += 1;
    }
    return ans;
}