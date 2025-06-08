#include "std_lib.h" 

#ifndef bool
    #define bool char 
    #define true 1
    #define false 0
#endif

#ifndef byte
    #define byte unsigned char
#endif

int div(int a, int b) {
    int quotient = 0;
    int sign = 1;

    if (b == 0) {    
        return 0;
    }

    if (a < 0) {
        sign = -sign;
        a = -a; 
    }
    if (b < 0) {
        sign = -sign;
        b = -b; 
    }

   
    while (a >= b) {
        a -= b;
        quotient++;
    }

    return sign * quotient;
}

int mod(int a, int b) {
    if (b == 0) {
        return a;
    }
    return a - (div(a, b) * b);
}

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

void strcpy(char *dst, char *src) {
    while (*src != '\0') {
        *dst = *src;
        dst++;
        src++;
    }
    *dst = '\0'; 
}

void clear(byte *buf, unsigned int size) {
    unsigned int i;
    for (i = 0; i < size; i++) {
        buf[i] = 0x00;
    }
}

void atoi(char *str, int *num) {
    int result = 0;
    int sign = 1;
    int i = 0;

   
    if (str[0] == '-') {
        sign = -1;
        i++;
    } else if (str[0] == '+') {
        i++;
    }

  
    while (str[i] != '\0') {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        } else {
      
            break;
        }
        i++;
    }
    *num = result * sign;
}

void itoa(int num, char *str) {
    int i = 0;
    bool isNegative = false;
    int start_index_digits = 0;

  
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (num < 0) {
        isNegative = true;
    
        str[i++] = '-';
        start_index_digits = 1; 
    }

    int n_temp = num; 
    if (isNegative) {
        
    } else {
        
    }

    int current_digit_idx = start_index_digits;
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
