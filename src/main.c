#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "stretchy_buffer.h"
#include "json_types.h"
#include "json_tokenizer.h"

int main() {

    FILE* fp = fopen("test.json", "r");
    const int MAX_TOKENS = 100;
    jsonToken tokens[MAX_TOKENS];
    jsonObject root = {NULL, NULL};

    if (fp == NULL) {
        printf("fp is null\n");
        exit(1);
    }

    int tokenCount = parseJsonFile(fp, tokens, MAX_TOKENS);
    printf("tokens: %d\n", tokenCount);
    printTokens(tokens, tokenCount);

    if (tokens[0].type != JSON_TOK_BRACE_L || tokens[tokenCount-1].type != JSON_TOK_BRACE_R) {
        printf("Invalid root braces\n");
        exit(1);
    }

    printf("\n");

    jsonToken* curTok = tokens;
    curTok++;
    while (true) {
        if (curTok->type != JSON_TOK_STRING) {
            printf("expected property key of type string\n");
            exit(1);
        }
        char* key = curTok->data.s;
        curTok++;
        if (curTok->type != JSON_TOK_COLON) {
            printf("expected colon after key\n");
            free(key);
            exit(1);
        }
        curTok++;
        jsonValue val;
        switch (curTok->type) {
            case JSON_TOK_NUMBER:
                val.type = JSON_VAL_NUMBER;
                val.data.d = curTok->data.d;
                break;
            case JSON_TOK_STRING:
                val.type = JSON_VAL_STRING;
                val.data.s = curTok->data.s;
                break;
            case JSON_TOK_BOOL:
                val.type = JSON_VAL_BOOL;
                val.data.b = curTok->data.b;
                break;
            case JSON_TOK_NULL:
                val.type = JSON_VAL_NULL;
                break;
            default:
                printf("Not implemented\n");
                exit(1);
                break;
        }
        sb_push(root.keys, key);
        sb_push(root.values, val);
        
        curTok++;
        if (curTok->type != JSON_TOK_COMMA) {
            break;
        }
        curTok++;
    }

    int rootSize = sb_count(root.keys);

    for (int i = 0; i < rootSize; i++) {
        printf("%s: ", root.keys[i]);
        jsonValueData* data = &root.values[i].data;
        switch (root.values[i].type) {
            case JSON_VAL_NUMBER:
                printf("%f\n", data->d);
                break;
            case JSON_VAL_STRING:
                printf("%s\n", data->s);
                free(data->s);
                break;
            case JSON_VAL_BOOL:
                printf("%s\n", data->b ? "true" : "false");
                break;
            case JSON_VAL_NULL:
                printf("%s\n", "null");
                break;
            default:
                printf("Not implemented\n");
        }
        free(root.keys[i]);
    }

    sb_free(root.keys);
    sb_free(root.values);

    fclose(fp);

    return 0;
}
