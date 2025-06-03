#include "shell.h"
#include "kernel.h"
#include "std_lib.h"
#include "std_type.h" 

// --- Konstanta ---
#define INPUT_BUFFER_SIZE 128
#define CMD_BUFFER_SIZE 64
#define ARG_BUFFER_SIZE 64
#define USERNAME_MAX_LEN 15
#define SUFFIX_MAX_LEN 15

// Definisi Warna (contoh, sesuaikan dengan implementasi kernel Anda)
#define TEXT_COLOR_DEFAULT 0x07
#define TEXT_COLOR_RED     0x04
#define TEXT_COLOR_YELLOW  0x0E
#define TEXT_COLOR_BLUE    0x01


// --- Variabel Global (File-Static) ---
static char g_currentUsername[USERNAME_MAX_LEN + 1] = "user";
static char g_grandCompanySuffix[SUFFIX_MAX_LEN + 1] = "";
static char* g_yogurtResponses[] = {
    "yo",
    "ts unami gng </3",
    "sygau"
};
static int g_yogurtResponseCount = 3; // Sesuai jumlah respons di atas
static unsigned int g_randomSeed = 0;

// --- Definisi Fungsi Helper (File-Static) ---
static void initRandomSeed() {
    if (g_randomSeed == 0) {
        g_randomSeed = getBiosTick(); 
    }
}

static int simpleRandom(int max) {
    if (max <= 0) return 0; 
    if (g_randomSeed == 0) {
        initRandomSeed();
    }
    g_randomSeed = (g_randomSeed * 1103515245U + 12345U);
    return (g_randomSeed / 65536U) % max; 
}

static void handleUserCommand(char* newName) {
    if (newName[0] == '\0') {
        strcpy(g_currentUsername, "user");
        printString("Username changed to user\n");
    } else {
  
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

static void handleGrandCompanyCommand(char* companyName) {
    byte targetColor = TEXT_COLOR_DEFAULT;
    char* targetSuffix = ""; 
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
        result = div(val1, val2); // Menggunakan fungsi div dari std_lib.c
    } else {
        printString("Math error: Unknown operation.\n"); // Seharusnya tidak terjadi
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
    while (buf[i] != ' ' && buf[i] != '\0') { // Ambil arg2
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

    // initRandomSeed(); // Bisa dipanggil sekali di awal shell, atau di handleYogurtCommand

    while (true) {
        // Bentuk prompt
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
