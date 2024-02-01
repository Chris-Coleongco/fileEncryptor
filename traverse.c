#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

int main() {
    const char *folderPath = "/home/nix/Desktop/ransom/localroot";
    DIR *dir;
    struct dirent *ent;

    // Dynamically allocate memory for incompletePaths
    int capacity1 = 10; // Initial capacity for incompletePaths
                        //
    int capacity2 = 10;
    char **incompletePaths = malloc(capacity1 * sizeof(char *));
    char **completePaths = malloc(capacity2 * sizeof(char *));

    if (incompletePaths == NULL) {
        return 1;
    }

    if (completePaths == NULL) {
        return 1;
    }
    
    int count1 = 0; // Count of directories

    int count2 = 0;

    if ((dir = opendir(folderPath)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if ((ent->d_type) == DT_DIR) {
                if (count1 >= capacity1) {
                    // Resize incompletePaths using realloc
                    capacity1 *= 2;
                    char **temp = realloc(incompletePaths, capacity1 * sizeof(char *));
                if (temp == NULL) {
                    perror("Failed to reallocate memory for incompletePaths");
                    break;
                }
                    incompletePaths = temp;
                }

            // Allocate memory for the path and copy the string
                incompletePaths[count1] = malloc(strlen(ent->d_name));
                
                if (incompletePaths[count1] == NULL) {
                    perror("Failed to allocate memory for incompletePaths[count1]");
                    break;
                }
            
                strcpy(incompletePaths[count1], ent->d_name);
                count1++;
            }

            else if ((ent->d_type) == DT_REG) {
                if (count2 >= capacity2) {
                    // Resize incompletePaths using realloc
                    capacity2 *= 2;
                    char **temp2 = realloc(completePaths, capacity2 * sizeof(char *));
                if (temp2 == NULL) {
                    perror("Failed to reallocate memory for incompletePaths");
                    break;
                }
                    completePaths = temp2;
                }

            // Allocate memory for the path and copy the string
                completePaths[count2] = malloc(strlen(ent->d_name));
                
                if (completePaths[count2] == NULL) {
                    perror("Failed to allocate memory for incompletePaths[count2]");
                    break;
                }
            
                strcpy(completePaths[count2], ent->d_name);
                count2++;
            }
            else {
                perror("what da hail??");
            }

        }
        closedir(dir);
    } else {
        perror("Error opening directory");
        return 1;
    }

    // Print the directory paths
    for (int x = 0; x < count1; x++) {
        printf("dirs: %s\n", incompletePaths[x]);
        free(incompletePaths[x]); // Free allocated memory for each path
    }
    free(incompletePaths); // Free the array of pointers

    for (int y = 0; y < count2; y++) {
        printf("files: %s\n", completePaths[y]);
        free(completePaths[y]); // Free allocated memory for each path
    }
    free(completePaths); // Free the array of pointers


    return 0;
}
