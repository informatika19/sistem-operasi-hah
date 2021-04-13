void addParameter(char *param, char currentDirectory);
int main()
{
    int flag;
    char test[512];
    addParameter("", 0xFF);
    printString("BABA");
    interrupt(0x21, 0xFF06, "ls", 0x3000, &flag);
    printString("BABA");
}

void addParameter(char *param, char currentDirectory)
{
    char otherBuffer[512];
    int i;
    char *temp;

    temp = param;
    clear(otherBuffer, 512);
    otherBuffer[0] = currentDirectory;
    i = 1;
    // printString(param);
    while (*temp != 0x00 && i < 512)
    {
        otherBuffer[i] = *temp;
        temp++;
        i++;
    }
    if (i >= 512)
    {
        otherBuffer[511] = 0x0;
    }
    interrupt(0x21, 0x03, otherBuffer, 0x404, 0);
}