#include "CSVPretPrin.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int isNum(const char* s)
{
    if (!s || *s == '\0') {
        return 0;
    }
    char* endPtr;
    strtod(s, &endPtr);
    while (isspace((unsigned char)*endPtr)) {
        endPtr++;
    }

    return *endPtr == '\0';
}

static void printSep(FILE* out, int* widths, int cols, char symbol)
{
    fprintf(out, "+");
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < widths[i] + 2; j++) {
            fputc(symbol, out);
        }
        fprintf(out, "+");
    }
    fprintf(out, "\n");
}

void algCSV(const char* inPath, const char* outPath)
{
    FILE* in = fopen(inPath, "r");
    FILE* out = fopen(outPath, "w");
    if (!in || !out) {
        printf("Error with opening or creating file\n");
        return;
    }

    char*** table = NULL;
    int* colWidths = NULL;
    int row = 0;
    int col = 0;
    char buffer[4096];

    while (fgets(buffer, sizeof(buffer), in)) {
        buffer[strcspn(buffer, "\r\n")] = 0;
        if (strlen(buffer) == 0) {
            continue;
        }

        table = realloc(table, (row + 1) * sizeof(char**));
        table[row] = NULL;

        int currCol = 0;
        char* linePtr = buffer;
        char* token;

        while ((token = strsep(&linePtr, ",")) != NULL) {
            table[row] = realloc(table[row], (currCol + 1) * sizeof(char*));
            table[row][currCol] = strdup(token);

            if (currCol >= col) {
                colWidths = realloc(colWidths, (currCol + 1) * sizeof(int));
                colWidths[currCol] = 0;
                if (row == 0) {
                    col = currCol + 1;
                }
            }

            int len = (int)strlen(token);
            if (len > colWidths[currCol]) {
                colWidths[currCol] = len;
            }
            currCol++;
        }
        if (row == 0) {
            col = currCol;
        }
        row++;
    }
    if (row > 0) {
        printSep(out, colWidths, col, '=');
        fprintf(out, "|");
        for (int j = 0; j < col; j++) {
            fprintf(out, " %-*s |", colWidths[j], table[0][j]);
        }
        fprintf(out, "\n");
        printSep(out, colWidths, col, '=');

        for (int i = 1; i < row; i++) {
            fprintf(out, "|");
            for (int j = 0; j < col; j++) {
                char* val = table[i][j];
                if (isNum(val)) {
                    fprintf(out, " %*s |", colWidths[j], val);
                } else {
                    fprintf(out, " %-*s |", colWidths[j], val);
                }
            }
            fprintf(out, "\n");
            printSep(out, colWidths, col, '-');
        }
        printf("Success! Check the output.txt file\n");
    } else {
        printf("Data error");
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            free(table[i][j]);
        }
        free(table[i]);
    }
    free(table);
    free(colWidths);
    fclose(in);
    fclose(out);
}
