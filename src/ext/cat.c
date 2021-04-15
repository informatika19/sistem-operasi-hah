int main()
{
    char currentDirectory;
    char filename[13];
    char buffer[8192];
    char flag;
    clear(buffer, 8192);
    clear(filename, 13);
    // clear(buffer, 512 * 16);
    // printString("CAT CALLED\n");
    // printString("A\r\n");
    getParam(&currentDirectory, filename);
    // printString(filename);
    // printString("\r\n\r\n");
    // if (currentDirectory == 0xFF)
    // {
    //     printString("Current directory : 0xFF\r\n");
    // }
    // if (currentDirectory == 0xFF)
    // {
    //     printString("Benar");
    // }
    // returnToSender();
    // return 0;
    // printString(&currentDirectory);
    readFile(buffer, filename, &flag, currentDirectory);
    // printString("Isi File:\r\n");
    printString(buffer);
    backToShell();
}
