#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s result\n", argv[0]);
        exit(1);
    }

    // Gelen sonucu al
    const char *result = argv[1];

    // Sonucu bir dosyaya kaydet
    FILE *file = fopen("results.txt", "a");
    if (!file) {
        perror("Error opening results.txt");
        exit(1);
    }

    // Dosyaya yaz ve kapat
    fprintf(file, "Result: %s\n", result);
    fclose(file);



    return 0;
}
