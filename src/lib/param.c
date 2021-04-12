void getParam(char *currentDirectory, char *param)
{
    char buffer[512];
    int i;
    char *temp;
    interrupt(0x21, 0x02, buffer, 0x404, 0);

    *currentDirectory = buffer[0];
    temp = buffer + 1;
    i = 0;
    while (temp != 0x00)
    {
        param[i] = *temp;
        temp++;
        i++;
    }
}

void splitString(char *buffer, char *first, char *second, char delimiter)
{
    int splitted = 0;
    char *pointer = buffer;
    int firstLength;
    int secondLength;

    firstLength = 0;
    secondLength = 0;
    while (*pointer != 0x00)
    {
        if (*pointer == delimiter)
        {
            if (splitted == 0)
            {
                splitted = 1;
            }
        }
        else
        {
            if (splitted == 1)
            {
                second[secondLength] = *pointer;
                secondLength++;
            }
            else
            {
                first[firstLength] = *pointer;
                firstLength++;
            }
        }

        pointer++;
    }
    second[secondLength] = 0x0;
    first[firstLength] = 0x0;
}