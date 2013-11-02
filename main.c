#include <mpi.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include <signal.h>

int rank, size, linesize, max_rank, m_rank, line, column, s_rank;
FILE* m_file;
double** matrix;

//
// Basically wraps around printf, but writing the process name too.
//
void logproc(char* format, ...)
{
   va_list args;
   va_start(args, format);
   printf("Process %d/%d: ", rank, size);
   vprintf(format, args);
   va_end(args);
}

void skip_line()
{
    int i = m_rank, n;
    // Usage of the obscure --> operator!
    while (i --> 0)
    {
        fscanf(m_file, "%d", &n);
    }
}

void read_matrix()
{
    double buf, *data = malloc(s_rank * s_rank * sizeof(double));
    int i, j;
    matrix = malloc(s_rank * sizeof(double*));

    for (i = 0; i < s_rank; i++)
    {
        matrix[i] = data;
        data += s_rank;
    }
    // f will be divided into `size` submatrices;
    // We'll skip lines until we get to the first of our lines
    for (i = 0; i < s_rank * line; i++)
    {
        skip_line();
    }
    for (i = 0; i < s_rank; i++)
    {
        for (j = 0; j < s_rank * column; j++)
        {
            fscanf(m_file, "%lf", &buf); 
        }
        for (j = 0; j < s_rank; j++)
        {
            fscanf(m_file, "%lf", &matrix[i][j]);
            logproc("%d, %d: %lf\n", i, j, matrix[i][j]);
        }
        for (j = 0; j + s_rank * column + s_rank < m_rank; j++)
        {
            fscanf(m_file, "%lf", &buf);
        }
    }
}

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);             	/* starts MPI */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);	/* get current process id */
    MPI_Comm_size(MPI_COMM_WORLD, &size);	/* get number of processes */

    // We're getting everything through a file.
    assert (argc > 1);
    m_file = fopen(argv[1], "r");
    fscanf(m_file, "%d\n", &m_rank); 
    
    logproc("Reading a matrix of rank %d\n", m_rank);

    // Some processes will be left idling if you assign too many since we're
    // dividing equally in rows and columns. Such is life.
    linesize = floor(sqrt(size));
    max_rank = linesize * linesize - 1;
    if (m_rank % linesize != 0) 
    {
        logproc("Matrix rank not divisible by process line size. Aborting.\n");
        MPI_Finalize();   
        exit(0);
    }
    if (rank > max_rank)
    {
        logproc("I am useless: finalizing...\n");
        MPI_Finalize();   
        exit(0);
    }
    else
    {
        line = rank / linesize, column = rank % linesize;
        s_rank = m_rank / linesize;
        logproc("I am on line %d, column %d. I'll read a rank %d matrix.\n", line, column, s_rank);
        read_matrix();
    }
    MPI_Finalize();   
    return 0;
}
