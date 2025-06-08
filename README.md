# Sisop-5-2025-IT40

## Kelompok

Nama | NRP
--- | ---
Ahmad Ibnu Athaillah | 5027241024
Muhammad Ahsani Taqwiin Rakhman | 5027231099
I Gede Bagus Saka Sinatrya | 5027241088


## Daftar Isi

- [Kelompok](#kelompok)
- [Daftar Isi](#daftar-isi)
- [Soal](#soal)
- [Petunjuk Soal](#petunjuk-soal)
  - [Source](#source)
  - [Headers](#headers)
  - [Makefile](#makefile)
  - [Video Demonstrasi](#video-demonstrasi)
- [Laporan](#laporan)

## Soal

Pada suatu hari, anda merasa sangat lelah dari segala macam praktikum yang sudah ada, sehingga anda berencana untuk tidur yang nyenyak di sebuah jam 3:34AM yang cerah. Tetapi, anda terbangun di dalam dunia berbeda yang bernama "Eorzea". Ada sesuatu yang mengganggu pikiran anda sehingga anda diharuskan membuat sebuah operating system bernama "EorzeOS" untuk mendampingi diri anda dalam dunia ini.

1. Sebagai seorang main character dari dunia ini, ternyata anda memiliki kekuatan yang bernama "The Echo", kekuatan ini memungkinkan anda untuk berbicara pada Operating System ini (mungkin sebenarnya bukan ini kekuatannya, tetapi ini cukup kuat juga), dengan tujuan agar semua hal yang anda katakan, bila bukan merupakan sebuah command yang valid, akan mengulang hal yang anda katakan.

   Ilustrasi:
   ```
   user> Hello!
   Hello!
   user> I have the Echo
   I have the Echo
   ```

2. gurt: yo

   Ilustrasi:
   ```
   user> yo
   gurt
   user> gurt
   yo
   ```

3. Seorang main character memerlukan sebuah nama yang semua orang bisa ingat dengan baik. Buatlah sebuah command yang memungkinkan pengguna untuk mengubah nama user pada shell yang digunakan:
   * `user <username>` = mengubah username menjadi `<username>`
   * `user` = mengubah username menjadi default `user`
   
   Ilustrasi:
   ```
   user> user Tia
   Username changed to Tia
   Tia> user
   Username changed to user
   user>
   ```

4. Tiga negara besar dari Eorzean Alliance butuh bantuan anda untuk ikut serta dalam "Grand Company" mereka sehingga anda bisa mengubah warna terminal ajaib anda sesuai warna utama dari company mereka:
   * `grandcompany maelstrom` = clear terminal, ubah semua teks berikutnya jadi merah
   * `grandcompany twinadder` = clear terminal, ubah semua teks berikutnya jadi kuning
   * `grandcompany immortalflames` = clear terminal, ubah semua teks berikutnya jadi biru
   * `grandcompany <selain atau kosong>` = tunjukkan error message
   * `clear` = clear terminal, ubah semua teks berikutnya kembali jadi awal (para Grand Company sedih kamu netral)

   Selain mengubah seluruh warna terminal, nama anda di dalam terminal akan diberikan tambahan nama judul Grand Company:
   * Maelstrom = `user@Storm`
   * Twin Adder = `user@Serpent`
   * Immortal Flames = `user@Flame`
   * `clear` = menghapus nama grand company

   Ilustrasi:
   ```
   gurt> grandcompany maelstrom
   -- terminal clear menjadi warna merah --
   gurt@Storm> clear
   -- terminal clear menjadi warna putih --
   ```

5. Sebagai pahlawan terkenal di antara ketiga negara besar Eorzean Alliance, salah satu supplier senjata terbesar di seluruh Eorzea bernama "Rowena's House of Splendors" tiba-tiba memerlukan bantuan anda untuk membuat sebuah sistem kalkulator sederhana melalui command karena pemimpin mereka tertidur setelah mengurus semua orang di dalam Eorzea:
   * `add <x> <y>` = x + y
   * `sub <x> <y>` = x - y
   * `mul <x> <y>` = x * y
   * `div <x> <y>` = x / y

   Ilustrasi:
   ```
   user> add 4 2
   6
   user> sub 4 2
   2
   user> mul 3 -2
   -6
   user> div -6 -2
   3
   ```

6. me: yogurt
   
   gurt:
   * `yo`
   * `ts unami gng </3`
   * `sygau`

   pilih secara *random*

   Ilustrasi:
   ```
   user> yogurt
   gurt> yo
   user> yogurt
   gurt> ts unami gng </3
   user> yogurt
   gurt> sygau
   ```

8. Perusahaan mesin "Garlond Ironworks" tiba-tiba lelah mengurus permintaan senjata perang untuk orang ke-148649813234 yang berusaha menghadapi final boss yang sama, sehingga mereka perlu bantuan kamu untuk melengkapi `Makefile` yang diberikan dengan command-command yang sesuai untuk compile seluruh operating system ini.

## Petunjuk Soal

### Source

* [kernel.asm](src/kernel.asm)
  * `_putInMemory`: penjelasan terdapat dalam modul
  * `_interrupt`: fungsi untuk interrupt
    * `number`: interrupt vector number
    * `AX`,`BX`,`CX`,`DX`: register untuk diisi
    * `AX` merupakan kombinasi dari `AH` dan `AL`, pola ini juga berlaku untuk `BX`,`CX`, dan `DX`
    * Untuk menggabungkan jenis register `H` dan `L` menjadi `X` bisa menggunakan salah satu metode berikut:
      ```c
      AX = AH << 8 | AL    // first method
      AX = AH * 256 + AL   // second method
      ```
   * `getBiosTick`: fungsi untuk mendapatkan tick dari BIOS
* [kernel.c](src/kernel.c)
  * Diisi penerapan fungsi `printString`, `readString`, dan `clearScreen` dengan bantuan `kernel.asm`
    * untuk `printString`: Implementasi dapat menggunakan fungsi `interrupt` dengan service `int 10h` dengan parameter `AH = 0x0E` untuk _teletype output_. Karakter yang ditampilkan dapat dimasukkan pada register `AL`. Fungsi ini akan menampilkan string karakter ASCII (_null-terminated_) ke layar.
    * untuk `readString`: Implementasi dapat menggunakan fungsi `interrupt` dengan service `int 16h` dengan parameter `AH = 0x00` untuk _keyboard input_. Fungsi ini akan membaca karakter ASCII (_non-control_) yang dimasukkan oleh pengguna dan menyimpannya pada buffer hingga menekan tombol `Enter`. Handle tombol `Backspace` dibebaskan kepada praktikan.
    * untuk `clearScreen`: Ukuran layar adalah `80x25` karakter. Setelah layar dibersihkan, kursor akan kembali ke posisi awal yaitu `(0, 0)` dan buffer video untuk warna karakter diubah ke warna putih. Implementasi dapat menggunakan fungsi `interrupt` dengan service `int 10h` atau menggunakan fungsi `putInMemory` untuk mengisi memori video.
* [shell.c](src/shell.c)
  * Diisi penerapan shell yang menggunakan fungsi kernel untuk parsing keseluruhan command yang diinput

### Headers

* [std_type.h](include/std_type.h)
  * `byte`: unsigned char data type, untuk angka 0-255 (`0x00`-`0xFF`)
  * `bool`: untuk boolean (true/false), karena boolean tidak built-in
* [std_lib.h](include/std_lib.h)
  * `div`: division
  * `mod`: modulo
  * Pembagian dan modulo tidak ada dalam assembly, sehingga harus dilengkapi dengan operator yang tersedia (`+`,`-`,`*`,`<<`,`>>`)
  * `strcmp`: membandingkan dua string
  * `strcpy`: copy string
  * `clear`: fill memory dengan `0`
  * `atoi`: alphanumeric to integer (string menjadi angka)
  * `itoa`: integer to alphanumeric (angka menjadi string)
* [kernel.h](include/kernel.h)
  * Deklarasi header untuk fungsi-fungsi dalam `kernel.c`
* [shell.h](include/shell.h)
  * Deklarasi header untuk fungsi-fungsi dalam `shell.c`

### Makefile

* [makefile](./makefile)
   * `prepare` : membuat disk image baru `floppy.img` pada direktori `bin/` dengan ukuran 1.44 MB.
   * `bootloader` : mengkompilasi `bootloader.asm` menjadi `bootloader.bin` pada direktori `bin/`.
   * `stdlib` : mengkompilasi `std_lib.c` menjadi `std_lib.o` pada direktori `bin/`.
   * `shell`: mengkompilasi `shell.c` menjadi `shell.o` pada direktori `bin/`.
   * `kernel` : mengkompilasi `kernel.c` menjadi `kernel.o` pada direktori `bin/` dan mengkompilasi `kernel.asm` menjadi `kernel_asm.o` pada direktori `bin/`.
   * `link` : menggabungkan `bootloader.bin`, `kernel.o`, `kernel_asm.o`, dan `std_lib.o` menjadi `floppy.img`.
   * `build` : menjalankan perintah `prepare`, `bootloader`, `stdlib`, `kernel`, dan `link`.

## Laporan

#### kernel.c
```c
#include "kernel.h"
#include "shell.h"      
#include "std_type.h"   

// Konstanta untuk Manajemen Layar
#define VIDEO_MEMORY_SEGMENT 0xB800
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define DEFAULT_TEXT_ATTRIBUTE 0x07 

// Variabel Global Kernel
static int g_cursor_x = 0;
static int g_cursor_y = 0;
static byte g_current_text_attribute = DEFAULT_TEXT_ATTRIBUTE;

// Fungsi Kursor bios
static void update_cursor_bios() { // Memanggil interrupt BIOS INT 0x10, fungsi 0x02 untuk memindahkan kursor ke posisi (g_cursor_y, g_cursor_x).
    int ax = (0x02 << 8); 
    int bx = 0x0000;      
    int dx = (g_cursor_y << 8) | g_cursor_x; 
    _interrupt_invoke(0x10, ax, bx, 0, dx);
}

// Fungsi scroll
static void scroll_screen() {
    int x, y;
    unsigned int offset_dst; 
    y = SCREEN_HEIGHT - 1; // Baris terakhir
    for (x = 0; x < SCREEN_WIDTH; x++) {
        offset_dst = (y * SCREEN_WIDTH + x) * 2;  // Hitung offset 
        _putInMemory(VIDEO_MEMORY_SEGMENT, offset_dst, ' '); // Kosongkan karakter
        _putInMemory(VIDEO_MEMORY_SEGMENT, offset_dst + 1, g_current_text_attribute); // Set warna
    }

    g_cursor_y = SCREEN_HEIGHT - 1;
    g_cursor_x = 0;
}

// Warna text
void setGlobalTextColor(byte attribute) {
    g_current_text_attribute = attribute;
}

// Cetak karakter
void printChar(char c) {
    unsigned int offset;

    if (c == '\n') {  // Enter: pindah ke baris baru
        g_cursor_y++;
        g_cursor_x = 0;
    } else if (c == '\r') {  // Carriage return: kembali ke awal baris
        g_cursor_x = 0;
    } else if (c == '\b') {  // Backspace
        if (g_cursor_x > 0) {
            g_cursor_x--;
            offset = (g_cursor_y * SCREEN_WIDTH + g_cursor_x) * 2;
            _putInMemory(VIDEO_MEMORY_SEGMENT, offset, ' ');
            _putInMemory(VIDEO_MEMORY_SEGMENT, offset + 1, g_current_text_attribute);
        } else if (g_cursor_y > 0) {
            g_cursor_y--;
            g_cursor_x = SCREEN_WIDTH - 1;  // Kembali ke ujung baris sebelumnya
        }
    } else if (c >= ' ') {
        offset = (g_cursor_y * SCREEN_WIDTH + g_cursor_x) * 2;
        _putInMemory(VIDEO_MEMORY_SEGMENT, offset, (byte)c);
        _putInMemory(VIDEO_MEMORY_SEGMENT, offset + 1, g_current_text_attribute);
        g_cursor_x++;
    }

    if (g_cursor_x >= SCREEN_WIDTH) {
        g_cursor_x = 0;
        g_cursor_y++;
    }
    if (g_cursor_y >= SCREEN_HEIGHT) {
        scroll_screen();
    }
    update_cursor_bios();
}

// Cetak string
void printString(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        printChar(str[i]);
        i++;
    }
}

void clearScreen() {
    int x, y;
    unsigned int offset;
    for (y = 0; y < SCREEN_HEIGHT; y++) {
        for (x = 0; x < SCREEN_WIDTH; x++) {
            offset = (y * SCREEN_WIDTH + x) * 2;
            _putInMemory(VIDEO_MEMORY_SEGMENT, offset, ' ');
            _putInMemory(VIDEO_MEMORY_SEGMENT, offset + 1, g_current_text_attribute);
        }
    }
    g_cursor_x = 0;
    g_cursor_y = 0;
    update_cursor_bios();
}

// Baca karakter dari keyboard
char readKeyboardChar() { // Memanggil INT 0x16, fungsi 0x00 untuk membaca 1 karakter dari keyboard
    int ax_input = (0x00 << 8);
    int returned_ax = _interrupt_invoke(0x16, ax_input, 0, 0, 0);
    return (char)(returned_ax & 0xFF); // Mengembalikan karakter ASCII dari keyboard
}

// baca string dari keyboard
void readString(char *buf) {
    int i = 0;
    char c;
    while (1) {
        c = readKeyboardChar();
        if (c == '\r') {
            buf[i] = '\0';
            printChar('\n');
            break;
        } else if (c == '\b') {
            if (i > 0) {
                i--;
                printChar('\b');
            }
        } else if (c >= ' ' && c <= '~') {

            buf[i] = c;
            printChar(c);
            i++;
        }
    }
}

int main() {
  
    int ax_debug = (0x0E << 8) | 'K'; 
    int bx_debug = 0x0007;            
    _interrupt_invoke(0x10, ax_debug, bx_debug, 0, 0); 
 
    clearScreen(); 
    setGlobalTextColor(DEFAULT_TEXT_ATTRIBUTE); 
    
    printString("Welcome to EorzeOS!\n");
    
    shell(); 
    
    while(1); 
    return 0; 
}
```

#### shell.c
```c
#include "shell.h"
#include "kernel.h"
#include "std_lib.h"
#include "std_type.h" 

// Konstanta
#define INPUT_BUFFER_SIZE 128
#define CMD_BUFFER_SIZE 64
#define ARG_BUFFER_SIZE 64
#define USERNAME_MAX_LEN 15
#define SUFFIX_MAX_LEN 15

// Definisi Warna 
#define TEXT_COLOR_DEFAULT 0x07
#define TEXT_COLOR_RED     0x04
#define TEXT_COLOR_YELLOW  0x0E
#define TEXT_COLOR_BLUE    0x01


// Variabel Global 
static char g_currentUsername[USERNAME_MAX_LEN + 1] = "user";
static char g_grandCompanySuffix[SUFFIX_MAX_LEN + 1] = "";
static char* g_yogurtResponses[] = {
    "yo",
    "ts unami gng </3",
    "sygau"
};
static int g_yogurtResponseCount = 3; // Sesuai dengan jumlah respon di atas
static unsigned int g_randomSeed = 0;

// Definisi Fungsi Helper 
static void initRandomSeed() {
    if (g_randomSeed == 0) {
        g_randomSeed = getBiosTick(); // getBiosTick harus dideklarasikan di kernel.h
    }
}

static int simpleRandom(int max) {
    if (max <= 0) return 0; // Tangani kasus max tidak valid
    if (g_randomSeed == 0) {
        initRandomSeed();
    }
    g_randomSeed = (g_randomSeed * 1103515245U + 12345U);
    return (g_randomSeed / 65536U) % max; // Kembalikan nilai acak
}

static void handleUserCommand(char* newName) {
    if (newName[0] == '\0') {
        strcpy(g_currentUsername, "user");
        printString("Username changed to user\n");
    } else {
	// Batasi panjang username untuk menghindari overflow
        int k = 0;
        while(newName[k] != '\0' && k < USERNAME_MAX_LEN) {
            g_currentUsername[k] = newName[k];
            k++;
        }
        g_currentUsername[k] = '\0'; 
        
        printString("Username changed to ");
        printString(g_currentUsername);
        printString("\n");
    }
}

// Mengatur warna teks dan suffix sesuai nama Grand Company
static void handleGrandCompanyCommand(char* companyName) {
    byte targetColor = TEXT_COLOR_DEFAULT;
    char* targetSuffix = "";  // literal string
    bool companyFound = false;

    if (strcmp(companyName, "maelstrom") == 0) {
        targetColor = TEXT_COLOR_RED;
        targetSuffix = "@Storm";
        companyFound = true;
    } else if (strcmp(companyName, "twinadder") == 0) {
        targetColor = TEXT_COLOR_YELLOW;
        targetSuffix = "@Serpent";
        companyFound = true;
    } else if (strcmp(companyName, "immortalflames") == 0) {
        targetColor = TEXT_COLOR_BLUE;
        targetSuffix = "@Flame";
        companyFound = true;
    }

    if (companyFound) {
        clearScreen();
        setGlobalTextColor(targetColor); 
        strcpy(g_grandCompanySuffix, targetSuffix);
    } else if (companyName[0] == '\0') {
        printString("Error: Grand Company name required. (maelstrom, twinadder, immortalflames)\n");
    } else {
        printString("Error: Unknown Grand Company '");
        printString(companyName);
        printString("'.\n");
    }
}

static void handleClearCommand() {
    clearScreen();
    setGlobalTextColor(TEXT_COLOR_DEFAULT);
    g_grandCompanySuffix[0] = '\0';
}

//  Mengatur perintah matematika
static void handleMathCommand(char* operation, char* val1Str, char* val2Str) {
    int val1, val2, result;
    char resultBuffer[12]; 

    if (val1Str[0] == '\0' || val2Str[0] == '\0') {
        printString("Math error: Two arguments required.\n");
        return;
    }

    atoi(val1Str, &val1);
    atoi(val2Str, &val2);

    if (strcmp(operation, "add") == 0) {
        result = val1 + val2;
    } else if (strcmp(operation, "sub") == 0) {
        result = val1 - val2;
    } else if (strcmp(operation, "mul") == 0) {
        result = val1 * val2;
    } else if (strcmp(operation, "div") == 0) {
        if (val2 == 0) {
            printString("Math error: Division by zero.\n");
            return;
        }
        result = div(val1, val2); 
    } else {
        printString("Math error: Unknown operation.\n"); 
        return;
    }
    itoa(result, resultBuffer);
    printString(resultBuffer);
    printString("\n");
}

static void handleYogurtCommand() {
    initRandomSeed(); // Pastikan seed sudah diinisialisasi
    int randomIndex = simpleRandom(g_yogurtResponseCount);
    printString(g_yogurtResponses[randomIndex]);
    printString("\n");
}


void parseCommand(char *buf, char *cmd, char arg[2][64]) {
    int i = 0, j = 0;
    cmd[0] = '\0'; arg[0][0] = '\0'; arg[1][0] = '\0';

    while (buf[i] == ' ' && buf[i] != '\0') i++; // Lewati spasi awal
    while (buf[i] != ' ' && buf[i] != '\0') { // Ambil command
        if (j < CMD_BUFFER_SIZE - 1) cmd[j++] = buf[i];
        i++;
    }
    cmd[j] = '\0';
    if (buf[i] == '\0') return;
    while (buf[i] == ' ' && buf[i] != '\0') i++; // Lewati spasi
    if (buf[i] == '\0') return;

    j = 0;
    while (buf[i] != ' ' && buf[i] != '\0') { // Ambil arg1
        if (j < ARG_BUFFER_SIZE - 1) arg[0][j++] = buf[i];
        i++;
    }
    arg[0][j] = '\0';
    if (buf[i] == '\0') return;
    while (buf[i] == ' ' && buf[i] != '\0') i++; // Lewati spasi
    if (buf[i] == '\0') return;
    
    j = 0;
    while (buf[i] != ' ' && buf[i] != '\0') {  // Ambil arg2
        if (j < ARG_BUFFER_SIZE - 1) arg[1][j++] = buf[i];
        i++;
    }
    arg[1][j] = '\0';
}

void shell() {
    char input_buf[INPUT_BUFFER_SIZE];
    char cmd_buf[CMD_BUFFER_SIZE];
    char args_buf[2][ARG_BUFFER_SIZE];
    char prompt_buf[USERNAME_MAX_LEN + SUFFIX_MAX_LEN + 3];

    while (true) {
        strcpy(prompt_buf, g_currentUsername);
        if (g_grandCompanySuffix[0] != '\0') {
            int userLen = 0;
            while(prompt_buf[userLen] != '\0') userLen++;
            strcpy(prompt_buf + userLen, g_grandCompanySuffix);
        }
        int currentLen = 0;
        while(prompt_buf[currentLen] != '\0') currentLen++;
        prompt_buf[currentLen++] = '>';
        prompt_buf[currentLen++] = ' ';
        prompt_buf[currentLen] = '\0';
        printString(prompt_buf);

        readString(input_buf);
        parseCommand(input_buf, cmd_buf, args_buf);

        if (cmd_buf[0] == '\0') { // Jika hanya enter atau input kosong
            if (input_buf[0] != '\0' && input_buf[0] != '\n' && input_buf[0] != '\r') {
                printString(input_buf); // Echo input mentah jika bukan hanya enter
                printString("\n");
            }
            continue;
        }

        if (strcmp(cmd_buf, "user") == 0) {
            handleUserCommand(args_buf[0]);
        } else if (strcmp(cmd_buf, "grandcompany") == 0) {
            handleGrandCompanyCommand(args_buf[0]);
        } else if (strcmp(cmd_buf, "clear") == 0) {
            handleClearCommand();
        } else if (strcmp(cmd_buf, "add") == 0) {
            handleMathCommand("add", args_buf[0], args_buf[1]);
        } else if (strcmp(cmd_buf, "sub") == 0) {
            handleMathCommand("sub", args_buf[0], args_buf[1]);
        } else if (strcmp(cmd_buf, "mul") == 0) {
            handleMathCommand("mul", args_buf[0], args_buf[1]);
        } else if (strcmp(cmd_buf, "div") == 0) {
            handleMathCommand("div", args_buf[0], args_buf[1]);
        } else if (strcmp(cmd_buf, "yogurt") == 0) {
            handleYogurtCommand();
        } else if (strcmp(cmd_buf, "yo") == 0) {
            printString("gurt\n");
        } else if (strcmp(cmd_buf, "gurt") == 0) {
            printString("yo\n");
        } else {
            printString(input_buf); // "The Echo"
            printString("\n");
        }
    }
}
```

#### std_lib.c
```c
#include "std_lib.h" 

//// Jika bool tidak terdefinisi di std_type.h
#ifndef bool
    #define bool char 
    #define true 1
    #define false 0
#endif

#ifndef byte
    #define byte unsigned char
#endif

// // Fungsi pembagian integer tanpa menggunakan operator '/'
int div(int a, int b) {
    int quotient = 0; // Menyimpan hasil pembagian
    int sign = 1; // Menyimpan tanda hasil (+/-)

    if (b == 0) { // // Cegah pembagian dengan nol
        return 0;
    }

    // Atur tanda dan ubah a ke nilai positif jika negatif
    if (a < 0) {
        sign = -sign;
        a = -a; 
    }

    // Atur tanda dan ubah b ke nilai positif jika negatif
    if (b < 0) {
        sign = -sign;
        b = -b; 
    }

   // Proses pembagian dengan pengurangan berulang
    while (a >= b) {
        a -= b;
        quotient++;
    }

    return sign * quotient;
}

// Fungsi modulus menggunakan div()
int mod(int a, int b) {
    if (b == 0) {
        return a;
    }
    return a - (div(a, b) * b); // a mod b = a - (a / b) * b
}

// Fungsi membandingkan dua string
bool strcmp(char *str1, char *str2) {
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return false; 
        }
        str1++;
        str2++;
    }
   
    return (*str1 == *str2);
}

// Fungsi menyalin string dari src ke dst
void strcpy(char *dst, char *src) {
    while (*src != '\0') {
        *dst = *src;
        dst++;
        src++;
    }
    *dst = '\0'; 
}

// Fungsi membersihkan buffer dengan 0x00
void clear(byte *buf, unsigned int size) {
    unsigned int i;
    for (i = 0; i < size; i++) {
        buf[i] = 0x00;
    }
}

// Fungsi mengubah string ke integer
void atoi(char *str, int *num) {
    int result = 0;
    int sign = 1;
    int i = 0;

   // Cek apakah string mengandung tanda - atau +
    if (str[0] == '-') {
        sign = -1;
        i++;
    } else if (str[0] == '+') {
        i++;
    }

   // Ubah karakter digit menjadi angka
    while (str[i] != '\0') {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        } else {
            break; // Jika bukan digit, berhenti
        }
        i++;
    }
    *num = result * sign;
}

// Fungsi mengubah integer ke string
void itoa(int num, char *str) {
    int i = 0;
    bool isNegative = false;
    int start_index_digits = 0;

    // Kasus khusus: angka 0
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Tangani bilangan negatif
    if (num < 0) {
        isNegative = true;
        str[i++] = '-'; // Tambahkan tanda minus di depan
        start_index_digits = 1; // Angka mulai dari index ke-1
    }

    int n_temp = num; 
    int current_digit_idx = start_index_digits;

    // Jika negatif, konversi digit dengan mengurangkan dari '0'
    if (n_temp < 0) { 
        while (n_temp != 0) {
            str[current_digit_idx++] = '0' - (n_temp % 10); 
            n_temp /= 10;
        }
    } else { 
         while (n_temp != 0) {
            str[current_digit_idx++] = (n_temp % 10) + '0';
            n_temp /= 10;
        }
    }
    str[current_digit_idx] = '\0'; 

    // Balik digit (karena menyusun dari belakang)
    int j = current_digit_idx - 1;
    int k = start_index_digits;
    while (k < j) {
        char temp = str[k];
        str[k] = str[j];
        str[j] = temp;
        k++;
        j--;
    }
}
```

#### makefile
```bash
# Compiler dan Linker
AS = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy

SRC_DIR = src
INC_DIR = include
BIN_DIR = bin

# Pastikan direktori output ada
$(shell mkdir -p $(BIN_DIR))

# Compiler/Assembler Flags
ASFLAGS_BOOT = -f bin

# Untuk NASM 
ASFLAGS_KERNEL_ASM = -f elf

# Untuk GCC 
# -I$(INC_DIR) memberitahu GCC untuk mencari header di direktori include/
CFLAGS = -m32 -ffreestanding -nostdlib -Wall -Wextra -c -I$(INC_DIR)

# Untuk LD (link sebagai 32-bit ELF, set kernel entry point/text segment start)
LDFLAGS_KERNEL = -m elf_i386 -Ttext 0x1000 -nostdlib
# Untuk OBJCOPY (konversi ELF ke biner mentah)
OBJCOPYFLAGS = -O binary

# Nama File Output Utama
BOOTLOADER_BIN = $(BIN_DIR)/bootloader.bin
KERNEL_ELF = $(BIN_DIR)/kernel.elf
KERNEL_BIN = $(BIN_DIR)/kernel.bin
FLOPPY_IMG = $(BIN_DIR)/floppy.img

# File Objek
STD_LIB_O = $(BIN_DIR)/std_lib.o
SHELL_O = $(BIN_DIR)/shell.o
KERNEL_C_O = $(BIN_DIR)/kernel.o
KERNEL_ASM_O = $(BIN_DIR)/kernel_asm.o

KERNEL_OBJECTS = $(KERNEL_C_O) $(KERNEL_ASM_O) $(STD_LIB_O) $(SHELL_O)

# Phony targets
.PHONY: all clean prepare bootloader_build stdlib_build shell_build kernel_c_build kernel_asm_build kernel_build link_elf convert_to_bin create_image build

# Default target: running 'make' akan menjalankan 'make all' yang menjalankan 'make build'
all: build

# Tahapan Build Utama

# Target 'build' utama yang bergantung pada image floppy akhir
build: $(FLOPPY_IMG)
	@echo "===> EorzeOS build completed successfully!"
	@echo "===> Final image: $(FLOPPY_IMG)"

# Membuat image floppy akhir
$(FLOPPY_IMG): $(BOOTLOADER_BIN) $(KERNEL_BIN)
	@echo "===> Creating final floppy image: $(FLOPPY_IMG)..."
	cat $(BOOTLOADER_BIN) $(KERNEL_BIN) > $(FLOPPY_IMG)

# Membuat file disk image kosong awal (opsional jika 'cat' selalu overwrite)
# Bisa juga digunakan sebagai dependency jika diperlukan oleh langkah lain
prepare:
	@echo "===> Preparing empty floppy disk image (1.44MB)..."
	dd if=/dev/zero of=$(FLOPPY_IMG) bs=512 count=2880 status=none

# Mengompilasi bootloader
$(BOOTLOADER_BIN): $(SRC_DIR)/bootloader.asm
	@echo "===> Assembling bootloader: $< -> $@"
	$(AS) $(ASFLAGS_BOOT) $< -o $@

# Mengompilasi std_lib.c
$(STD_LIB_O): $(SRC_DIR)/std_lib.c $(INC_DIR)/std_lib.h $(INC_DIR)/std_type.h
	@echo "===> Compiling stdlib: $< -> $@"
	$(CC) $(CFLAGS) $< -o $@

# Mengompilasi shell.c
$(SHELL_O): $(SRC_DIR)/shell.c $(INC_DIR)/shell.h $(INC_DIR)/kernel.h $(INC_DIR)/std_lib.h $(INC_DIR)/std_type.h
	@echo "===> Compiling shell: $< -> $@"
	$(CC) $(CFLAGS) $< -o $@

# Mengompilasi kernel.c
$(KERNEL_C_O): $(SRC_DIR)/kernel.c $(INC_DIR)/kernel.h $(INC_DIR)/shell.h $(INC_DIR)/std_type.h
	@echo "===> Compiling kernel C code: $< -> $@"
	$(CC) $(CFLAGS) $< -o $@

# Mengompilasi kernel.asm
$(KERNEL_ASM_O): $(SRC_DIR)/kernel.asm
	@echo "===> Assembling kernel ASM code: $< -> $@"
	$(AS) $(ASFLAGS_KERNEL_ASM) $< -o $@

# Target untuk kernel.elf (hasil linking sebelum konversi ke biner mentah)
$(KERNEL_ELF): $(KERNEL_OBJECTS)
	@echo "===> Linking kernel object files into ELF: $@"
	$(LD) $(LDFLAGS_KERNEL) -o $@ $(KERNEL_OBJECTS)

# Target untuk kernel.bin (biner mentah)
$(KERNEL_BIN): $(KERNEL_ELF)
	@echo "===> Converting ELF to raw binary: $< -> $@"
	$(OBJCOPY) $(OBJCOPYFLAGS) $< $@


# --- Target "Perintah" Sesuai Permintaan Awal Anda (opsional, bisa dihapus jika 'make build' cukup) ---
# Ini adalah target .PHONY yang menjalankan sub-bagian dari build jika dipanggil manual.
# 'make build' sudah mencakup semuanya secara otomatis karena dependensi file.

bootloader_build: $(BOOTLOADER_BIN)
stdlib_build: $(STD_LIB_O)
shell_build: $(SHELL_O)
kernel_c_build: $(KERNEL_C_O)
kernel_asm_build: $(KERNEL_ASM_O)
kernel_build: kernel_c_build kernel_asm_build # Atau langsung $(KERNEL_C_O) $(KERNEL_ASM_O)
link_elf: $(KERNEL_ELF)
convert_to_bin: $(KERNEL_BIN)
create_image: $(FLOPPY_IMG)

# --- Clean Target ---
clean:
	@echo "===> Cleaning build directory: $(BIN_DIR)..."
	rm -rf $(BIN_DIR)
	@echo "===> Clean complete."
```



