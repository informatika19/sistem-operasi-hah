void makeFolder(char parentIndex, char * path, int * result)
{
    char buffer[0];
    interrupt(0x21, (parentIndex << 8) + 0x07, buffer, path, result);
}