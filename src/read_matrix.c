#include <stdio.h>
#include <stdlib.h>
#include "read_matrix.h"

void logproc(char* format, ...);

//
// Skips a line of matrix input.
//
void skip_line(FILE* m_file, int m_rank)
{
    int i = m_rank, n;
    while (i --> 0) // i goes to 0
    {
        fscanf(m_file, "%d", &n);
    }
}

//
// Reads a file comprised of a matrix rank, then n * n elements.
//
double** read_matrix(FILE* m_file, int m_rank, int s_rank, int row, int column)
{
    double buf, *data = malloc(s_rank * s_rank * sizeof(double));
    int i, j;
    double **matrix = malloc(s_rank * sizeof(double*));

    for (i = 0; i < s_rank; i++)
    {
        matrix[i] = data;
        data += s_rank;
    }
    // We'll skip lines until we get to the first of our lines
    for (i = 0; i < s_rank * row; i++)
    {
        skip_line(m_file, m_rank);
    }
    for (i = 0; i < s_rank; i++)
    {
        // First set of skips
        for (j = 0; j < s_rank * column; j++)
        {
            fscanf(m_file, "%lf", &buf); 
        }
        // Reads the pertaining part
        for (j = 0; j < s_rank; j++)
        {
            fscanf(m_file, "%lf", &matrix[i][j]);
            //logproc("%d, %d: %lf\n", i, j, matrix[i][j]);
        }
        // Skips until the end of the line
        for (j = 0; j + s_rank * column + s_rank < m_rank; j++)
        {
            fscanf(m_file, "%lf", &buf);
        }
    }
    return matrix;
}
