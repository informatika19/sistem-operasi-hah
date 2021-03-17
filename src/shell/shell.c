int pathIndex(char parent, char *path);
char strswith(char *first, char *second, int length);
int strcmp(char *first, char *second);
int strbcmp(char *buffer, int length, char *string);
void printString(char *string);
void printCurrentDirectory(char currentDirectory);
void splitString(char *buffer, char *first, char *second, char delimiter);
void strcpy (char * src, char * dst);
void clear(char *buffer, int length);
int strlen(char *string);
void printInteger(int n);
int mod(int a, int b);
int div(int a, int b);

int main()
{
    char command[256];
    char program[256];
    char parameter[256];
    char file[1024];
    char fileBuffer[8192];
    char commandHistory[4][256];
    char currentDirectory = 0xFF;

    int i;
    int indexToMove;
    int flag;
    int historyPointer;
    int historyCount = 0;
    int arrowPressed = 0;
    historyPointer = -1;
    
    clear(commandHistory, 256 * 4);
    
    
    while (1)
    {
        clear(command, 256);
        if (arrowPressed == 0)
        {
            printString("\r\n");
            printCurrentDirectory(currentDirectory);
        }
        interrupt(0x21, 0x01, command, 0, 0);
        if (command[0] == 0x0)
        {
            if (command[1] == 0x48) // Key up
            {
                if (historyPointer < historyCount - 1) // Masih bisa up
                {
                    historyPointer ++;
                    printString(commandHistory + historyPointer * 256);
                }
            }
            else if (command[1] == 0x50) // Key down
            {
                if (historyPointer >= 0) // Masih bisa down
                {
                    historyPointer --;
                    if (historyPointer != -1)
                    {
                        printString(command + historyPointer * 256);
                    }
                }
                
            }
            arrowPressed = 1;
        }
        else 
        {
            if (strbcmp(command, 2, "cd"))
            {
                splitString(command, program, parameter, ' ');
                
                indexToMove = pathIndex(currentDirectory, parameter);
                if (indexToMove == -1)
                {
                    printString("No such folder");
                }
                else
                {
                    currentDirectory = indexToMove;
                }
            }
            else if (strbcmp(command, 3, "ls"))
            {
                printString("\r\n");
                interrupt(0x21, 0x02, file, 0x101, 0);
                interrupt(0x21, 0x02, file + 512, 0x102, 0);
                for (i = 0; i < 64; i++)
                {
                    if (file[i * 16] == currentDirectory)
                    {
                        printString(file + i * 16 + 2);
                        printString("\n\r");
                    }
                }

            }
            else if (strbcmp(command, 3, "cat"))
            {
                splitString(command, program, parameter, ' ');
                clear(fileBuffer, 8192);
                // printString(parameter);
                // printString("\r\n");
                // if (strcmp("test", parameter))
                // {
                //     printString("sama hehe\r\n");
                // }
                printString("\r\n");
                printString(parameter);
                printString("\r\n");

                printInteger(strlen(parameter));
                printString("\r\n");
                interrupt(0x21, (currentDirectory << 8) + 0x04, fileBuffer, parameter, &flag);
                printString("\r\n");
                printInteger(flag);
                printString("\r\n");

                if (flag == -1)
                {
                    printString("No such file\n\r");
                }
                else
                {
                    printString("\r\n");
                    printString(fileBuffer);
                }
            }
            else
            {
                printString("Unknown command\r\n");
            }
            if (historyCount < 4)
            {
                historyCount ++;
            }
            if (historyCount == 4)
            {
                for (i = 3; i >= 1;i++)
                {
                    clear(commandHistory + 256 * i, 256);
                    strcpy(commandHistory + 256 * (i - 1), commandHistory + 256 * i);
                }
            }
            strcpy(command, commandHistory);
            historyPointer = -1;
            arrowPressed = 0;
            
        }
        
    }
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

int pathIndex(char parent, char *path)
{
    char file[1024];

    char P = parent;
    char pos = 0x00;
    char idx = 0x00;

    interrupt(0x21, 0x02, file, 0x101, 0);
    interrupt(0x21, 0x02, file + 512, 0x102, 0);
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

void printString(char *string)
{
    interrupt(0x21, 0x00, string, 0, 0);
}

void printCurrentDirectory(char currentDirectory)
{
    char folderOrder[32];
    char stringToShow[256];
    char dir[1024];
    int i;
    int fileNameIdx;
    int current;
    
    printString("~/");
    current = 0;
    if (currentDirectory != 0xFF) // Sekarang di root
    {
        interrupt(0x21, 0x02, dir, 0x101, 0);
        interrupt(0x21, 0x02, dir + 512, 0x102, 0);
        i = 0;
        while (currentDirectory != 0xFF)
        {
            folderOrder[i] = currentDirectory;
            currentDirectory = dir[currentDirectory * 16];
            i++;
        }
        i--;
        while (i >= 0)
        {
            fileNameIdx = 0;
            while (dir[folderOrder[i] * 16 + 2 + fileNameIdx] != 0x00)
            {
                stringToShow[current] = dir[folderOrder[i] * 16 + 2 + fileNameIdx];
                fileNameIdx++;
                current++;
            }
            if (i > 0)
            {
                stringToShow[current] = "/";
                current++;
            }
            i--;
        }
        stringToShow[current] = 0x00;
        printString(stringToShow);
    }
    printString("$ ");
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
void clear(char *buffer, int length)
{
    int i;
    for (i = 0; i < length; i++)
    {
        buffer[i] = 0x0;
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

void printInteger(int n) {
	int tmp = n;
    int i;
    int length = 1;
	char number[5];
	char isNegative = 0;

    // If n is 0...
	if(n == 0) {
        printString("0");
        return;
    }
    
    // If is negative...
    if(tmp<0)
    {
    	isNegative = 1;
    	tmp = -tmp;
    }

    // Check length of int
    while(tmp>10)
    {
        tmp = div(tmp, 10);
        ++length;
    }

    tmp = n;
	if (isNegative) {
		tmp = -tmp;
	}

	for(i=length-1;i>=0;i--)
	{
		number[i] = mod(tmp,10) + '0';
		tmp = div(tmp, 10);
	}
    number[length] = 0;

    if(isNegative) printString("-");
    printString(number);
}
int mod(int a, int b)
{
    return (a - b * (a / b));
}

int div(int a, int b)
{
    int is_negative = 0;
    if (a < 0 && b < 0)
    {
        a = a * (-1);
        b = b * (-1);
    }
    else if ((a < 0) && (b > 0))
    {
        a = a * (-1);
        is_negative = 1;
    }
    else if ((a > 0) && (b < 0))
    {
        b = b * (-1);
        is_negative = 1;
    }

    if (!is_negative)
    {
        if (b > a)
        {
            return 0;
        }
        else
        {
            return (1 + div(a - b, b));
        }
    }
    else if (is_negative)
    {
        if (b > a)
        {
            return (-1);
        }
        else
        {
            return ((-1) * (1 + div(a - b, b)));
        }
    }
}