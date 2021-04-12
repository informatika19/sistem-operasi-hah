int main()
{
    char currentDirectory;
    char filename[16];
    char buffer[512 * 16];
    char flag;

    getParam(&currentDirectory, filename);

    readFile(buffer, filename, &flag, currentDirectory);

    printString(buffer);
    return 0;
}