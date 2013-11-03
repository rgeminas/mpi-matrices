#ifndef __READ_MATRIX_H
#define __READ_MATRIX_H

void skip_line(FILE* m_file, int m_rank);
double** read_matrix(FILE* m_file, int m_rank, int s_rank, int row, int column);

#endif
