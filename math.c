#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int fd[2]; 
    pid_t pid;
    int n;

    printf("Masukkan angka maksimal: ");
    scanf("%d", &n);

    if (pipe(fd) == -1) {
        perror("Pipe gagal");
        return 1;
    }

    pid = fork();

    if (pid < 0) {
        perror("Fork gagal");
        return 1;
    }

    if (pid > 0) {
        // Parent process
        close(fd[0]); 

        int sum = 0;
        for (int i = 1; i <= n; i++) {
            sum += i;
        }

        // Kirim hasil ke child
        write(fd[1], &sum, sizeof(sum));
        close(fd[1]); 

        wait(NULL); 
    } else {
        // Child process
        close(fd[1]); 

        int result;
        read(fd[0], &result, sizeof(result));
        printf("Child: Jumlah dari 1 sampai %d adalah %d\n", n, result);

        close(fd[0]); 
    }

    return 0;
}
