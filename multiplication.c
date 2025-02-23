#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    // Komut satırı argümanlarının doğruluğunu kontrol et
    if (argc != 3) {
        fprintf(stderr, "Usage: %s pipe_read_fd result_pipe_fd\n", argv[0]);
        exit(1); // Yanlış kullanım durumunda programı sonlandır
    }

    // Pipe dosya tanıtıcılarını komut satırı argümanlarından al
    int pipe_read_fd = atoi(argv[1]);
    int result_pipe_fd = atoi(argv[2]);

    while (1) {
        float operand1 = 0.0, operand2 = 0.0; // İşlem yapılacak operandlar
        ssize_t bytes_read;

        // İlk operandı pipe'dan oku
        bytes_read = read(pipe_read_fd, &operand1, sizeof(float));
        if (bytes_read != sizeof(float)) {
            // Okuma başarısızsa hata mesajı yazdır ve sonraki döngüye geç
            fprintf(stderr, "Error reading operand1: %s\n", strerror(errno));
            continue;
        }

        // İkinci operandı pipe'dan oku
        bytes_read = read(pipe_read_fd, &operand2, sizeof(float));
        if (bytes_read != sizeof(float)) {
            // Okuma başarısızsa hata mesajı yazdır ve sonraki döngüye geç
            fprintf(stderr, "Error reading operand2: %s\n", strerror(errno));
            continue;
        }

        // İşlemi gerçekleştir (çarpma işlemi)
        float result = operand1 * operand2; // Çarpma işlemini gerçekleştir

        // Sonucu string'e çevir
        char result_str[20];
        snprintf(result_str, sizeof(result_str), "%.2f", result);

        // Sonucu result_pipe_fd'ye yaz
        if (write(result_pipe_fd, result_str, strlen(result_str) + 1) == -1) {
            // Yazma sırasında hata oluşursa hata mesajını yazdır
            fprintf(stderr, "Error writing result: %s\n", strerror(errno));
        }

        // Sonucu kaydetmek için yeni bir süreç oluştur
        int pid = fork();
        if (pid == -1) {
            // Süreç oluşturma başarısızsa hata mesajını yazdır
            fprintf(stderr, "Error forking child process: %s\n", strerror(errno));
            continue; // Bir sonraki döngüye geç
        }

        if (pid == 0) { // Alt süreç
            // Saver programını çalıştırmak için argümanları hazırla
            char *saver_args[] = {
                "saver",       // Çalıştırılacak program (saver)
                result_str,    // Sonuç string'i
                NULL           // Argüman dizisini sonlandır
            };

            // Execve ile saver programını çalıştır
            if (execve("./saver", saver_args, NULL) == -1) {
                // Execve başarısız olursa hata mesajını yazdır
                fprintf(stderr, "Error executing saver: %s\n", strerror(errno));
                exit(1); // Alt süreci sonlandır
            }
        }

        // Ana süreç, alt sürecin tamamlanmasını beklemeden devam eder
    }

    return 0; // Program başarıyla sonlanır
}
