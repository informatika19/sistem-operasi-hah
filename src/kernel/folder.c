void makeFolder (char parentIndex, char * path, int * result) {
    char file[1024];
    int i;
    int emptyDirEntry;
    int dirRow;
    char * pathTemp;
    int tempIndex;

    readSector(file, 0x101);
    readSector(file + 512, 0x102);
    emptyDirEntry = -1;
    dirRow = 0;


    while (emptyDirEntry == -1 && dirRow <= 0x3F) {
        if (strbcmp(file + dirRow * 16, 16, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"))
        {
            emptyDirEntry = dirRow;
        }
        dirRow++;
    }
    if (emptyDirEntry == -1)
    {
        *result = -2;
        return;
    }

    file[emptyDirEntry * 16] = parentIndex;
    file[emptyDirEntry * 16 + 1] = 0xFF;
    pathTemp = path;
    tempIndex = 0;
    while (*pathTemp != 0x00)
    {
        file[emptyDirEntry * 16 + 2 + tempIndex] = *pathTemp;
        pathTemp++;
        tempIndex++;
    }
    *result = 1;
    writeSector(file, 0x101);
    writeSector(file + 512, 0x102);

}