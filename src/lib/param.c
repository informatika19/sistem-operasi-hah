
void getParam(char *currentDirectory, char *param)
{
    char buffer[512];
    int i;
    char *temp;
    interrupt(0x21, 0x02, buffer, 0x404, 0);
    // printString(buffer);
    *currentDirectory = buffer[0];
    temp = buffer;
    temp++;
    i = 0;
    while (*temp != 0x00)
    {
        param[i] = *temp;
        temp++;
        i++;
    }
    // printString(buffer);

    return;
}