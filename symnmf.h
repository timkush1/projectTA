
struct cord
{
    double value;
    struct cord *next;
};

struct vector
{
    struct vector *next;
    struct cord *cords;
};
double getDistance1(double *centroids, struct vector *head_vec, int D);
double getDistance2(double *centroids, double *newCentroids, int D);
void printClusters(double **, int K, int D);
double *calculateCentroids(double *newCentroid,int **arr, struct vector *head_vec, int *counters, int D, int N, int i);
int updateCentroids(double **centroids, int **arr, struct vector *head_vec, int *counters, int K, int D, int N, double epsilon);
void kmeans_clustering(double ** clusters, struct vector *head_vec , int K, int D, int N, int iter, double epsilon);