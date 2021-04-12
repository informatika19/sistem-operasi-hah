void readFile(char *buffer, char *path, int *result, char parentIndex)
{
    interrupt(0x21, (parentIndex << 8) + 0x04, buffer, path, result);
}

void writeFile(char *buffer, char *path, int *result, char parentIndex)
{
    interrupt(0x21, (parentIndex << 8) + 0x05, buffer, path, result);
}
//DELETE FILE
//TODO

void getFileName(int index, char *filename)
{
    char files[1024];
    int i;

    // Read sector 0x101 & 0x102
    interrupt(0x21, 0x02, files, 0x101, 0);
    interrupt(0x21, 0x02, files + 512, 0x102, 0);

    for (i = 0; i < 14; i++)
    {
        filename[i] = files[index * 16 + 2 + i];
    }
}