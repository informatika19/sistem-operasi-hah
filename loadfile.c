// loadFile.c
// Michael Black, 2007

// Loads a file into the file system
// This should be compiled with gcc and run outside of the OS

#include <stdio.h>

void main(int argc, char* argv[]) {
  int i;

  if (argc < 2) {
    printf("Specify file name to load\n");
    return;
  }

  // open the source file
  FILE* loadFil;
  loadFil = fopen(argv[1], "r");
  if (loadFil == 0) {
    printf("File not found\n");
    return;
  }

  // open the floppy image
  FILE* floppy;
  floppy = fopen("system.img", "r+");
  if (floppy == 0) {
    printf("system.img not found\n");
    return;
  }

  // load the disk map
  char map[512];
  fseek(floppy, 512 * 0x100, SEEK_SET);
  for (i = 0; i < 512; i++) map[i] = fgetc(floppy);

  // load the files list
  char files[1024];
  fseek(floppy, 512 * 0x101, SEEK_SET);
  for (i = 0; i < 1024; i++) files[i] = fgetc(floppy);

  // load the sectors list
  char sectors[512];
  fseek(floppy, 512 * 0x103, SEEK_SET);
  for (i = 0; i < 512; i++) sectors[i] = fgetc(floppy);

  // find a free entry in files
  for (i = 0; i < 1024; i = i + 16)
    if (files[i+2] == 0) break;
  if (i == 1024) {
    printf("Not enough room in files\n");
    return;
  }
  int filesindex = i;

  // fill the name field with 00s first
  for (i = 0; i < 14; i++) files[filesindex + i + 2] = 0x00;
  // copy the name over
  for (i = 0; i < 14; i++) {
    if (argv[2][i] == 0) break;
    files[filesindex + i + 2] = argv[2][i];
  }

  // find a free entry in sectors
  for (i = 0; i < 32; i = i + 1)
    if (sectors[i<<4] == 0) break;
  if (i == 32) {
    printf("Not enough room in sectors\n");
    return;
  }

  int secindex = i * 16;
  files[filesindex + 1] = i;
  files[filesindex] = 0xFF;

  // find free sectors and add them to the file
  int sectcount = 0;
  while (!feof(loadFil)) {
    if (sectcount == 20) {
      printf("Not enough space in directory entry for file\n");
      return;
    }

    // find a free map entry
    for (i = 0; i < 256; i++)
      if (map[i] == 0) break;
    if (i == 256) {
      printf("Not enough room for file\n");
      return;
    }

    // mark the map entry as taken
    map[i] = 0xFF;

    // mark the sector in the directory entry
    sectors[secindex] = i;
    secindex++;
    sectcount++;
    
    printf("Loaded %s to sector %d\n", argv[2], i);

    // move to the sector and write to it
    fseek(floppy, i * 512, SEEK_SET);
    for (i = 0; i < 512; i++) {
      if (feof(loadFil)) {
        fputc(0x0, floppy);
        break;
      } else {
        char c = fgetc(loadFil);
        fputc(c, floppy);
      }
    }
  }

  // write the map, files, and sectors back to the floppy image
  fseek(floppy, 512 * 0x100, SEEK_SET);
  for (i = 0; i < 512; i++) fputc(map[i], floppy);

  fseek(floppy, 512 * 0x101, SEEK_SET);
  for (i = 0; i < 1024; i++) fputc(files[i], floppy);

  fseek(floppy, 512 * 0x103, SEEK_SET);
  for (i = 0; i < 512; i++) fputc(sectors[i], floppy);

  fclose(floppy);
  fclose(loadFil);
}