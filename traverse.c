#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

int main() {
    const char *folderPath = "/home/nix/Desktop/ransom/localroot";
    DIR *dir;
    struct dirent *ent;

    // Dynamically allocate memory for incompletePaths
    int capacity = 10; // Initial capacity
    char **incompletePaths = malloc(capacity * sizeof(char *));
    if (incompletePaths == NULL) {
        return 1;
    }
    
    int count = 0; // Count of directories

    if ((dir = opendir(folderPath)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (count >= capacity) {
                // Resize incompletePaths using realloc
                capacity *= 2;
                char **temp = realloc(incompletePaths, capacity * sizeof(char *));
                if (temp == NULL) {
                    perror("Failed to reallocate memory for incompletePaths");
                    break;
                }
                incompletePaths = temp;
            }

            // Allocate memory for the path and copy the string
            incompletePaths[count] = malloc(strlen(ent->d_name));
            if (incompletePaths[count] == NULL) {
                perror("Failed to allocate memory for incompletePaths[count]");
                break;
            }
            strcpy(incompletePaths[count], ent->d_name);

            count++;
        }
        closedir(dir);
    } else {
        perror("Error opening directory");
        return 1;
    }

    // Print the directory paths
    for (int x = 0; x < count; x++) {
        printf("dirs: %s\n", incompletePaths[x]);
        free(incompletePaths[x]); // Free allocated memory for each path
    }
    free(incompletePaths); // Free the array of pointers

    return 0;
}
