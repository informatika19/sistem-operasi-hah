void backToShell()
{
    int flag;
    interrupt(0x21, 0xFF06, "shell", 0x1000, &flag);
}