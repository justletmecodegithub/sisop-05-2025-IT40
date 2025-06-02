#include "shell.h"
#include "kernel.h"   // Untuk printString, readString, clearScreen, getBiosTick, setGlobalTextColor (asumsi)
#include "std_lib.h"  // Untuk strcpy, strcmp, atoi, itoa

// Asumsi std_type.h (untuk 'byte') di-include oleh salah satu header di atas atau tersedia global.
// Jika tidak, Anda mungkin perlu menambahkannya atau mendefinisikan 'byte' di sini.
#ifndef byte
typedef unsigned char byte;
#endif

// --- Konstanta ---
#define CMD_BUFFER_SIZE 64
#define ARG_BUFFER_SIZE 64    // Sesuai dengan parseCommand arg[2][64]
#define USERNAME_MAX_LEN 15   // Untuk buffer 16 byte (char + null)
#define SUFFIX_MAX_LEN 15     // Untuk buffer 16 byte

// Definisi Warna (sesuaikan dengan implementasi kernel Anda)
#define TEXT_COLOR_DEFAULT 0x07 // Putih pada Hitam
#define TEXT_COLOR_RED     0x04 // Merah pada Hitam
#define TEXT_COLOR_YELLOW  0x0E // Kuning pada Hitam
#define TEXT_COLOR_BLUE    0x01 // Biru pada Hitam (atau 0x09 untuk Biru Terang)

// --- Variabel Global (File-Static) ---
static char g_currentUsername[USERNAME_MAX_LEN + 1] = "user";
static char g_grandCompanySuffix[SUFFIX_MAX_LEN + 1] = "";

// Untuk fitur 'yogurt'
static char* g_yogurtResponses[] = {
    "yo",
    "ts unami gng </3",
    "sygau"
};
static int g_yogurtResponseCount = 3;
static unsigned int g_randomSeed = 0;

// --- Fungsi Helper ---

/**
 * @brief Inisialisasi seed untuk generator angka acak sederhana menggunakan BIOS tick.
 */
static void initRandomSeed() {
    if (g_randomSeed == 0) {
        g_randomSeed = getBiosTick();
    }
}

/**
 * @brief Generator angka acak sederhana (Linear Congruential Generator).
 * @param max Batas atas eksklusif untuk angka acak (hasil akan dari 0 hingga max-1).
 * @return Angka acak antara 0 dan max-1.
 */
static int simpleRandom(int max) {
    if (max <= 0) return 0;
    if (g_randomSeed == 0) {
        initRandomSeed();
    }
    g_randomSeed = (g_randomSeed * 1103515245U + 12345U);
    return (g_randomSeed / 65536U) % max;
}

/**
 * @brief Menangani perintah 'user'.
 * @param newName Nama baru untuk user, atau string kosong jika hanya 'user'.
 */
static void handleUserCommand(char* newName) {
    if (newName[0] == '\0') {
        strcpy(g_currentUsername, "user");
        printString("Username changed to user\n");
    } else {
        // Batasi panjang username
        int len = 0;
        char tempName[USERNAME_MAX_LEN + 1];
        while(newName[len] != '\0' && len < USERNAME_MAX_LEN) {
            tempName[len] = newName[len];
            len++;
        }
        tempName[len] = '\0';
        strcpy(g_currentUsername, tempName);

        printString("Username changed to ");
        printString(g_currentUsername);
        printString("\n");
    }
}

/**
 * @brief Menangani perintah 'grandcompany'.
 * @param companyName Nama Grand Company.
 */
static void handleGrandCompanyCommand(char* companyName) {
    byte targetColor = TEXT_COLOR_DEFAULT;
    char* targetSuffix = ""; // Pointer ke literal string, aman.
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

/**
 * @brief Menangani perintah 'clear'.
 */
static void handleClearCommand() {
    clearScreen();
    setGlobalTextColor(TEXT_COLOR_DEFAULT);
    g_grandCompanySuffix[0] = '\0';
}

/**
 * @brief Menangani perintah kalkulator.
 * @param operation String operasi.
 * @param val1Str Operand pertama (string).
 * @param val2Str Operand kedua (string).
 */
static void handleMathCommand(char* operation, char* val1Str, char* val2Str) {
    if (val1Str[0] == '\0' || val2Str[0] == '\0') {
        printString("Error: Math operations require two arguments.\n");
        return;
    }

    int val1 = atoi(val1Str);
    int val2 = atoi(val2Str);
    int result = 0;
    bool validOp = true;
    char resultBuffer[12]; // Cukup untuk int 32-bit + tanda + null

    if (strcmp(operation, "add") == 0) {
        result = val1 + val2;
    } else if (strcmp(operation, "sub") == 0) {
        result = val1 - val2;
    } else if (strcmp(operation, "mul") == 0) {
        result = val1 * val2;
    } else if (strcmp(operation, "div") == 0) {
        if (val2 == 0) {
            printString("Error: Division by zero.\n");
            validOp = false;
        } else {
            result = val1 / val2;
        }
    } else {
        validOp = false; // Should not happen
    }

    if (validOp) {
        itoa(result, resultBuffer);
        printString(resultBuffer);
        printString("\n");
    }
}

/**
 * @brief Menangani perintah 'yogurt'.
 */
static void handleYogurtCommand() {
    int randomIndex = simpleRandom(g_yogurtResponseCount);
    printString(g_yogurtResponses[randomIndex]);
    printString("\n");
}


// --- Implementasi Fungsi dari Template ---

/**
 * @brief Mem-parsing input string menjadi command dan hingga dua argumen.
 * @param buf String input dari pengguna.
 * @param cmd Buffer output untuk menyimpan command.
 * @param arg Array 2D buffer output untuk menyimpan argumen [0] dan argumen [1].
 */
void parseCommand(char *buf, char *cmd, char arg[2][64]) {
    int i = 0; // Indeks untuk buf
    int j = 0; // Indeks untuk cmd/arg

    // Bersihkan buffer output
    cmd[0] = '\0';
    arg[0][0] = '\0';
    arg[1][0] = '\0';

    // Lewati spasi di awal
    while (buf[i] == ' ' && buf[i] != '\0') {
        i++;
    }

    // Ekstrak command
    j = 0;
    while (buf[i] != ' ' && buf[i] != '\0') {
        if (j < CMD_BUFFER_SIZE - 1) { // Pastikan tidak overflow cmd buffer
            cmd[j++] = buf[i];
        }
        i++;
    }
    cmd[j] = '\0';

    // Jika tidak ada argumen lagi
    if (buf[i] == '\0') {
        return;
    }

    // Lewati spasi setelah command
    while (buf[i] == ' ' && buf[i] != '\0') {
        i++;
    }
     if (buf[i] == '\0') { // Hanya command, tidak ada argumen
        return;
    }


    // Ekstrak argumen pertama (arg[0])
    j = 0;
    while (buf[i] != ' ' && buf[i] != '\0') {
        if (j < ARG_BUFFER_SIZE - 1) { // Pastikan tidak overflow arg buffer
            arg[0][j++] = buf[i];
        }
        i++;
    }
    arg[0][j] = '\0';

    // Jika tidak ada argumen kedua lagi
    if (buf[i] == '\0') {
        return;
    }

    // Lewati spasi setelah argumen pertama
    while (buf[i] == ' ' && buf[i] != '\0') {
        i++;
    }
    if (buf[i] == '\0') { // Hanya satu argumen
        return;
    }

    // Ekstrak argumen kedua (arg[1])
    j = 0;
    while (buf[i] != ' ' && buf[i] != '\0') {
        if (j < ARG_BUFFER_SIZE - 1) { // Pastikan tidak overflow arg buffer
            arg[1][j++] = buf[i];
        }
        i++;
    }
    arg[1][j] = '\0';
}


/**
 * @brief Fungsi utama loop shell EorzeOS.
 */
void shell() {
    char buf[128];                     // Buffer input dari pengguna
    char cmd_buf[CMD_BUFFER_SIZE];     // Buffer untuk menyimpan command yang diparsing
    char args_buf[2][ARG_BUFFER_SIZE]; // Buffer untuk menyimpan argumen yang diparsing

    printString("Welcome to EorzeOS!\n");
    setGlobalTextColor(TEXT_COLOR_DEFAULT); // Atur warna teks default awal
    initRandomSeed();                       // Inisialisasi seed untuk 'yogurt'

    while (true) {
        // Bentuk dan tampilkan prompt dinamis
        char promptBuffer[USERNAME_MAX_LEN + SUFFIX_MAX_LEN + 3]; // username + suffix + "> " + null
        promptBuffer[0] = '\0';

        strcpy(promptBuffer, g_currentUsername);
        if (g_grandCompanySuffix[0] != '\0') {
            // Implementasi strcat sederhana jika tidak ada di std_lib.h
            int userLen = 0;
            while(promptBuffer[userLen] != '\0') userLen++; // cari akhir dari username
            strcpy(promptBuffer + userLen, g_grandCompanySuffix);
        }
        
        int currentLen = 0;
        while(promptBuffer[currentLen] != '\0') currentLen++;
        promptBuffer[currentLen++] = '>';
        promptBuffer[currentLen++] = ' ';
        promptBuffer[currentLen] = '\0';
        printString(promptBuffer);

        // Baca input dari pengguna
        readString(buf);

        // Parse command dan argumen
        parseCommand(buf, cmd_buf, args_buf);

        // Proses command
        if (cmd_buf[0] == '\0') { // Jika tidak ada command (misalnya hanya Enter)
            if (buf[0] != '\0' && buf[0] != '\n' && buf[0] != '\r') { 
                // Jika ada input non-kosong yang tidak menghasilkan command (misal hanya spasi)
                // "The Echo" untuk input mentah yang tidak menghasilkan command
                printString(buf);
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
            // "The Echo" untuk command tidak dikenal
            printString(buf); // Cetak input mentah
            printString("\n");
        }
    }
}
