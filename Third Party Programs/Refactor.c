#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <windows.h>
#define RENAME_FUNCTION rename
#else
#include <unistd.h>
#define RENAME_FUNCTION rename
#endif

#define MAX_MODS 12
#define MAX_NAME 256

typedef struct {
    char oldName[MAX_NAME];
} ModFolder;

int isModFolder(const char *name, int *num) {
    if (strncmp(name, "mod", 3) == 0) {
        int n = atoi(name + 3);
        if (n > 0 && n <= MAX_MODS) {
            *num = n;
            return 1;
        }
    }
    return 0;
}

int main() {
    DIR *dir;
    struct dirent *entry;
    ModFolder mods[MAX_MODS];
    int count = 0;

    dir = opendir(".");
    if (!dir) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        struct stat st;
        if (stat(entry->d_name, &st) == 0 && S_ISDIR(st.st_mode)) {
            int num;
            if (isModFolder(entry->d_name, &num)) {
                strncpy(mods[count].oldName, entry->d_name, MAX_NAME);
                count++;
            }
        }
    }
    closedir(dir);
    for (int i = 0; i < count && i < MAX_MODS; i++) {
        char newName[MAX_NAME];
        snprintf(newName, sizeof(newName), "mod%d", i + 1);
        if (strcmp(mods[i].oldName, newName) != 0) {
            printf("Renaming %s -> %s\n", mods[i].oldName, newName);
            if (RENAME_FUNCTION(mods[i].oldName, newName) != 0) {
                perror("rename");
            }
        }
    }

    return 0;
}
