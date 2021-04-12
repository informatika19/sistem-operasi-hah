// Input output
void printString(char *string)
{
    interrupt(0x21, 0x00, string, 0, 0);
}

void readString(char *string)
{
    interrupt(0x21, 0x01, string, 0, 0);
}

// String utility
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

    // printString("Comparing : \r\n");
    // printString(first);
    // printString(" ");
    // printString(second);
    // printString("\r\n");
    for (i = 0; i < length; i++)
    {
        if (second[i] == 0x0)
        {
            // printString("RETURNING TRUE\r\n");
            return 1;
        }
        else if (first[i] != second[i])
        {
            printString(".");
            printString("\b \b");

            return 0;
        }
    }
    // printString("RETURNING TRUE\r\n");

    return 1;
}

//string bounded compare
int strbcmp(char *buffer, int length, char *string)
{

    char i;
    char first[8192];
    i = 0x00;

    for (i = 0; i < length; i++)
    {
        first[i] = buffer[i];
    }
    // first[++i] = 0x00;
    // printString("Comparing : \r\n");
    // printInteger(strlen(first));
    // printString("\r\n");
    // printInteger(strlen(string));
    // printString("\r\n");
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

void strcpy(char *src, char *dst)
{
    char *temp = src;
    int index;
    index = 0;
    while (*temp != 0x0)
    {
        dst[index] = *temp;
        temp++;
        index++;
    }
}
