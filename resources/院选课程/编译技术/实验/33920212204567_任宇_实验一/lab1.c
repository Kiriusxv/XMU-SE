#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    IDENTIFIER, DECIMAL_INTEGER, OCTAL_INTEGER, HEXADECIMAL_INTEGER, FLOAT,
    OPERATOR, KEYWORD_IF, KEYWORD_THEN, KEYWORD_ELSE, KEYWORD_WHILE, KEYWORD_DO,
    UNRECOGNIZED
} TokenType;

// ����Ƿ�Ϊ�ؼ���
TokenType checkKeyword(const char* str) {
    if (strcmp(str, "if") == 0) return KEYWORD_IF;
    if (strcmp(str, "then") == 0) return KEYWORD_THEN;
    if (strcmp(str, "else") == 0) return KEYWORD_ELSE;
    if (strcmp(str, "while") == 0) return KEYWORD_WHILE;
    if (strcmp(str, "do") == 0) return KEYWORD_DO;
    return IDENTIFIER;
}

// ���ʻ㵥Ԫ����ת��Ϊ�ַ���
const char* TokenTypeToString(TokenType type) {
    switch (type) {
    case IDENTIFIER: return "IDN";
    case DECIMAL_INTEGER: return "INT10";
    case OCTAL_INTEGER: return "INT8";
    case HEXADECIMAL_INTEGER: return "INT16";
    case FLOAT: return "FLOAT";
    case OPERATOR: return "OPR";
    case KEYWORD_IF:
    case KEYWORD_THEN:
    case KEYWORD_ELSE:
    case KEYWORD_WHILE:
    case KEYWORD_DO: return "KEYWORD";
    default: return "UNRECOGNIZED";
    }
}

typedef struct {
    TokenType type;
    char text[100];
} Token;

// ɨ�����벢����һ���ʻ㵥Ԫ
Token scan(const char** input) {
    Token token = { UNRECOGNIZED, {0} };

    while (isspace(**input)) (*input)++;  // ����ǰ���հ��ַ�

    const char* start = *input;  // ��¼��ʼλ��

    if (isalpha(**input) || **input == '_') { // ��ʶ����ؼ���
        while (isalnum(**input) || **input == '_' || **input == '.') (*input)++;
        strncpy(token.text, start, *input - start);
        token.text[*input - start] = '\0';
        token.type = checkKeyword(token.text);
    }
    else if (isdigit(**input) || (**input == '.' && isdigit(*(*input + 1)))) { // ���ֻ򸡵���
        bool isFloat = false;
        if (**input == '0') {
            start = *input;
            (*input)++;
            if (**input == 'x' || **input == 'X') { // ʮ������
                (*input)++;
                while (isxdigit(**input) || **input == '.') {
                    if (**input == '.') isFloat = true;
                    (*input)++;
                }
                token.type = isFloat ? FLOAT : HEXADECIMAL_INTEGER;
            }
            else if (**input == 'o' || **input == 'O') { // �˽���
                (*input)++;
                while (**input >= '0' && **input <= '7') (*input)++;
                token.type = OCTAL_INTEGER;
            }
            else { // ʮ���ƻ򸡵�����0��ʼ
                while (isdigit(**input) || **input == '.') {
                    if (**input == '.') isFloat = true;
                    (*input)++;
                }
                token.type = isFloat ? FLOAT : DECIMAL_INTEGER;
            }
        }
        else { // ʮ���ƻ򸡵�������0��ʼ
            while (isdigit(**input) || **input == '.') {
                if (**input == '.') isFloat = true;
                (*input)++;
            }
            token.type = isFloat ? FLOAT : DECIMAL_INTEGER;
        }
    }
    else if (strchr("+-*/><=();", **input)) { // �������綨��
        token.type = OPERATOR;
        (*input)++;
    }
    else { // δʶ����ַ�
        token.type = UNRECOGNIZED;
        (*input)++;
    }

    // Ϊ�Ǳ�ʶ���ͷǹؼ��������ı�
    if (token.type != IDENTIFIER && token.type != KEYWORD_IF && token.type != KEYWORD_THEN && token.type != KEYWORD_ELSE && token.type != KEYWORD_WHILE && token.type != KEYWORD_DO) {
        size_t length = *input - start;
        strncpy(token.text, start, length);
        token.text[length] = '\0';
    }

    return token;
}

// ʮ�������ַ�ת����
int hexCharToInt(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    return -1; // �Ƿ��ַ�
}

// ʮ�������ַ���ת˫���ȸ�����
double hexStringToDouble(const char* hexStr, int base) {
    if (hexStr[0] != '0' || (hexStr[1] != 'x' && hexStr[1] != 'X' && hexStr[1] != 'o')) {
        printf("������Ч\n");
        return 0;
    }

    double result = 0.0;
    int isDecimal = 0;
    double decimalFactor = 1.0;

    for (const char* p = hexStr + 2; *p; ++p) {
        if (*p == '.') {
            isDecimal = 1;
            continue;
        }

        int val = hexCharToInt(*p);
        if (val < 0) {
            printf("ʮ�������ַ����к�����Ч�ַ�: %c\n", *p);
            return 0;
        }

        if (isDecimal) {
            decimalFactor /= (float)base;
            result += val * decimalFactor;
        }
        else {
            result = result * base + val;
        }
    }

    return result;
}

int main() {
    FILE* file;
    char buffer[1024];  // ���ڴ洢�ļ��е�ÿһ��
    const char* filename = "test.txt";

    file = fopen(filename, "r");
    if (file == NULL) {
        perror("�޷����ļ�");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        const char* input = buffer;  // ָ��ǰ�е���ʼλ��
        while (*input != '\0') {
            Token token = scan(&input);  // ���ôʷ�����������ÿ���ʻ㵥Ԫ
            if (*input == '\n') {
                break;  // �������з�ʱ�������ڲ�ѭ����������ȡ��һ��
            }
            char* type = TokenTypeToString(token.type);
            double result;
            if (!strcmp(type, "FLOAT") || !strcmp(type, "INT8") || !strcmp(type, "INT16")) {
                if (token.text[1] == 'x' || token.text[1] == 'X' || token.text[1] == 'o') {
                    int base = (token.text[1] == 'o') ? 8 : 16;
                    result = hexStringToDouble(token.text, base);
                    if (token.type == FLOAT) {
                        if(base==8)
                        printf("INT8\t  %f\n", result);
                        else
                        printf("INT16\t  %f\n", result);
                    }
                    else {
                        printf("%s\t  %d\n", type, (int)result);
                    }
                }
                else {
                    printf("INT10\t  %s\n", token.text);
                }
            }
            else {
                printf("%s\t  %s\n", type, token.text);
            }
        }
    }
    fclose(file);  // �ر��ļ�
    return 0;
}