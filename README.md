# Final Project Sistem Operasi IT

## Peraturan
1. Waktu pengerjaan dimulai hari Kamis (19 Juni 2025) setelah soal dibagikan hingga hari Rabu (25 Juni 2025) pukul 23.59 WIB.
2. Praktikan diharapkan membuat laporan penjelasan dan penyelesaian soal dalam bentuk Readme(github).
3. Format nama repository github “Sisop-FP-2025-IT-[Kelas][Kelompok]” (contoh:Sisop-FP-2025-IT-A01).
4. Setelah pengerjaan selesai, seluruh source code dan semua script bash, awk, dan file yang berisi cron job ditaruh di github masing - masing kelompok, dan link github dikumpulkan pada form yang disediakan. Pastikan github di setting ke publik.
5. Commit terakhir maksimal 10 menit setelah waktu pengerjaan berakhir. Jika melewati maka akan dinilai berdasarkan commit terakhir.
6. Jika tidak ada pengumuman perubahan soal oleh asisten, maka soal dianggap dapat diselesaikan.
7. Jika ditemukan soal yang tidak dapat diselesaikan, harap menuliskannya pada Readme beserta permasalahan yang ditemukan.
8. Praktikan tidak diperbolehkan menanyakan jawaban dari soal yang diberikan kepada asisten maupun praktikan dari kelompok lainnya.
9. Jika ditemukan indikasi kecurangan dalam bentuk apapun di pengerjaan soal final project, maka nilai dianggap 0.
10. Pengerjaan soal final project sesuai dengan modul yang telah diajarkan.

## Kelompok C05

Nama                              |     NRP    |
--------------------------------- | ---------- |
Shinta Alya Ramadani              | 5027241016 |
Ahmad Rabbani Fata                | 5027241046 |
Dimas Muhammad Putra              | 5027241076 |
Jofanka Al-kautsar Pangestu Abady | 5027241107 |

## Deskripsi Soal

![WhatsApp Image 2025-06-20 at 20 45 06_05a74c64](https://github.com/user-attachments/assets/78664984-6222-490b-94be-de9b86c30650)

Program diminta untuk menerima input berupa angka dari user, yang kemudian dihitung penjumlahan dari 1 hingga n menggunakan mekanisme Inter Process Communication (IPC) berupa pipe dan fork. Proses parent bertugas menghitung jumlah dari 1 sampai n, lalu mengirimkan hasil perhitungan melalui pipe ke proses child, dan proses child akan menampilkan hasilnya ke layar.

### Catatan

- Program berhasil dijalankan di sistem Linux.
- Menggunakan system call pipe(), fork(), write(), read(), dan wait().
- Proses parent dan child telah sinkron dengan baik, program berhasil menghitung jumlah dari 1 sampai n dan mengirim hasil ke child.
- Tidak menggunakan file eksternal, seluruh komunikasi dilakukan melalui pipe.

Struktur repository:
```
.
├── math.c
└── README.md
```

## Pengerjaan

Poin Soal A : Hitung Penjumlahan 1 hingga n Menggunakan Pipe dan Fork

**Teori**

Inter Process Communication (IPC) adalah metode yang memungkinkan dua atau lebih proses untuk bertukar data. Salah satu bentuk IPC yang umum dalam sistem operasi UNIX/Linux adalah pipe.

Fungsi pipe(int fd[2]) membuat dua file descriptor :
- fd[0] untuk read (membaca data),
- fd[1] untuk write (menulis data).

Setelah pipe() dipanggil, sistem operasi membuat buffer memori yang dapat diakses melalui kedua file descriptor tersebut. Menurut dokumentasi man7.org (pipe(2)), pipe bersifat unidirectional, artinya data hanya bisa mengalir satu arah — dari penulis ke pembaca, dan setiap proses harus menutup sisi pipe yang tidak digunakannya untuk menghindari kebuntuan (man7.org, n.d.).

Dalam program ini :
- Proses utama membuat child menggunakan fork().
- Parent menghitung jumlah dari 1 sampai n, lalu mengirimkan hasilnya ke child lewat write(fd[1], ...).
- Child menerima data melalui read(fd[0], ...), lalu menampilkannya.

Penggunaan wait() pada parent memastikan bahwa parent tidak keluar sebelum child menyelesaikan prosesnya.
Konsep ini dijelaskan dalam Operating System Concepts bahwa pipe merupakan salah satu bentuk komunikasi antar proses yang efisien, terutama dalam skenario parent-child (Silberschatz et al., 2018). Sementara itu, detail teknis seperti struktur fd[], blocking behavior, dan cara penggunaan disajikan secara teknis dalam dokumentasi man7.org dan tutorial praktikal seperti di GeeksforGeeks.

**Solusi**

Berikut adalah kode program c yang kelompok kami gunakan : 
```
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
        close(fd[0]); 

        int sum = 0;
        for (int i = 1; i <= n; i++) {
            sum += i;
        }

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
```
Contoh output :
```
Masukkan angka maksimal: 12  
Child: Jumlah dari 1 sampai 12 adalah 78
```
Penjelasan kode
```
int main() {
    int fd[2]; 
    pid_t pid;
    int n;

```
Penjelasan:
- fd[2]: Array untuk menyimpan file descriptor pipe:
- fd[0]: ujung baca pipe.
- fd[1]: ujung tulis pipe.
- pid_t pid: Variabel untuk menyimpan Process ID hasil fork().
- int n: Variabel untuk menyimpan angka maksimal yang akan dihitung jumlahnya.

```
printf("Masukkan angka maksimal: ");
scanf("%d", &n);
```
Penjelasan:
- Program meminta user memasukkan angka maksimal (n) sebagai batas atas perhitungan jumlah bilangan.

```
if (pipe(fd) == -1) {
perror("Pipe gagal");
return 1;
}
```
Penjelasan:
- pipe(fd) membuat saluran komunikasi antar proses (pipe).
- Jika pipe() gagal, program menampilkan pesan error menggunakan perror() dan keluar dengan return value 1.

```
pid = fork();
if (pid < 0) {
perror("Fork gagal");
return 1;
}

```
Penjelasan:
- fork() digunakan untuk membuat child process.
- Return value:
   - kurang dari 0: Gagal membuat child.
   - 0: Proses child.
   - lebih dari 0: Proses parent.
- Jika gagal, tampilkan pesan error dan keluar.

```
if (pid > 0) {
close(fd[0]); 

int sum = 0;
for (int i = 1; i <= n; i++) {
sum += i;
}

write(fd[1], &sum, sizeof(sum));
close(fd[1]); 

wait(NULL); 
```
Penjelasan:
- Parent menutup ujung baca pipe (fd[0]) karena hanya akan menulis.
- Melakukan perhitungan jumlah bilangan dari 1 sampai n.
- Mengirim hasil perhitungan ke child melalui pipe menggunakan write().
- Menutup ujung tulis pipe (fd[1]) setelah selesai.
- Memanggil wait(NULL) agar parent menunggu child selesai.

```
} else {
close(fd[1]); 

int result;
read(fd[0], &result, sizeof(result));
printf("Child: Jumlah dari 1 sampai %d adalah %d\n", n, result);

close(fd[0]); 
}
```
Penjelasan:
- Child menutup ujung tulis pipe (fd[1]) karena hanya akan membaca.
- Membaca hasil perhitungan dari parent melalui pipe ke variabel result.
- Menampilkan hasil ke layar.
- Menutup ujung baca pipe (fd[0]) setelah selesai.


**Video Menjalankan Program**
```
https://github.com/user-attachments/assets/4ca86ce6-1f18-4974-a16f-176505909320
```
- Jika ingin langsung jalankan, geser ke menit 01:37

## Daftar Pustaka

1. Silberschatz, A., Galvin, P. B., & Gagne, G. (2018). Operating System Concepts (10th ed.). Wiley.
2. GeeksforGeeks. (n.d.). Pipe in C. Retrieved from https://www.geeksforgeeks.org/pipe-system-call/
3. man7.org. (n.d.). pipe(2) - Linux manual page. Retrieved from https://man7.org/linux/man-pages/man2/pipe.2.html
