int availableSector(char *buffer)
{
    int i;
    int size = 0;

    for (i = 0; i < 512; i++)
    {
        if (buffer[i] == 0x00)
        {
            size++;
        }
    }
    return size;
}

int getNextSector(char *buffer)
{
    int i = 0;
    while (buffer[i] != 0x00)
    {
        i++;
    }
    return i;
}

int main()
{
    char file[1024];
    char folderName[512];
    char filename[16];
    char sector[512];
    char map[512];
    char param[512];
    char src[512];
    char target[512];
    char copy[512];
    char currentDirectory;
    int copyIdx;
    int idx;
    int newFolderIdx;
    int i;
    char *temp;
    char sectorLine;
    int copySectorLine;
    char mapTarget;
    char mapNew;
    clear(param, 512);
    clear(folderName, 512);
    clear(src, 512);
    clear(target, 512);
    clear(map, 512);
    clear(sector, 512);
    clear(filename, 16);
    getParam(&currentDirectory, param);
    splitString(param, src, target, ' ');

    readSector(map, 0x100);
    readSector(file, 0x101);
    readSector(file + 512, 0x102);
    readSector(sector, 0x103);

    splitToFolderAndFilename(target, folderName, filename);

    idx = pathIndex(file, currentDirectory, src);

    newFolderIdx = folderIndex(file, currentDirectory, folderName);

    for (i = 0; i < 63; i++)
    {
        if (strbcmp(file + i * 16, 16, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"))
        {
            copyIdx = i;
            break;
        }
    }
    file[16 * copyIdx] = newFolderIdx;
    copySectorLine = availableSector(sector);
    file[16 * copyIdx + 1] = copySectorLine;
    temp = filename;
    i = 0;
    while (*temp != 0x00)
    {
        file[16 * copyIdx + 2 + i] = *temp;
        temp++;
        i++;
    }

    sectorLine = file[16 * idx + 1];
    for (i = 0; i < 16; i++)
    {
        mapTarget = sector[sectorLine * 16 + i];
        if (mapTarget != 0x00)
        {
            mapNew = getNextSector(map);
            map[mapNew] = 0xFF;
            clear(copy, 512);
            readSector(copy, mapTarget);
            writeSector(copy, mapNew);
            sector[copySectorLine * 16 + i] = mapNew;
        }
    }

    writeSector(map, 0x100);
    writeSector(file, 0x101);
    writeSector(file + 512, 0x102);
    writeSector(sector, 0x103);
    backToShell();
}