#include "avp_transformer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void runTest(char const *const testTitle, char const *const moduleName, char const *const functionName, char *const value, unsigned int maxValSz);

int main(int argc, char *argv[])
{
    /* Default operation is to run tests */
    if (1 == argc)
    {
        enum
        {
            MAX_VAL_SZ = 32
        };
        char value[MAX_VAL_SZ] = "value set in c";

        runTest("Valid transform", "scripts.valid", "transform", value, MAX_VAL_SZ);

        runTest("Provide Null moduleName to function", NULL, "transform", value, MAX_VAL_SZ);
        runTest("Provide Null functionName to function", "scripts.valid", NULL, value, MAX_VAL_SZ);
        runTest("Provide Null value to function", "script", "transform", NULL, MAX_VAL_SZ);
        runTest("Provide Non-existant script to function", "scripts.invalidFilename", "transform", value, MAX_VAL_SZ);
        runTest("Script returns number", "scripts.invalidReturnNumber", "transform", value, MAX_VAL_SZ);
        runTest("Script has different function name", "scripts.invalidFunctionName", "transform", value, MAX_VAL_SZ);
        runTest("Script function has no args", "scripts.invalidNoArgs", "transform", value, MAX_VAL_SZ);
        runTest("Script function has extra args", "scripts.invalidNotEnoughArgs", "transform", value, MAX_VAL_SZ);
        runTest("Script returns None", "scripts.invalidReturnNone", "transform", value, MAX_VAL_SZ);
        runTest("Script returns a string bigger than our buffer", "scripts.invalidReturnTooBig", "transform", value, MAX_VAL_SZ);
    }
    else if (5 == argc)
    {
        char const *const moduleName = argv[1];
        char const *const functionName = argv[2];
        unsigned int maxValSz = atoi(argv[4]);
        char *const value = malloc(maxValSz * sizeof(char));

        strncpy(value, argv[3], maxValSz);
        printf("Calling transform with the following:\n");
        printf("\tmoduleName: '%s'\n", moduleName);
        printf("\tfunctionName: '%s'\n", functionName);
        printf("\tvalue: '%s'\n", value);
        printf("\tmaxValSz: %i\n", maxValSz);

        int result = AVP_TransformValue(moduleName, functionName, value, maxValSz);
        printf("Function returned %s. Value is now '%s'\n", result == 0 ? "SUCCESS" : "FAILURE", value);

        free(value);
    }
    else
    {
        printf("Number of arguments, accept either 1 or 5\n");
        printf("E.g. ./main scripts.valid transform \"my name jeff\" 32\n");
    }
}

void runTest(char const *const testTitle, char const *const moduleName, char const *const functionName, char *const value, unsigned int maxValSz)
{
    if (NULL != value)
    {
        strcpy(value, "value set in c");
    }

    printf("\n");
    printf("*************************************************\n");
    printf("* Run test: %s\n", testTitle);
    printf("*************************************************\n");
    printf("value before transform: '%s'\n", value);
    int result = AVP_TransformValue(moduleName, functionName, value, maxValSz);
    printf("value after transform : '%s'\n", value);
    printf("Function returned [%i] - AKA %s\n\n", result, result == 0 ? "Success" : "Failure");
}