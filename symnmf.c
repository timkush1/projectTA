#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "symnmf.h"

double getDistance1(double *centroids, struct vector *head_vec, int D);
double getDistance2(double *centroids, double *newCentroids, int D);
double *calculateCentroids(double *newCentroid, int **arr, struct vector *head_vec, int *counters, int D, int N, int i);
int updateCentroids(double **centroids, int **arr, struct vector *head_vec, int *counters, int K, int D, int N, double epsilon);
void kmeans_clustering(double **clusters, struct vector *head_vec, int K, int D, int N, int iter, double epsilon);
int main(int argc, char **argv)
{
    return 0;
}
void kmeans_clustering(double **clusters, struct vector *head_vec, int K, int D, int N, int iter, double epsilon) /*cluster - eshkolot, vectorList - all the vectors*/
{

    struct vector *curr_vec;
    struct cord *head_cord;
    struct cord *curr_cord;
    int i;
    int j;
    int *counters;
    int iterCounter = 0;
    double distance;
    double minDistance = DBL_MAX;
    int minDistanceIndex;

    int **array;
    int epsilonIndicator = 0;

    counters = malloc(K * sizeof(int)); /* יוצרים מערך מונים ומאתחלים אותו ל0 כל אחד*/
    for (i = 0; i < K; i++)
    {
        counters[i] = 1;
    }

    array = (int **)malloc(K * sizeof(int *)); /*מאתחל מערך בינארי שישמש אידיקציה לשייכות אשכול*/
    for (i = 0; i < K; i++)
    {
        array[i] = (int *)malloc(N * sizeof(int));
    }

    head_cord = head_vec->cords;
    while (epsilonIndicator == 0 && iterCounter < iter)
    {
        curr_vec = head_vec; /*before entereing the loop we set currs vec and cord to top*/
        head_cord = curr_vec->cords;
        curr_cord = head_cord;
        for (i = 0; i < K; i++) /*updating boolean array to zero for all iteration*/
        {
            for (j = 0; j < N; j++)
            {
                array[i][j] = 0;
            }
        }

        for (i = 0; i < K; i++)
        {
            counters[i] = 0;
        }
        for (i = 0; i < N; i++) /*this loop finds the the minimum distance and relevant cluster*/
        {
            minDistance = DBL_MAX;
            minDistanceIndex = -1;
            for (j = 0; j < K; j++) /*finds the attachment cluster */
            {
                distance = getDistance1(clusters[j], curr_vec, D);
                if (distance < minDistance)
                {
                    minDistance = distance;
                    minDistanceIndex = j;
                }
            }

            counters[minDistanceIndex]++;
            array[minDistanceIndex][i] = 1;
            curr_vec = curr_vec->next;
            curr_cord = curr_vec->cords;
        }

        iterCounter++;
        epsilonIndicator = updateCentroids(clusters, array, head_vec, counters, K, D, N, epsilon);
    }

    for (i = 0; i < K; i++) /*free array*/
    {
        free(array[i]);
    }
    free(array);

    free(counters); /*free counters*/
}

double *calculateCentroids(double *newCentroid, int **array, struct vector *head_vec, int *counters, int D, int N, int lineIndex)
{
    struct vector *curr_vec = head_vec;
    struct cord *curr_cord = head_vec->cords;
    int h = 0;
    int j;
    int counter;
    for (h = 0; h < D; h++)
    {
        newCentroid[h] = 0;
    }
    for (j = 0; j < N; j++) /*checks if in arrays which vectors the cluster contains*/
    {
        counter = 0;
        if (array[lineIndex][j] == 1)
        {
            counter += 1;
            for (h = 0; h < D; h++)
            {
                newCentroid[h] = newCentroid[h] + curr_cord->value;
                curr_cord = curr_cord->next;
            }
        }
        curr_vec = curr_vec->next;
        curr_cord = curr_vec->cords;
    }
    for (h = 0; h < D; h++)
    {
        newCentroid[h] = newCentroid[h] / (counters[lineIndex]);
    }

    return newCentroid;
}
int updateCentroids(double **centroids, int **array, struct vector *head_vec, int *counters, int K, int D, int N, double epsilon)
{
    double *newCentroid = (double *)malloc(D * sizeof(double));
    int epsilonIndicator = 1;
    int h = 0;
    int i = 0;
    for (i = 0; i < K; i++) /*run on the k clusters* and checks if the new centroid - curr centroid for all is below epsilon*/
    {
        newCentroid = calculateCentroids(newCentroid, array, head_vec, counters, D, N, i);
        if (getDistance2(centroids[i], newCentroid, D) >= epsilon)
        {
            epsilonIndicator = 0;
        }
        for (h = 0; h < D; h++)
        {
            centroids[i][h] = newCentroid[h];
        }
    }
    free(newCentroid);
    return epsilonIndicator;
}
double getDistance2(double *centroids, double *newCentroids, int D) /*return distance between vector and array*/
{
    int j = 0;
    double distance = 0;
    for (j = 0; j < D; j++)
    {
        distance = distance + pow((centroids[j] - newCentroids[j]), 2);
    }
    distance = sqrt(distance);
    return distance;
}
double getDistance1(double *centroids, struct vector *head_vec, int D) /* return distance between vector and array */
{
    int j = 0;
    double distance = 0;
    struct cord *curr_cord = head_vec->cords;

    for (j = 0; j < D; j++)
    {
        distance = distance + pow((centroids[j] - curr_cord->value), 2);
        curr_cord = curr_cord->next;
    }
    distance = sqrt(distance);
    return distance;
}
