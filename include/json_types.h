#pragma once

typedef enum {
    JSON_VAL_NUMBER,
    JSON_VAL_STRING,
    JSON_VAL_BOOL,
    JSON_VAL_NULL,
    JSON_VAL_OBJ,
    JSON_VAL_ARRAY,
    JSON_VAL_SIZE
} jsonValueType;

typedef union {
    bool b;
    double d;
    char* s;
    void* v;
} jsonValueData;

typedef struct {
    jsonValueType type;
    jsonValueData data;
} jsonValue;

typedef struct {
    jsonValue* values;
} jsonArray;

typedef struct {
    char** keys;
    jsonValue* values;
} jsonObject;

typedef enum {
    JSON_TOK_BRACE_L,
    JSON_TOK_BRACE_R,
    JSON_TOK_SQ_BRACE_L,
    JSON_TOK_SQ_BRACE_R,
    JSON_TOK_STRING,
    JSON_TOK_BOOL,
    JSON_TOK_COLON,
    JSON_TOK_COMMA,
    JSON_TOK_NUMBER,
    JSON_TOK_NULL,
    JSON_TOK_SIZE
} jsonTokenType;

typedef struct {
    jsonTokenType type;
    jsonValueData data;
} jsonToken;