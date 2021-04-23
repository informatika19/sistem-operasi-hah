int main()
{
    char file[1024];
    char folderName[512];
    char filename[16];
    char param[512];
    char src[512];
    char target[512];
    char currentDirectory;
    int idx;
    int newFolderIdx;
    int i;
    char *temp;
    clear(param, 512);
    clear(folderName, 512);
    clear(src, 512);
    clear(target, 512);
    clear(filename, 16);
    getParam(&currentDirectory, param);
    splitString(param, src, target, ' ');

    readSector(file, 0x101);
    readSector(file + 512, 0x102);

    splitToFolderAndFilename(target, folderName, filename);

    idx = pathIndex(file, currentDirectory, src);

    newFolderIdx = folderIndex(file, currentDirectory, folderName);

    file[16 * idx] = newFolderIdx;
    for (i = 0; i < 14; i++)
    {
        file[16 * idx + 2 + i] = 0x00;
    }
    temp = filename;
    i = 0;
    while (*temp != 0x00)
    {
        file[16 * idx + 2 + i] = *temp;
        temp++;
        i++;
    }

    writeSector(file, 0x101);
    writeSector(file + 512, 0x102);
    backToShell();
}