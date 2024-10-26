#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* extractFileName(char *path) {
    char *ptr;
    for (ptr = path + strlen(path); ptr >= path && *ptr != '/'; ptr--);
    ptr++;
    return ptr;
}

void searchFile(char *path, char *targetName) {
    char buffer[512], *ptr;
    int fileDescriptor;
    struct stat fileStat;
    struct dirent directoryEntry;

    if ((fileDescriptor = open(path, 0)) < 0) {
        fprintf(2, "searchFile: cannot open %s\n", path);
        return;
    }

    if (fstat(fileDescriptor, &fileStat) < 0) {
        fprintf(2, "searchFile: cannot stat %s\n", path);
        close(fileDescriptor);
        return;
    }

    switch (fileStat.type) {
    case T_FILE:
        ptr = extractFileName(path);
        if (!strcmp(ptr, targetName))
            printf("%s\n", path);
        break;
    
    case T_DIR:
        while(read(fileDescriptor, &directoryEntry, sizeof(directoryEntry)) == sizeof(directoryEntry)) {
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buffer){
                printf("searchFile: path too long\n");
                break;
            }
            if (directoryEntry.inum == 0)
                continue;
            if (!strcmp(directoryEntry.name, ".") || !strcmp(directoryEntry.name, ".."))
                continue;

            strcpy(buffer, path);
            ptr = buffer + strlen(buffer);
            *ptr++ = '/';
            memmove(ptr, directoryEntry.name, DIRSIZ);
            ptr[DIRSIZ] = '\0';
            searchFile(buffer, targetName);
        }
        break;
    }
    close(fileDescriptor);
    return;
}

void main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "Usage: searchFile <path> <name>\n");
        exit(1);
    }

    searchFile(argv[1], argv[2]);
    exit(0);
}
