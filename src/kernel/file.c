int pathIndex(char *file, char parent, char *path)
{
    char P = parent;
    char pos = 0x00;
    char idx = 0x00;
    char readingState = 0x00;

    if (path[0] == '/')
    {
        pos++;
        P = 0xFF;
    }

    while (path[pos] != 0x00)
    {
        if (path[pos] == '.' && path[pos + 1] == '.' && path[pos + 2] == '/')
        {
            if (P == 0xFF)
            {
                return -1;
            }
            P = file[P * 16];
            pos += 3;
        }
        else if (path[pos] == '.' && path[pos + 1] == '/')
        {
            pos += 2;
        }
        else if (path[pos] == '/')
        {
            // if (readingState == 0)
            // {
            //     return -1;
            // }
            // pos++;
            // readingState = 0;

            if (path[pos + 1] == '/')
            {
                return -1;
            }
            else
            {
                pos++;
            }
        }
        else
        {
            // int i;
            // for (i = 0; i < 64; i++)
            // {
            //     if (P != file[i * 16] || file[i * 16 + 2] == 0)
            //     {
            //         continue;
            //     }
            //     else
            //     {
            //         if (strswith(path[pos], file[i * 16 + 2]))
            //         {
            //             pos += strlen(file[i * 16 + 2]);
            //             readingState = 1;
            //             P = i;
            //             break;
            //         }
            //     }
            // }
            // if (i == 64) {
            //     return -1;
            // }
            if (P != file[idx * 16])
            {
                idx++;
            }
            else if (file[idx * 16 + 2] == 0)
            {
                idx++;
            }
            //TODO blm bkin strswith
            else if (strswith(&(path[pos]), &(file[idx * 16 + 2])))
            {
                pos += strlen(&(file[idx * 16 + 2]));
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

    //if ((P != 0xFF))
    if (P != 0xFF && file[P * 16 + 1] != 0XFF)
    {
        return P;
    }
    else
    {
        return -1;
    }
}

void readFile(char *buffer, char *path, int *result, char parentIndex)
{
    char file[1024];
    char sector[512];

    readSector(file, 0x101);
    readSector(file + 512, 0x102);

    int P = pathIndex(file, parentIndex, path);

    if (P == -1 || (P >> 8) == 0x1)
    {
        *result = -1;
        return;
    }
    readSector(sector, 0x103);

    int S = file[P * 16 + 1];
    int i;
    for (i = 0; i < 16; i++)
    {
        if (sector[S * 16 + i] == 0x00)
        {
            break;
        }
        readSector(&(buffer[i * 512]), sector[i]);
    }
    *result = 1;
}

void writeFile(char *buffer, char *path, int *sectors, char parentIndex)
{

    char dir[1024];
    char sector[512];
    char map[512];

    readSector(dir, 0x101);
    readSector(dir + 512, 0x102);
    readSector(map, 0x103);
    // Cek dir yang kosong
    int dirRow = 0;
    int emptyDirEntry = -1;
    while (emptyDirEntry == -1 && dirRow < 64)
    {
        if (dir[dirRow * 16] == 0x00 && dir[dirRow * 16 + 1] == 0x00 && dir[dirRow * 16 + 2] == 0x0)
        {
            emptyDirEntry = dirRow;
        }
        dirRow++;
    }
    // Entry di files udah full semua
    if (emptyDirEntry == -1)
    {
        *sectors = -2;
    }
    // Cek entri di sectors
    int sectorsRow = 0;
    int emptySectorEntry = -1;

    while (emptySectorEntry == -1 && sectorsRow < 32)
    {
        if (sector[sectorsRow * 16] == 0)
        {
            emptySectorEntry = sectorsRow;
        }
    }
    // Cek sektor yang kosong dengan cek map
    int mapPos = 0;
    int emptySector = 0;
    for (mapPos = 0; mapPos < 512; mapPos++)
    {
        if (map[mapPos] == 0x00)
        {
            emptySector++;
        }
    }

    // Entry di sectors udh full semua
    if (emptySectorEntry == -1)
    {
        *sectors = -2;
        return;
    }
    // Todo : Cek apakah di directory yang sama terdapat nama file yang sama!!

    int bufferSize = strlen(buffer);
    int sectorNeeded = div(bufferSize, 512);
    if (mod(bufferSize, 512) != 0)
    {
        sectorNeeded += 1;
    }
    // Sektor di map udah ga cukup
    if (sectorNeeded > emptySector)
    {
        *sectors = -3;
        return;
    }
    // Edit files
    dir[emptyDirEntry * 16] = parentIndex;
    dir[emptyDirEntry * 16 + 1] = emptySectorEntry;
    char *pathTemp = path;
    int tempIndex = 0;
    while (*pathTemp != 0x00)
    {
        dir[emptyDirEntry * 16 + 2 + tempIndex] = *pathTemp;
        pathTemp++;
        tempIndex++;
    }

    // Edit Sectors dan Map sekaligus writeSector
    int sectorWritten = 0;
    while (sectorWritten < sectorNeeded)
    {
        // Get first index in map yang kosong
        int emptyMapPos = -1;
        mapPos = 0;
        while (emptyMapPos < 0)
        {
            if (map[mapPos] != 0x00)
            {
                emptyMapPos = mapPos;
            }
            mapPos++;
        }
        map[emptyMapPos] = 0xFF;
        writeSector(buffer + sectorWritten * 512, emptyMapPos);
        sector[emptySectorEntry * 16 + sectorWritten] = emptyMapPos;
        sectorWritten++;
    }

    writeSector(map, 0x100);
    writeSector(dir, 0x101);
    writeSector(dir + 512, 0x102);
    writeSector(sector, 0x103);
}