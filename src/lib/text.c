// Input output
void printString(char *string)
{
    char *p = string;
    while (*p != 0x0)
    {
        interrupt(0x10, 0x0e00 + *p, 0x000A, 0, 0);
        p++;
    }
    return;
}

void readString(char *string)
{
    int input = interrupt(0x16, 0, 0, 0, 0);
    char AH = (char)(input >> 8);
    char AL = (char)(input & 0x00FF);
    int length = 0;

    while (!(AL == '\r'))
    {
        if (AH == 0x48 && AH == 0x50)
        {
            while (length > 0)
            {
                printString("\b");
                string[--length] = 0x00;
            }
            string[0] = 0x00;
            string[1] = AH;
            return;
        }
        if (AL != 0x0)
        {

            if (AL == '\b' && length > 0)
            {
                interrupt(0x10, 0xe00 + '\b', 0xF, 0, 0);
                interrupt(0x10, 0xe00 + ' ', 0xF, 0, 0);
                interrupt(0x10, 0xe00 + '\b', 0xF, 0, 0);

                string[--length] = 0x00;
            }
            else
            {
                printString(&AL);
                string[length++] = AL;
            }
        }
        input = interrupt(0x16, 0, 0, 0, 0);
        AH = (char)(input >> 8);
        AL = (char)(input);
    }
    printString("\n\r");
    string[length] = 0;
    return;
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
    clear(first, 8192);
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


void strcpy (char * src, char * dst)
{
    char * temp = src;
    int index;
    index = 0;
    while (*temp != 0x0)
    {
        dst[index] = *temp;
        temp ++;
        index ++;
    }
}

