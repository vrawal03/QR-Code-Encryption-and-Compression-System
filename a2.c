#include "a2.h"

// int malloc_count = 0;
// int malloc_binary_num_count = 0;
// int malloc_xor_encrypt_count = 0;
// int free_count_a2 = 0;

int bitwise_xor(int value){ // taking in int, outputting int

    int key_ascii_val = (int)KEY;
    int xor_done = key_ascii_val^value;
    return xor_done;
}

char* xor_encrypt(char c){ // taking in char, outputting char* (pointer to string)

    int key_ascii_val = (int)KEY;
    int c_ascii_val = (int)c;
    int xor_val = (int)key_ascii_val^c_ascii_val; // All this stays constant

    int binary_xor_val = 0;

    char* output = (char*)malloc(sizeof(char)*8);
    // printf("MALLOC for xor_encrypt output\n");
    // malloc_count++;
    // malloc_xor_encrypt_count++;
    for (int i=0; i<7; i++) {
        if (xor_val - pow(2, (6-i)) < 0) {
            // *output &= ~(1<<i);
            *(output + i) = '0';
        } else {
            // *output |= 1<<i;
            *(output + i) = '1';
            xor_val = xor_val - pow(2, (6-i));
        }
    }

    *(output + 7) = '\0'; // Setting final char in string to null

    return output;
}

char xor_decrypt(char *s){ 

    int base_ten = 0;
    for (int i=0; i<7; i++) {
        // if (*s & 1<<i) {
        if (*(s+i) == '1') { // Should be the only thing we have to change, since key_ascii_val is constant
            base_ten = base_ten + pow(2, (6-i));
        }
    }

    int key_ascii_val = (int)KEY;
    int xor_result = base_ten^key_ascii_val;
    char decrypted = (char)xor_result;

    return decrypted;
}

char *gen_code(char *msg){

    char* encrypted_code = (char*)malloc(sizeof(char)*257); // Extra for null character
    // printf("MALLOC\n");
    // malloc_count++;
    for (int i=0; i<256; i++) {
        *(encrypted_code + i) = '0';
    }
    *(encrypted_code + 256) = '\0'; // This stays the same - we're simply initializing the return string.

    int constant_one[] = {0, 1, 2, 3, 4, 11, 12, 13, 14, 15, 16, 20, 27, 31, 32, 34, 36, 43, 45, 47, 48, 52, 59, 63, 64, 65, 
    66, 67, 68, 75, 76, 77, 78, 79, 176, 177, 178, 179, 180, 192, 196, 208, 210, 212, 224, 228, 240, 241, 242, 243, 244, 255};

    int constant_zero[] = {17, 18, 19, 28, 29, 30, 33, 35, 44, 46, 49, 50, 51, 60, 61, 62, 193, 194, 195, 209, 211, 225, 226, 
    227};

    int combined_constants[] = {0, 1, 2, 3, 4, 11, 12, 13, 14, 15, 16, 20, 27, 31, 32, 34, 36, 43, 45, 47, 48, 52, 59, 63, 64, 
    65, 66, 67, 68, 75, 76, 77, 78, 79, 176, 177, 178, 179, 180, 192, 196, 208, 210, 212, 224, 228, 240, 241, 242, 243, 244, 
    17, 18, 19, 28, 29, 30, 33, 35, 44, 46, 49, 50, 51, 60, 61, 62, 193, 194, 195, 209, 211, 225, 226, 227, 255};

    int size_of_c1 = (int)(sizeof(constant_one))/(sizeof(constant_one[0]));
    int size_of_c2 = (int)(sizeof(constant_zero))/(sizeof(constant_zero[0]));
    int size_of_cc = (int)(sizeof(combined_constants))/(sizeof(combined_constants[0]));

    for (int i=0; i<size_of_c1; i++) {
        *(encrypted_code + constant_one[i]) = '1';
    }

    for (int j=0; j<size_of_c2; j++) {
        *(encrypted_code + constant_zero[j]) = '0';
    }

    int size_of_string = 0;
    while (*(msg+size_of_string) != '\0') {
        size_of_string++;
    } 

    int placeholder = 0;
    // char* binary_num = (char*)malloc(sizeof(char)*8);
    char* binary_num;
    // printf("MALLOC for binary_num\n");
    // malloc_count++;
    // malloc_binary_num_count++;

    // Next mission is to figure out WHY binary_num is leaking 1 byte.
    // Reminder: the error message is telling us that we haven't freed
    // All the mallocs that we've allocated.
    
    for (int i=0; i<=size_of_string; i++) {

        if (placeholder <= 255) {
            binary_num = xor_encrypt(*(msg+i));
            // printf("MALLOC for xor_encrypt \n");
            // malloc_count++;

            for (int j=0; j<7; j++) {
                int bool = 0; // we assume we have to move forward
                int breakbool = 0;
                while (bool == 0) {
                    placeholder++;
                    for (int i=0; i<size_of_cc; i++) {
                        if (placeholder == combined_constants[i]) {
                            breakbool = 1;
                            break; // we have to move forward
                        }
                    }
                    if (breakbool == 0) {
                        bool = 1;
                    } else {
                        breakbool = 0;
                        bool = 0;
                    }
                }

                encrypted_code[placeholder] = *(binary_num + j);
            }

            if (binary_num != NULL) {
                free(binary_num);
                // printf("FREE for binary_num\n");
                // free_count_a2++;
                binary_num = NULL;
            }
        }

        if (binary_num) {
            free(binary_num);
            // printf("FREE for binary_num\n");
            // free_count_a2++;
            binary_num = NULL;
        }
    }

    if (binary_num != NULL) {
        free(binary_num);
        // printf("FREE for binary_num\n");
        // free_count_a2++;
        binary_num = NULL;
    }

    return encrypted_code;
}

char *read_code(char *code) {

    int constant_one[] = {0, 1, 2, 3, 4, 11, 12, 13, 14, 15, 16, 20, 27, 31, 32, 34, 36, 43, 45, 47, 48, 52, 59, 63, 64, 65, 
    66, 67, 68, 75, 76, 77, 78, 79, 176, 177, 178, 179, 180, 192, 196, 208, 210, 212, 224, 228, 240, 241, 242, 243, 244, 255};

    int constant_zero[] = {17, 18, 19, 28, 29, 30, 33, 35, 44, 46, 49, 50, 51, 60, 61, 62, 193, 194, 195, 209, 211, 225, 226, 
    227};

    int combined_constants[] = {0, 1, 2, 3, 4, 11, 12, 13, 14, 15, 16, 20, 27, 31, 32, 34, 36, 43, 45, 47, 48, 52, 59, 63, 64, 
    65, 66, 67, 68, 75, 76, 77, 78, 79, 176, 177, 178, 179, 180, 192, 196, 208, 210, 212, 224, 228, 240, 241, 242, 243, 244, 
    17, 18, 19, 28, 29, 30, 33, 35, 44, 46, 49, 50, 51, 60, 61, 62, 193, 194, 195, 209, 211, 225, 226, 227, 255};

    int size_of_c1 = (int)(sizeof(constant_one))/(sizeof(constant_one[0]));
    int size_of_c2 = (int)(sizeof(constant_zero))/(sizeof(constant_zero[0]));
    int size_of_cc = (int)(sizeof(combined_constants))/(sizeof(combined_constants[0]));

    char* decrypted_code = (char*)malloc(sizeof(char)*26);
    // printf("MALLOC\n");
    // malloc_count++;

    int message_end = 0; // This will switch to 1 once we encounter a null char
    int placeholder = 0;
    int curr_num_chars = 0;

    char* each_character = (char*)malloc(sizeof(char)*8); // Each set of 7 digits
    // printf("MALLOC\n");
    // malloc_count++;

    for (int i=0; i<7; i++) {
        *(each_character + i) = '0';
    }

    *(each_character + 7) = '\0';

    while (message_end == 0 && placeholder < 256) {

        for (int i=0; i<7; i++) {
            int bool = 0; // we assume we have to move forward
            int breakbool = 0;
            while (bool == 0) {
                placeholder++;
                // printf("Placeholder: %d\n", placeholder);
                for (int i=0; i<size_of_cc; i++) {
                    if (placeholder == combined_constants[i]) {
                        breakbool = 1;
                        break; // we have to move forward
                    }
                }
                if (breakbool == 0) {
                    bool = 1;
                } else {
                    breakbool = 0;
                    bool = 0;
                }
            }

            *(each_character + i) = *(code + placeholder);
        }

        char real_character = xor_decrypt(each_character);

        if (real_character == '\0') {
            message_end = 1;
        }

        *(decrypted_code + curr_num_chars) = real_character;
        curr_num_chars++;

        // printf("Reading in character: %c\n", real_character);
    }

    free(each_character);
    // printf("FREE\n");
    // free_count_a2++;
    return decrypted_code;
} 

char *compress(char *code){

    char* compressed_code = (char*)malloc(sizeof(char)*65);
    // printf("MALLOC\n");
    // malloc_count++;

    for (int i=0; i<64; i++) {
        *(compressed_code + i) = '0';
    }

    *(compressed_code + 64) = '\0';

    int message_end = 0; // Again, this will switch to 1 once we encounter a null char.
    int placeholder = 0;
    int curr_num_chars = 0;
    int base_ten_null_count = 0;

    char* each_character = (char*)malloc(sizeof(char)); // Each set of 4 digits
    // printf("MALLOC\n");
    // malloc_count++;
    for (int i=0; i<8; i++) {
        *each_character &= ~(0<<i);
    }

    while (curr_num_chars < 64) {

        for (int i=0; i<4; i++) {

            if (*(code + placeholder) == '1') {
                *each_character |= 1<<i; // Setting the ith bit to 1.
            } else if (*(code + placeholder) == '0') {
                *each_character &= ~(1<<i); // Setting the ith bit to 0.
            } else {
                // printf("Either we're done or something's wrong.");
            }

            placeholder++;
        }

        int base_ten = 0;
        for (int i=0; i<4; i++) {
            if (*each_character & 1<<i) {
                base_ten = base_ten + pow(2, (3-i));
            }
        }

        char hex_char;
        if (base_ten < 10) {
            hex_char = (char)(base_ten + '0');
        } else {
            hex_char = (char)(base_ten + '7');
        }

        *(compressed_code + curr_num_chars) = hex_char;
        curr_num_chars++;

        if (base_ten == 0) {
            base_ten_null_count ++;
        } else {
            base_ten_null_count = 0;
        } 

        if (base_ten_null_count > 1 || curr_num_chars >= 64) {
            message_end = 1;
        }
    }

    free(each_character);
    // printf("FREE\n");
    // free_count_a2++;
    return compressed_code; // Works for now, test later
}

char *decompress(char *code){

    char* decompressed_code = (char*)malloc(sizeof(char)*257);
    // printf("MALLOC\n");
    // malloc_count++;

    for (int i=0; i<256; i++) {
        *(decompressed_code + i) = '0';
    }
    *(decompressed_code + 256) = '\0';

    char curr_char;

    int placeholder = 0; 

    for (int i=0; i<64; i++) {
        curr_char = *(code + i);

        int converted_int = 0;
        if ((int)curr_char - 48 < 10) {
            int ascii = (int)curr_char - 48;
            for (int i=0; i<4; i++) {
                if (ascii - pow(2, (3-i)) >= 0) {
                    ascii = ascii - pow(2, (3-i));
                    converted_int = converted_int + pow(10, (3-i));
                }
            }
        } else {
            int ascii = (int)curr_char - 55;
            for (int i=0; i<4; i++) {
                if (ascii - pow(2, (3-i)) >= 0) {
                    ascii = ascii - pow(2, (3-i));
                    converted_int = converted_int + pow(10, (3-i));
                }
            }
        }

        char four_digits[5];
        sprintf(four_digits, "%04d", converted_int);

        for (int i=0; i<4; i++) {
            *(decompressed_code + placeholder) = four_digits[i];
            placeholder++;
        }
        
    }

    return decompressed_code;
}

// int callcounter = 0;

int min(int a, int b, int c) {
    if (a <= b && a <= c) {
        return a;
    }
    if (b <= a && b <= c) {
        return b;
    }
    return c;
}

// We can try storing a pointer to the memory in some kind of data structure
// After 

int callcounter = 0;

char *substring(char* string) {
    // Specifically, we want the substring from index 1 to n
    // printf("Callcounter: %d\n", callcounter);
    char* output = (char*)malloc(sizeof(char)*(strlen(string)));
    for (int i=0; i<strlen(string); i++) {
        *(output + i) = '\0';
    }
    strncpy(output, string + 1, strlen(string) - 1);
    /*
    if (string != NULL && callcounter != 0) {
        free(string);
        string = NULL;
    } 
    if (callcounter == 0) {
        callcounter++;
    } */
    // free(string);
    // printf("%s\n", output);

    return output;
}

int calc_ld(char *sandy, char *cima) {

    int m = strlen(sandy);
    int n = strlen(cima);
    int array[n+1][m+1];

    array[0][0] = 0;

    for (int i=0; i<n; i++)
        array[i+1][0] = array[i][0] + 1;
        
    for (int j=0; j<m; j++)
        array[0][j+1] = array[0][j] + 1;

    for (int i=0; i<n; i++)
        for (int j=0; j<m; j++)
            if (sandy[j] == cima[i]) {
                array[i+1][j+1] = min(array[i][j+1] + 1, array[i+1][j] + 1, array[i][j]);
            } else {
                array[i+1][j+1] = min(array[i][j+1] + 1, array[i+1][j] + 1, array[i][j] + 1);
            }

    return(array[n][m]);
}   
