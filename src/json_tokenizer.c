#include "json_tokenizer.h"

char strBuf[4096];

bool checkForTrue(FILE* fp) {
    char trueBuf[4];
    if (fgets(trueBuf, sizeof trueBuf, fp) != NULL) {
        if (strcmp(trueBuf, "rue") == 0) {
            return true;
        }
    }
    return false;
}

bool checkForFalse(FILE* fp) {
    char trueBuf[5];
    if (fgets(trueBuf, sizeof trueBuf, fp) != NULL) {
        if (strcmp(trueBuf, "alse") == 0) {
            return true;
        }
    }
    return false;
}

bool checkForNull(FILE* fp) {
    char trueBuf[4];
    if (fgets(trueBuf, sizeof trueBuf, fp) != NULL) {
        if (strcmp(trueBuf, "ull") == 0) {
            return true;
        }
    }
    return false;
}

int readString(FILE* fp, char* strBuf, int bufLen) {
    char lastChar = '\0';
    for (int i = 0; i < bufLen - 1; i++) {
        char c = getc(fp);
        if (c == '"' && lastChar != '\\') {
            strBuf[i] = '\0';
            return i + 1;
        }
        strBuf[i] = c;
        lastChar = c;
    }
    strBuf[bufLen-1] = '\0';
    return bufLen;
}

int parseJsonFile(FILE* fp, jsonToken* tokens, int maxTokens) {
    int c = getc(fp);
    int iTok = 0;
    while (c != EOF && iTok < maxTokens) {
        if (c == '{') {
            tokens[iTok++].type = JSON_TOK_BRACE_L;
        } else if (c == '}') {
            tokens[iTok++].type = JSON_TOK_BRACE_R;
        } else if (c =='[') {
            tokens[iTok++].type = JSON_TOK_SQ_BRACE_L;
        } else if (c ==']') {
            tokens[iTok++].type = JSON_TOK_SQ_BRACE_R;
        } else if (c == ':') {
            tokens[iTok++].type = JSON_TOK_COLON;
        } else if (c == ',') {
            tokens[iTok++].type = JSON_TOK_COMMA;
        } else if (c == '"') {
            memset(strBuf, '\0', sizeof strBuf);
            int identSize = readString(fp, strBuf, sizeof strBuf);
            tokens[iTok].data.s = (char*)malloc(identSize);
            strncpy(tokens[iTok].data.s, strBuf, identSize);
            tokens[iTok++].type = JSON_TOK_STRING;
        } else if (c == 't') {
            if (checkForTrue(fp)) {
                tokens[iTok].data.b = true;
                tokens[iTok++].type = JSON_TOK_BOOL;
            }
        } else if (c == 'f') {
            if (checkForFalse(fp)) {
                tokens[iTok].data.b = false;
                tokens[iTok++].type = JSON_TOK_BOOL;
            }
        } else if (c == 'n') {
            if (checkForNull(fp)) {
                tokens[iTok++].type = JSON_TOK_NULL;
            }
        } else if (isnumber(c) || c == '-') {
            memset(strBuf, '\0', sizeof strBuf);
            int i = 0;
            strBuf[i++] = c;
            c = getc(fp);
            while (c != EOF && (isnumber(c) || c == '.') ) {
                strBuf[i++] = c;
                c = getc(fp);
            }
            ungetc(c, fp);

            tokens[iTok].data.d = atof(strBuf);
            tokens[iTok++].type = JSON_TOK_NUMBER;
        }
        c = getc(fp);
    }

    return iTok;
}

void printTokens(jsonToken* tokens, int tokenCount) {
    for (int i = 0; i < tokenCount; i++) {
        switch (tokens[i].type) {
            case JSON_TOK_BRACE_L:
                printf("%i\t%s\t%s\n", i, "LCBRACE", "{");
                break;
            case JSON_TOK_BRACE_R:
                printf("%i\t%s\t%s\n", i, "RCBRACE", "}");
                break;
            case JSON_TOK_SQ_BRACE_L:
                printf("%i\t%s\t%s\n", i, "LSBRACE", "[");
                break;
            case JSON_TOK_SQ_BRACE_R:
                printf("%i\t%s\t%s\n", i, "RSBRACE", "]");
                break;
            case JSON_TOK_COLON:
                printf("%i\t%s\t%s\n", i, "COLON", ":");
                break;
            case JSON_TOK_COMMA:
                printf("%i\t%s\t%s\n", i, "COMMA", ",");
                break;
            case JSON_TOK_BOOL:
                printf("%i\t%s\t%s\n", i, "BOOL", tokens[i].data.b ? "true" : "false");
                break;
            case JSON_TOK_NULL:
                printf("%i\t%s\n", i, "NULL");
                break;
            case JSON_TOK_NUMBER:
                printf("%i\t%s\t%f\n", i, "NUMBER", tokens[i].data.d);
                break;
            case JSON_TOK_STRING:
                printf("%i\t%s\t\"%s\"\n", i, "STRING", tokens[i].data.s);
                break;
            default:
                printf("%i\t%i\n", i, tokens[i].type);
        }
    }
}