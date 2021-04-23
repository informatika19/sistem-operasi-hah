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

void clear(char *buffer, int length)
{
    int i;
    for (i = 0; i < length; i++)
    {
        *(buffer + i) = 0x0000;
    }
}
void splitString(char *buffer, char *first, char *second, char delimiter)
{
    int splitted;
    char *pointer;
    int firstLength;
    int secondLength;

    pointer = buffer;
    splitted = 0;

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

void splitToFolderAndFilename(char *path, char *filename, char *folder)
{
    char temp[16];
    int i;
    int pathLength;
    int tempIdx;
    int nameLength;
    int endFolderIndex;
    for (i = 0; i < 16; i++)
    {
        temp[i] = 0x00;
    }
    endFolderIndex = -1;
    pathLength = strlen(path);
    tempIdx = pathLength - 1;
    i = 0;
    while (tempIdx >= 0)
    {
        if (path[tempIdx] == '/')
        {
            endFolderIndex = tempIdx;
            break;
        }
        temp[i] = path[tempIdx];
        tempIdx -= 1;
    }
    if (endFolderIndex == -1)
    {
        endFolderIndex = 0;
        *filename = 0x00;
    }
    else
    {

        for (i = 0; i <= endFolderIndex; i++)
        {
            folder[i] = path[i];
        }
    }

    nameLength = strlen(temp);
    nameLength -= 1;
    i = 0;
    while (nameLength >= 0)
    {
        filename[i] = temp[nameLength];
        i += 1;
        nameLength -= 1;
    }
}

int pathIndex(char *file, char parent, char *path)
{
    char P = parent;
    char pos = 0x00;
    char idx = 0x00;

    if (path[0] == '/')
    {
        pos++;
        P = 0xFF;
    }

    while (path[pos] != 0x00)
    {
        if (strbcmp(path + pos, 1, "/"))
        {
            if (strbcmp(path + pos, 2, "//"))
            {
                return -1;
            }
            pos++;
        }
        else if (strbcmp(path + pos, 3, "../"))
        {
            if (P == 0xFF)
            {
                return -1;
            }
            P = file[P * 16];
            pos += 3;
        }
        else if (strbcmp(path + pos, 2, "./"))
        {
            pos += 2;
        }
        else
        {
            if (P != file[idx * 16])
            {
                idx++;
            }
            else if (file[idx * 16 + 2] == 0)
            {
                idx++;
            }
            else if (strswith(path + pos, file + (idx * 16 + 2), 14))
            {

                pos += strlen(file + (idx * 16 + 2));
                P = idx;
                // return idx;

                idx = 0x00;
            }
            else
            {
                idx++;
            }
        }

        if (idx > 0x3f)
        {
            return -1;
        }
    }

    if (P != 0xFF && file[P * 16 + 1] != 0XFF)
    {
        return P;
    }
    else
    {
        // File not found atau file tersebut adalah folder
        return -1;
    }
}

int folderIndex(char *file, char parent, char *path)
{
    char P = parent;
    char pos = 0x00;
    char idx = 0x00;

    if (path[0] == '/')
    {
        pos++;
        P = 0xFF;
    }

    while (path[pos] != 0x00)
    {
        if (strbcmp(path + pos, 1, "/"))
        {
            if (strbcmp(path + pos, 2, "//"))
            {
                return -1;
            }
            pos++;
        }
        else if (strbcmp(path + pos, 3, "../"))
        {
            // printString("Masuk sini \r\n");
            if (P == 0xFF)
            {
                return -1;
            }
            P = file[P * 16];
            pos += 3;
        }
        else if (strbcmp(path + pos, 2, "./"))
        {
            pos += 2;
        }
        else
        {
            if (P != file[idx * 16])
            {
                idx++;
            }
            else if (file[idx * 16 + 2] == 0)
            {
                idx++;
            }
            else if (strswith(path + pos, file + (idx * 16 + 2), 14))
            {

                pos += strlen(file + (idx * 16 + 2));
                P = idx;
                // return idx;

                idx = 0x00;
            }
            else
            {
                idx++;
            }
        }

        if (idx > 0x3f)
        {
            return -1;
        }
    }

    if (file[P * 16 + 1] == 0XFF || P == 0XFF)
    {
        return P;
    }
    else
    {
        // File not found atau file tersebut adalah folder
        return -1;
    }
}