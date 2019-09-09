#pragma once
#include "std_includes.h"
#include "json_types.h"

int parseJsonFile(FILE* fp, jsonToken* tokens, int maxTokens);
void printTokens(jsonToken* tokens, int tokenCount);
