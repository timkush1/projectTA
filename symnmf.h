

double** createDdg(double** arr, int n, int d); // create diagonal matrix
double rowSum(double* arr, int d); // calculate sum of one row
double** createNorm(double** A, double** D, int n); // create the W(normalized) matrix
void matrixMulti(double** first, double** second, double** result, int n); // updating "result matrix" to be matrix multiplication of first*second
double** createSim(double** X,double** A,int n, int d) ; // create matrix A(similarity matrix)
double getSquaredDistance(double *pointA, double *pointB, int D) ;  //return distance between two points */
void printMatrix(double** x, int n,int d);