void handleInterrupt21(int AX, int BX, int CX, int DX);
void printString(char *string);
void readString(char *string);
void clear(char *buffer, int length);

int mod(int a, int b);
int strlen(char *string);

int main()
{

    makeInterrupt21();
    
    while (1) {
        char a[100];
        readString(a);
    }
}

void clear(char *buffer, int length)
{
    int i;
    for (i = 0; i < length; i++)
    {
        *(buffer + i) = 0x0000;
    }
}

void handleInterrupt21(int AX, int BX, int CX, int DX)
{
    char AL, AH;
    AL = (char)(AX);
    AH = (char)(AX >> 8);
    switch (AL)
    {
    case 0x0:
        printString(BX);
        break;
    case 0x1:
        readString(BX);
        break;
    default:
        printString("Invalid interrupt");
    }
}

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
    char AL = (char)(input);
    int length = 0;
    while (!(AL == '\r' || AH == 0x48 || AH == 0x50))
    {
        if (AL == '\b' && length > 0)
        {
            printString("\b \b");
            string[length--] = 0x00;
        }
        else
        {
            printString(&AL);
            string[length++] = AL;
        }
        input = interrupt(0x16, 0, 0, 0, 0);
        AH = (char)(input >> 8);
        AL = (char)(input);
    }
    printString("\n\r");
    string[length] = 0;
    return;
}

int mod(int a, int b)
{
    return (a - b * (a / b));
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