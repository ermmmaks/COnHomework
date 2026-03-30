#include "CSVPretPrin.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

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

static void printSep(FILE* out, const int* widths, int cols, char symbol)
{
    if (!widths) {
        return;
    }
    fprintf(out, "+");
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < widths[i] + 2; j++) {
            fputc(symbol, out);
        }
        fprintf(out, "+");
    }
    fprintf(out, "\n");
}

void freeTable(char*** table, int* colWidths, int row, int col, FILE* in, FILE* out)
{
    if (table) {
        for (int i = 0; i < row; i++) {
            if (table[i]) {
                for (int j = 0; j < col; j++) {
                    free(table[i][j]);
                }
                free(table[i]);
            }
        }
        free(table);
    }
    if (colWidths) {
        free(colWidths);
    }
    if (in) {
        fclose(in);
    }
    if (out) {
        fclose(out);
    }
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

        char*** tmpTable = realloc(table, (row + 1) * sizeof(char**));
        if (!tmpTable) {
            freeTable(table, colWidths, row, col, in, out);
            printf("Realloc error!\n");
            return;
        }
        table = tmpTable;
        table[row] = NULL;

        int currCol = 0;
        char* linePtr = buffer;
        char* token;

        while ((token = strsep(&linePtr, ",")) != NULL) {
            if (currCol >= col) {
                int* tmpColWidths = realloc(colWidths, (currCol + 1) * sizeof(int));
                if (!tmpColWidths) {
                    printf("Realloc error!\n");
                    freeTable(table, colWidths, row, col, in, out);
                    return;
                }
                colWidths = tmpColWidths;
                colWidths[currCol] = 0;
                col = currCol + 1;
            }

            char** tmpRow = realloc(table[row], (currCol + 1) * sizeof(char*));
            if (!tmpRow) {
                printf("Realloc error!\n");
                freeTable(table, colWidths, row + 1, col, in, out);
                return;
            }
            table[row] = tmpRow;
            table[row][currCol] = strdup(token);

            int len = (int)strlen(token);
            if (len > colWidths[currCol]) {
                colWidths[currCol] = len;
            }
            currCol++;
        }

        // нужно заполнить недостающие колонки нулями, если их меньше максимума
        if (currCol < col) {
            char** tmpRow = realloc(table[row], col * sizeof(char*));
            if (tmpRow) {
                table[row] = tmpRow;
                for (int j = currCol; j < col; j++) {
                    table[row][j] = NULL;
                }
            }
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
                char* val = (j < col) ? table[i][j] : "";
                if (val && isNum(val)) {
                    fprintf(out, " %*s |", colWidths[j], val);
                } else {
                    fprintf(out, " %-*s |", colWidths[j], val ? val : "");
                }
            }
            fprintf(out, "\n");
            printSep(out, colWidths, col, '-');
        }
        printf("Success! Check the output.txt file\n");
    } else {
        printf("Data error");
    }

    freeTable(table, colWidths, row, col, in, out);
}
