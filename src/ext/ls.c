int main()
{
    char file[1024];
    char param[512];
    int i;
    char currentDirectory;

    getParam(&currentDirectory, param);
    // currentDirectory = 0xFF;
    readSector(file, 0x101);
    readSector(file + 512, 0x102);
    for (i = 0; i < 64; i++)
    {
        if (file[i * 16] == currentDirectory && !strbcmp(file + i * 16, 17, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"))
        {
            printString(file + 2 + (i * 16));
            printString("\r\n");
        }
    }
    backToShell();
}
