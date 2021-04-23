void printString(char *buffer)
{
    interrupt(0x21, 0x00, buffer, 0, 0);
}
void readFile(char *buffer, char *path, int *result, char parentIndex)
{
    interrupt(0x21, (parentIndex << 8) + 0x04, buffer, path, result);
}

void writeFile(char *buffer, char *path, int *result, char parentIndex)
{
    interrupt(0x21, (parentIndex << 8) + 0x05, buffer, path, result);
}

void readSector(char *buffer, int sector)
{
    interrupt(0x21, 0x02, buffer, sector, 0);
}

void writeSector(char *buffer, int sector)
{
    interrupt(0x21, 0x03, buffer, sector, 0);
}
//DELETE FILE
//TODO

void getFileName(int index, char *filename)
{
    char files[1024];
    int i;

    // Read sector 0x101 & 0x102
    readSector(files, 0x101);
    readSector(files + 512, 0x102);
    // interrupt(0x21, 0x02, files, 0x101, 0);
    // interrupt(0x21, 0x02, files + 512, 0x102, 0);

    for (i = 0; i < 14; i++)
    {
        filename[i] = files[index * 16 + 2 + i];
    }
}
