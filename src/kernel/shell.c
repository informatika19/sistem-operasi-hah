void printCurrentDirectory(char currentDirectory);
void splitString(char *buffer, char *first, char *second, char delimiter);
void printHistory (char * commandHistory, int historyCount);
void runShell ()
{
    char command[256];
    char program[256];
    char parameter[256];
    char file[1024];
    char fileBuffer[8192];
    char commandHistory[1024];
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
            printCurrentDirectory(currentDirectory);
        }
        readString(command);
        printString("\r\n");
        // interrupt(0x21, 0x01, command, 0, 0);
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
                readSector(file, 0x101);
                readSector(file + 512, 0x102);
                indexToMove = pathIndex(file, currentDirectory, parameter);
                if (indexToMove == -1)
                {
                    printString("No such folder\r\n");
                }
                else
                {
                    currentDirectory = indexToMove;
                }
            }
            else if (strbcmp(command, 3, "ls"))
            {
                // interrupt(0x21, 0x02, file, 0x101, 0);
                // interrupt(0x21, 0x02, file + 512, 0x102, 0);
                readSector(file, 0x101);
                readSector(file + 512, 0x102);
                for (i = 0; i < 64; i++)
                {
                    if (file[i * 16] == currentDirectory)
                    {
                        printString(file + i * 16 + 2);
                        printString("\r\n");
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
                // printString("\r\n");
                // printString(parameter);
                // printString("\r\n");

                // printInteger(strlen(parameter));
                // printString("\r\n");
                // interrupt(0x21, (currentDirectory << 8) + 0x04, fileBuffer, parameter, &flag);
                readFile(fileBuffer, parameter, &flag, currentDirectory);
                // printString("\r\n");
                // printInteger(flag);
                // printString("\r\n");

                if (flag == -1)
                {
                    printString("No such file\n\r");
                }
                else
                {
                    printString(fileBuffer);
                    printString("\r\n");
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
            // if (historyCount == 4)
            
            for (i = historyCount; i >= 1;i--)
            {
                clear(commandHistory + i * 256, 256);
                strcpy(commandHistory + (i - 1) * 256, commandHistory + 256 * i);
                // printString(commandHistory + 256 * i);
                // printString("\r\n");
            }
            clear(commandHistory, 256);
            strcpy(command, commandHistory);
            // printString(commandHistory);
            historyPointer = -1;
            arrowPressed = 0;
            // printHistory(commandHistory, historyCount);
            
        }
        
    }
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
        readSector(dir, 0x101);
        readSector(dir + 512, 0x102);
        // interrupt(0x21, 0x02, dir, 0x101, 0);
        // interrupt(0x21, 0x02, dir + 512, 0x102, 0);
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

void printHistory (char * commandHistory, int historyCount)
{
    int i;

    printString("Printing history \r\n");
    for (i = 0; i < historyCount;i++)
    {
        printString(commandHistory + (i * 256));
        printString("\r\n");
    }
}