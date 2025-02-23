#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    // Komut satırı argümanlarının doğruluğunu kontrol et
    if (argc != 3) {
        fprintf(stderr, "Usage: %s pipe_read_fd result_pipe_fd\n", argv[0]);
        exit(1);
    }

    // Komut satırından gelen boru hattı dosya tanımlayıcılarını al
    int pipe_read_fd = atoi(argv[1]);
    int result_pipe_fd = atoi(argv[2]);

    while (1) {
        float operand1 = 0.0, operand2 = 0.0;
        ssize_t bytes_read;

        // İlk operandı oku
        bytes_read = read(pipe_read_fd, &operand1, sizeof(float));
        if (bytes_read != sizeof(float)) {
            fprintf(stderr, "Error reading operand1: %s\n", strerror(errno));
            continue; // Hata durumunda döngünün başına dön
        }

        // İkinci operandı oku
        bytes_read = read(pipe_read_fd, &operand2, sizeof(float));
        if (bytes_read != sizeof(float)) {
            fprintf(stderr, "Error reading operand2: %s\n", strerror(errno));
            continue; // Hata durumunda döngünün başına dön
        }

        // İşlemi gerçekleştir ve sonucu hazırla
        float result = operand1 - operand2;
        char result_str[20];
        snprintf(result_str, sizeof(result_str), "%.2f", result); // Sonucu string olarak biçimlendir

        // Sonucu ana programa yaz
        if (write(result_pipe_fd, result_str, strlen(result_str) + 1) == -1) {
            fprintf(stderr, "Error writing result: %s\n", strerror(errno));
        }

        // Sonucu dosyaya kaydetmek için yeni bir süreç oluştur
        int pid = fork();
        if (pid == -1) {
            fprintf(stderr, "Error forking child process: %s\n", strerror(errno));
            continue;
        }

        if (pid == 0) {  // Çocuk süreç
            // saver programını çalıştırmak için argümanları hazırla
            char *saver_args[] = {
                "saver",     // Çalıştırılacak program adı
                result_str,  // Sonucu argüman olarak geçir
                NULL
            };

            // execve kullanarak saver programını çalıştır
            if (execve("./saver", saver_args, NULL) == -1) {
                fprintf(stderr, "Error executing saver: %s\n", strerror(errno));
                exit(1); // Çocuk süreç başarısızlıkla sonlanır
            }
        }
    }

    return 0; // Program başarıyla sonlanır
}
