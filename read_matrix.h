extern FILE* m_file;
extern int s_rank, m_rank, row, column;
extern double** matrix;

void logproc(char* format, ...);

//
// Skips a line of matrix input.
//
static void skip_line()
{
    int i = m_rank, n;
    // Usage of the obscure --> operator!
    while (i --> 0)
    {
        fscanf(m_file, "%d", &n);
    }
}

static void read_matrix()
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
    for (i = 0; i < s_rank * row; i++)
    {
        skip_line();
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
}
