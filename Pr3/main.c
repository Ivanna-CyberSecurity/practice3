#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int rows;
    int cols;
    double **data;
} Matrix;

Matrix createMatrix(int rows, int cols) {
    Matrix matrix;
    matrix.rows = rows;
    matrix.cols = cols;
    matrix.data = (double **) malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
        matrix.data[i] = (double *) malloc(cols * sizeof(double));
    }
    return matrix;
}

void deleteMatrix(Matrix *matrix) {
    for (int i = 0; i < matrix -> rows; i++) {
        free(matrix -> data[i]);
    }
    free(matrix -> data);
}

Matrix resizeMatrix(Matrix *matrix, int newRows, int newCols) {
    Matrix newMatrix = createMatrix(newRows, newCols);
    int minRows = (matrix -> rows < newRows) ? matrix -> rows : newRows;
    int minCols = (matrix -> cols < newCols) ? matrix -> cols : newCols;

    for (int i = 0; i < minRows; i++) {
        for (int j = 0; j < minCols; j++) {
            newMatrix.data[i][j] = matrix -> data[i][j];
        }
    }

    deleteMatrix(matrix);
    *matrix = newMatrix;

    return newMatrix;
}

int getRowCount(Matrix matrix) {
    return matrix.rows;
}

int getColCount(Matrix matrix) {
    return matrix.cols;
}

void setElement(Matrix *matrix, int row, int col, double value) {
    if (row >= 0 && row < matrix -> rows && col >= 0 && col < matrix -> cols) {
        matrix -> data[row][col] = value;
    } else {
        printf("Error: Attempting to set a value outside the matrix.\n");
    }
}

double getElement(Matrix matrix, int row, int col) {
    if (row >= 0 && row < matrix.rows && col >= 0 && col < matrix.cols) {
        return matrix.data[row][col];
    } else {
        printf("Error: Attempting to get a value outside the matrix.\n");
        return 0.0;
    }
}

void printMatrix(Matrix matrix) {
    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            printf("%.2f\t", matrix.data[i][j]);
        }
        printf("\n");
    }
}

void saveMatrixToFile(Matrix matrix, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file) {
        fprintf(file, "%d %d\n", matrix.rows, matrix.cols);
        for (int i = 0; i < matrix.rows; i++) {
            for (int j = 0; j < matrix.cols; j++) {
                fprintf(file, "%.2f ", matrix.data[i][j]);
            }
            fprintf(file, "\n");
        }
        fclose(file);
        printf("The matrix is saved to a file %s\n", filename);
    } else {
        printf("Error opening file for writing.\n");
    }
}

Matrix readMatrixFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        int rows, cols;
        fscanf(file, "%d %d", &rows, &cols);
        Matrix matrix = createMatrix(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                fscanf(file, "%lf", &matrix.data[i][j]);
            }
        }
        fclose(file);
        printf("The matrix is read from the file %s\n", filename);
        return matrix;
    } else {
        printf("Error opening file for reading.\n");
        return createMatrix(0, 0);
    }
}

Matrix addMatrices(Matrix matrix1, Matrix matrix2) {
    if (matrix1.rows == matrix2.rows && matrix1.cols == matrix2.cols) {
        Matrix result = createMatrix(matrix1.rows, matrix1.cols);
        for (int i = 0; i < matrix1.rows; i++) {
            for (int j = 0; j < matrix1.cols; j++) {
                result.data[i][j] = matrix1.data[i][j] + matrix2.data[i][j];
            }
        }
        return result;
    } else {
        printf("Error: Unable to add matrices of different sizes.\n");
        return createMatrix(0, 0);
    }
}

Matrix subtractMatrices(Matrix matrix1, Matrix matrix2) {
    if (matrix1.rows == matrix2.rows && matrix1.cols == matrix2.cols) {
        Matrix result = createMatrix(matrix1.rows, matrix1.cols);
        for (int i = 0; i < matrix1.rows; i++) {
            for (int j = 0; j < matrix1.cols; j++) {
                result.data[i][j] = matrix1.data[i][j] - matrix2.data[i][j];
            }
        }
        return result;
    } else {
        printf("Error: Unable to subtract matrices of different sizes.\n");
        return createMatrix(0, 0);
    }
}

Matrix multiplyMatrixByScalar(Matrix matrix, double scalar) {
    Matrix result = createMatrix(matrix.rows, matrix.cols);
    for (int i = 0; i < matrix.rows; i++) {
        for (int j = 0; j < matrix.cols; j++) {
            result.data[i][j] = matrix.data[i][j] * scalar;
        }
    }
    return result;
}

Matrix multiplyMatrices(Matrix matrix1, Matrix matrix2) {
    if (matrix1.cols == matrix2.rows) {
        Matrix result = createMatrix(matrix1.rows, matrix2.cols);
        for (int i = 0; i < matrix1.rows; i++) {
            for (int j = 0; j < matrix2.cols; j++) {
                result.data[i][j] = 0;
                for (int k = 0; k < matrix1.cols; k++) {
                    result.data[i][j] += matrix1.data[i][k] * matrix2.data[k][j];
                }
            }
        }
        return result;
    } else {
        printf("Error: Unable to multiply matrices with these dimensions.\n");
        return createMatrix(0, 0);
    }
}

int main() {
    Matrix matrix1 = createMatrix(3, 3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            matrix1.data[i][j] = i * 3 + j + 1;
        }
    }

    printf("Matrix 1:\n");
    printMatrix(matrix1);

    resizeMatrix(&matrix1, 2, 2);

    printf("\nMatrix 1 after resizing:\n");
    printMatrix(matrix1);

    Matrix matrix2 = createMatrix(2, 2);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            matrix2.data[i][j] = (i + 1) * 0.5 + (j + 1) * 0.25;
        }
    }

    printf("\nMatrix 2:\n");
    printMatrix(matrix2);

    Matrix sum = addMatrices(matrix1, matrix2);
    printf("\nThe sum of matrices 1 and 2:\n");
    printMatrix(sum);

    Matrix scaledMatrix = multiplyMatrixByScalar(matrix1, 2.0);
    printf("\nMatrix 1 multiplied by 2.0:\n");
    printMatrix(scaledMatrix);

    Matrix product = multiplyMatrices(matrix1, matrix2);
    printf("\nProduct of matrices 1 and 2:\n");
    printMatrix(product);

    Matrix subtractedMatrix = subtractMatrices(matrix1, matrix2);
    printf("\nThe difference of matrices 1 and 2:\n");
    printMatrix(subtractedMatrix);

    saveMatrixToFile(matrix1, "matrix.txt");

    Matrix matrixFromFile = readMatrixFromFile("matrix.txt");

    printf("\nMatrix read from file:\n");
    printMatrix(matrixFromFile);

    deleteMatrix(&matrix1);
    deleteMatrix(&sum);
    deleteMatrix(&scaledMatrix);
    deleteMatrix(&product);
    deleteMatrix(&subtractedMatrix);
    deleteMatrix(&matrixFromFile);

    printf("\nFunction check:\n");

    printf("Матриця 2:\n");
    printMatrix(matrix2);

    printf("\nThe number of rows of the matrix is 2: %d\n", getRowCount(matrix2));
    printf("Number of columns of matrix 2: %d\n", getColCount(matrix2));

    setElement(&matrix2, 0, 0, 10.0);
    printf("Element (0,0) of matrix 2: %.2f\n", getElement(matrix2, 0, 0));

    printf("\nMatrix 2:\n");
    printMatrix(matrix2);

    deleteMatrix(&matrix2);

    return 0;
}