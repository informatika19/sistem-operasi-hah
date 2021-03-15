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

void readFile(char *buffer, char *path, int *result, char parentIndex)
{
    char file[1024];
    char sector[512];
    int P, S, i;

    readSector(file, 0x101);
    readSector(file + 512, 0x102);
    P = pathIndex(file, parentIndex, path);

    if (P == -1)
    {
        *result = -1;
        return;
    }
    readSector(sector, 0x103);

    S = file[P * 16 + 1];
    for (i = 0; i < 16; i++)
    {
        if (sector[S * 16 + i] == 0x00)
        {
            break;
        }
        readSector(buffer + (i * 512), sector[S * 16 + i]);
    }
    *result = 1;
}

void writeFile(char *buffer, char *path, int *sectors, char parentIndex)
{

    char file[1024];
    char sector[512];
    char map[512];

    char *pathTemp;
    int dirRow, emptyDirEntry, sectorsRow;
    int sectorWritten, emptyMapPos, emptySectorEntry;
    int emptySector, bufferSize;
    int sectorNeeded, tempIndex;

    readSector(file, 0x101);
    readSector(file + 512, 0x102);

    if (file[16 * parentIndex + 1] != 0xFF && parentIndex != 0xFF)
    {
        *sectors = -4;
        return;
    }

    readSector(map, 0x103);
    // Cek file yang kosong
    dirRow = 0;
    emptyDirEntry = -1;

    while (emptyDirEntry == -1 && dirRow <= 0x3f)
    {
        if (strbcmp(file + dirRow * 16, 16, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"))
        {
            emptyDirEntry = dirRow;
        }
        dirRow++;
    }
    // Entry di files udah full semua / Entry di sectors udh full semua
    if (emptyDirEntry == -1)
    {
        *sectors = -2;
        return;
    }
    emptySector = availableSector(sector);

    bufferSize = strlen(buffer);
    sectorNeeded = div(bufferSize, 512);
    if (mod(bufferSize, 512) != 0)
    {
        sectorNeeded += 1;
    }
    if (emptySector == 0 || sectorNeeded > emptySector)
    {
        *sectors = -3;
        return;
    }
    strsntz(path, 14);
    // Cek entri di sectors
    sectorsRow = 0;
    emptySectorEntry = -1;
    while (emptySectorEntry == -1 && sectorsRow <= 0x1f)
    {
        if (sector[sectorsRow * 16] == 0x00)
        {
            emptySectorEntry = sectorsRow;
        }
    }
    // Cek sektor yang kosong dengan cek map
    for (dirRow = 0; dirRow < 64; dirRow++)
    {
        if (file[dirRow * 16] == parentIndex &&
            strcmp(path, file + (dirRow * 16 + 2)))
        {
            // Udah ada di file yang sama
            *sector = -1;
            return;
        }
    }

    // Edit files
    clear(file+emptyDirEntry*16, 16);
    file[emptyDirEntry * 16] = parentIndex;
    file[emptyDirEntry * 16 + 1] = emptySectorEntry;
    pathTemp = path;
    tempIndex = 0;

    //Make sure the file/folder name is 13 characters long + 1 null character

    while (*pathTemp != 0x00)
    {
        file[emptyDirEntry * 16 + 2 + tempIndex] = *pathTemp;
        pathTemp++;
        tempIndex++;
    }
    // Edit Sectors dan Map sekaligus writeSector
    sectorWritten = 0;
    while (sectorWritten < sectorNeeded)
    {
        // Get first index in map yang kosong

        emptyMapPos = getNextSector(map);
        map[emptyMapPos] = 0xFF;
        clearSector(emptyMapPos);
        writeSector(buffer + sectorWritten * 512, emptyMapPos);
        sector[emptySectorEntry * 16 + sectorWritten] = emptyMapPos;
        sectorWritten++;
    }

    writeSector(map, 0x100);
    writeSector(file, 0x101);
    writeSector(file + 512, 0x102);
    writeSector(sector, 0x103);
}