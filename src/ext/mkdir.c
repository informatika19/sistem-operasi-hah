int main () {
    char currentDirectory;
    char foldername[13];
    int flag;
    getParam(&currentDirectory, foldername);
    makeFolder(currentDirectory, foldername, &flag);
    backToShell();
    
}