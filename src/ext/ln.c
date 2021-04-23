int main()
{
    char param[512];
    char first[256];
    char second[256];
    int indexPath;
    int i;
    int idx;
    char file[1024];
    char *pointer;
    char currentDirectory;

    readSector(file, 0x101);
    readSector(file + 512, 0x102);
    getParam(&currentDirectory, param);
    splitString(param, first, second, ' ');
    indexPath = pathIndex(file, currentDirectory, first);
    for (i = 0; i < 64; i++)
    {
        if (file[16 * i] == 0x00 && file[16 * i + 1] == 0x00 && file[16 * i + 2] == 0x00)
        {
            file[16 * i] = file[indexPath * 16];
            file[16 * i + 1] = file[indexPath * 16 + 1];
            break;
        }
    }

    pointer = second;
    idx = 0;
    while (*pointer != 0x0)
    {
        file[i * 16 + 2 + idx] = *pointer;
        pointer++;
        idx++;
    }

    writeSector(file, 0x101);
    writeSector(file + 512, 0x102);
    backToShell();
}