#include <mpi.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include <signal.h>

#include "read_matrix.h"

int rank, size, linesize, max_rank, m_rank, row, column, s_rank;
int dim[2], coord[2];
FILE* m_file;
double** matrix;
double* diagonal;
MPI_Comm cart_comm;

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
        int i;
        int period[] = { 0, 0 };
        MPI_Comm row_comm, column_comm;

        // The single-row maintains the column dimension and vice-versa
        int row_only[] = {0, 1};
        int column_only[] = {1, 0};

        dim[0] = linesize, dim[1] = linesize;

        // Create cartesian communicators.
        MPI_Cart_create(MPI_COMM_WORLD, 2, dim, period, 0, &cart_comm); 
        MPI_Cart_coords(cart_comm, rank, 2, coord); 
        logproc("I am on line %d, column %d on the cartesian communicator.\n", coord[0], coord[1]);

        // Read the matrix from the file.
        row = coord[0], column = coord[1];
        s_rank = m_rank / linesize;
        logproc("I'll read a rank %d matrix.\n", s_rank);
        read_matrix();

        // Create row and column communicators.
        MPI_Cart_sub(cart_comm, row_only, &row_comm);
        MPI_Cart_sub(cart_comm, column_only, &column_comm);
        
        // If we're diagonal holders, broadcast the diagonal vector.
        diagonal = malloc(s_rank * sizeof(double));
        if (row == column)
        {
            for (i = 0; i < s_rank; i++)
            {
                diagonal[i] = matrix[i][i];
            }
        }
        // The rank should be gotten from the row communicator, not the cartesian one.
        int diag_holder[] = { row };
        int bcast_rank;
        MPI_Cart_rank(row_comm, diag_holder, &bcast_rank);

        logproc("About to do broadcast\n");

        MPI_Bcast(diagonal, s_rank, MPI_DOUBLE, bcast_rank, row_comm);

        logproc("I have the diagonals\n");

        for (i = 0; i < s_rank; i++)
        {
            logproc("Diagonal %d: %lf\n", i, diagonal[i]);
        }
        MPI_Finalize(); 

    }
    return 0;
}
