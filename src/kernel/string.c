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

int strcmp(char *first, char *second)
{
    if (strlen(first) != strlen(second))
    {
        return 0;
    }
    else
    {
        int idx = 0;
        char *temp = first;
        while (*temp != 0x00)
        {
            if (first[idx] != second[idx])
            {
                return 0;
            }
            temp++;
            idx++;
        }
        return 1;
    }
}
char strswith(char *first, char *second, int length)
{
    int i;

    for (i = 0; i < length; i++)
    {
        if (second[i] == 0)
        {
            return 1;
        }
        if (first[i] != second[i])
        {
            return 0;
        }
    }

    return 1;
}

//string bounded compare
int strbcmp(char *buffer, int length, char *string)
{
    char first[length + 1];
    char i = 0x00;
    for (i = 0; i < length; i++)
    {
        first[i] = buffer[i];
    }
    first[++i] = 0x00;
    return strcmp(first, string);
}

//string sanitize for avoiding buffer overflow
void strsntz(char *buffer, int maxlength)
{
    int i, isOk;
    isOk = 0;
    i = maxlength - 1;
    while (i >= 0)
    {
        if (buffer[i] == 0x0)
        {
            isOk = 1;
            return;
        }
        i--;
    }
    if (isOk == 0)
    {
        buffer[maxlength - 1] = 0x00;
    }
    return;
}
