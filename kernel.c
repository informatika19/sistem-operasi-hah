void handleInterrupt21(int AX, int BX, int CX, int DX);
void printString(char *string);
void readString(char *string);
void clear(char *buffer, int length);
void bootLogo();
void bootImage();

int mod(int a, int b);
int strlen(char *string);

int main()
{

    makeInterrupt21();
    // bootLogo();
    interrupt(0x10, 0x0012, 0, 0, 0);

    bootImage();
    interrupt(0x16, 0, 0, 0, 0);
    interrupt(0x10, 0x0003, 0, 0, 0);
    bootLogo();
    while (1)
    {
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
    while (!(AL == '\r'))
    {
        if (AL != 0x0)
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
        }
        else
        {
            //DISINI BEB <3
        }
        input = interrupt(0x16, 0, 0, 0, 0);
        AH = (char)(input >> 8);
        AL = (char)(input);
    }
    printString("\n\r");
    string[length] = 0;
    return;
}

void bootLogo(){                                                  
    printString("8 8888        8          .8.          8 8888        8\r\n");
    printString("8 8888        8         .888.         8 8888        8\r\n");
    printString("8 8888        8        :88888.        8 8888        8\r\n");
    printString("8 8888        8       . `88888.       8 8888        8\r\n");
    printString("8 8888        8      .8. `88888.      8 8888        8\r\n");
    printString("8 8888        8     .8`8. `88888.     8 8888        8\r\n");
    printString("8 8888888888888    .8' `8. `88888.    8 8888888888888\r\n");
    printString("8 8888        8   .8'   `8. `88888.   8 8888        8\r\n"); 
    printString("8 8888        8  .888888888. `88888.  8 8888        8\r\n");
    printString("8 8888        8 .8'       `8. `88888. 8 8888        8\r\n");                             
}

void bootImage() {
    int i;
    int j;
    int map[30][30];
    for (i = 0; i < 30; i++){
        for (j = 0; j < 30; j++){
            map[i][j] = 0x0;
        }
    }
    for (i = 0; i < 30; i++){
        map[i][16] = 0x9;
        map[11][3] = 0x1;
        map[12][3] = 0x2;
        map[13][3] = 0x3;
        map[14][3] = 0x4;
        map[15][3] = 0x5;
        map[16][3] = 0x6;
        map[17][3] = 0x7;
        map[18][3] = 0x8;
    }

    for (j = 3; j < 30; j++){
        map[1][j] = 0x1;
        map[8][j] = 0x2;
        map[11][j] = 0x3;
        map[18][j] = 0x4;
        map[21][j] = 0x1;
        map[28][j] = 0x2;
    }

    for (i = 0; i < 300; i++) {
        for (j = 0; j < 300; j++) {
            int is = i/10;
            int js = j/10;
            interrupt(0x10, 0x0C00 + map[is][js], 0, i, j);
        }

    }
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

