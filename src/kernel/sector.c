void readSector(char *buffer, int sector)
{
    interrupt(0x13, 0x201, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

void writeSector(char *buffer, int sector)
{
    interrupt(0x13, 0x301, buffer, div(sector, 36) * 0x100 + mod(sector, 18) + 1, mod(div(sector, 18), 2) * 0x100);
}

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