#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Fonksiyon, matematiksel işlem alt sürecini oluşturur ve execve ile çalıştırır.
void execute_operation(int pipe_read_fd, int result_pipe_fd, const char *operation_path) {
    pid_t pid = fork(); // Yeni bir süreç oluştur.
    if (pid == 0) { // Alt süreç
        // Pipe dosya tanıtıcılarını string olarak al.
        char pipe_read_str[10], result_pipe_str[10];
        snprintf(pipe_read_str, sizeof(pipe_read_str), "%d", pipe_read_fd);
        snprintf(result_pipe_str, sizeof(result_pipe_str), "%d", result_pipe_fd);

        // Alt sürece gerekli argümanları gönder.
        char *args[] = {(char *)operation_path, pipe_read_str, result_pipe_str, NULL};
        execve(operation_path, args, NULL); // Alt süreci çalıştır.
        perror("execve failed"); // Eğer execve başarısız olursa hata mesajı yaz.
        exit(1); // Alt süreci sonlandır.
    }
}

int main() {
    // Her işlem için pipe'lar tanımlanır.
    int pipe_add[2], pipe_sub[2], pipe_mul[2], pipe_div[2], result_pipe[2];

    // Pipe'lar oluşturulur ve herhangi bir hata durumunda program sonlandırılır.
    if (pipe(pipe_add) == -1 || pipe(pipe_sub) == -1 || 
        pipe(pipe_mul) == -1 || pipe(pipe_div) == -1 || 
        pipe(result_pipe) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }

    // Matematiksel işlemler için alt süreçleri başlat.
    execute_operation(pipe_add[0], result_pipe[1], "./addition");
    execute_operation(pipe_sub[0], result_pipe[1], "./subtraction");
    execute_operation(pipe_mul[0], result_pipe[1], "./multiplication");
    execute_operation(pipe_div[0], result_pipe[1], "./division");

    // Pipe okuma uçları ana süreçte kapatılır.
    close(pipe_add[0]);
    close(pipe_sub[0]);
    close(pipe_mul[0]);
    close(pipe_div[0]);
    close(result_pipe[1]);

    int operation;      // Kullanıcı tarafından seçilen işlem.
    float operand1, operand2; // Kullanıcı tarafından girilen iki operand.

    while (1) {
        // Kullanıcıya menü gösterilir.
        printf("\nCalculator\n");
        printf("1- addition\n");
        printf("2- subtraction\n");
        printf("3- multiplication\n");
        printf("4- division\n");
        printf("5- exit\n");
        printf("Choose an operation: ");

        // Kullanıcının işlem seçimini al.
        if (scanf("%d", &operation) != 1) { 
            fprintf(stderr, "Invalid input.\n");
            while (getchar() != '\n'); // Giriş tamponunu temizle.
            continue;
        }

        if (operation == 5) { // Çıkış seçeneği
            printf("Exiting Calculator...\n");
            break;
        } else if (operation < 1 || operation > 5) { // Geçersiz işlem kontrolü
            printf("Invalid operation. Please choose a valid option between 1 and 5.\n");
            continue;
        }

        // İki operand'ı kullanıcıdan al.
        printf("Enter two operands (operand1 operand2): ");
        if (scanf("%f %f", &operand1, &operand2) != 2) {
            fprintf(stderr, "Invalid input. Try again.\n");
            while (getchar() != '\n');
            continue;
        }

        // Seçilen işleme göre ilgili pipe'a veri yazılır.
        switch (operation) {
            case 1:
                write(pipe_add[1], &operand1, sizeof(float));
                write(pipe_add[1], &operand2, sizeof(float));
                break;
            case 2:
                write(pipe_sub[1], &operand1, sizeof(float));
                write(pipe_sub[1], &operand2, sizeof(float));
                break;
            case 3:
                write(pipe_mul[1], &operand1, sizeof(float));
                write(pipe_mul[1], &operand2, sizeof(float));
                break;
            case 4:
                if (operand2 == 0) { // Sıfıra bölme hatası kontrolü
                    printf("Error: Division by zero.\n");
                    continue;
                }
                write(pipe_div[1], &operand1, sizeof(float));
                write(pipe_div[1], &operand2, sizeof(float));
                break;
            default:
                printf("Invalid operation. Try again.\n");
                continue;
        }

        // Alt süreçten sonucu okur ve ekrana yazdırır.
        char result[20];
        ssize_t bytes_read = read(result_pipe[0], result, sizeof(result));
        if (bytes_read > 0) {
            result[bytes_read] = '\0'; // Sonucu null-terminatör ile bitir.
            printf("Result: %s\n", result);
        } else {
            perror("Error reading result");
        }
    }

    // Ana süreçte kalan açık pipe uçları kapatılır.
    close(pipe_add[1]);
    close(pipe_sub[1]);
    close(pipe_mul[1]);
    close(pipe_div[1]);
    close(result_pipe[0]);
    
    return 0; // Program başarıyla sonlandırılır.
}
