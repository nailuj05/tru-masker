#include <stdio.h>
#include "tinyfiledialogs.h"

int main() {
	const char *filterPatterns[] = { "*.png", "*.jpg" };
    const char *filePath = tinyfd_openFileDialog("Open an image", "", 2, filterPatterns, "png/jpeg", 0);
    
    if (filePath) {
        printf("Selected file: %s\n", filePath);
        FILE *file = fopen(filePath, "r");
        if (file) {
            printf("File opened successfully.\n");
            fclose(file);
        } else {
            printf("Failed to open file.\n");
        }
    } else {
        printf("No file selected.\n");
    }
    
    return 0;
}

