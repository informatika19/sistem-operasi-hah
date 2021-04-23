int main()
{
    char file[1024];
    char sector[512];
    char map[512];
    char clean[512];
    char param[256];
    int idx;
    int i;
    char sectorLine;
    char currentDirectory;
    clear(param, 256);
    clear(map, 512);
    clear(sector, 512);
    clear(file, 1024);
    clear(clean, 512);
    getParam(&currentDirectory, param);

    readSector(map, 0x100);
    readSector(file, 0x101);
    readSector(file + 512, 0x102);
    readSector(sector, 0x103);
    idx = pathIndex(file, currentDirectory, param);
    sectorLine = file[16 * idx + 1];

    for (i = 0; i < 16; i++)
    {
        file[16 * idx + i] = 0x00;
    }
    for (i = 0; i < 16; i++)
    {
        writeSector(clean, sector[sectorLine * 16 + i]);
        map[sector[sectorLine * 16 + i]] = 0x00;
        sector[sectorLine * 16 + i] = 0x00;
    }
    writeSector(map, 0x100);
    writeSector(file, 0x101);
    writeSector(file + 512, 0x102);
    writeSector(sector, 0x103);
    backToShell();
}