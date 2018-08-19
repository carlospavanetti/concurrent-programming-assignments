#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <pthread.h>

#define min(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })

typedef struct {
  size_t size;
  double *magnitudes;
  double *directions;
} RadialPoints;

void read_arguments(int argc, char **argv, size_t *size, int *threads);
void initialize_with_random(RadialPoints *, int size);
void release_points(RadialPoints *);
void release_distances(double *);
double *points_distances(RadialPoints *, RadialPoints *);

int main(int argc, char **argv) {
  size_t size;
  int threads;
  RadialPoints points[2];
  read_arguments(argc, argv, &size, &threads);

  srand(time(NULL));
  initialize_with_random(&points[0], size);
  initialize_with_random(&points[1], size);

  double *distances = points_distances(&points[0], &points[1]);

  release_points(&points[0]);
  release_points(&points[1]);
  release_distances(distances);

  return 0;
}

void read_arguments(int argc, char **argv, size_t *size, int *threads) {
  *size = 0;
  *threads = 1;
  for (int i = 1; i <= argc; i++) {
    if (strstr(argv[i], "-size=") == argv[i]) {
      *size = strtol(argv[1] + 6, NULL, 10);
    }
    if (strstr(argv[1], "-threads=") == argv[i]) {
      *size = strtol(argv[1] + 9, NULL, 10);
    }
  }
}


void initialize_with_random(RadialPoints *points, int size) {
  points->size = size;
  points->magnitudes = (double *) malloc(size * sizeof(double));
  points->directions = (double *) malloc(size * sizeof(double));

  for (int i = 0; i < size; i++) {
    points->magnitudes[i] = ((rand() % 10000) / 7.0);
    points->directions[i] = ((rand() % 360) * M_PI / 180 );
  }
}

void release_points(RadialPoints *points) {
  free(points->magnitudes);
  free(points->directions);
  points->size = 0;
}

double *points_distances(RadialPoints *A, RadialPoints *B) {
  size_t size = min(A->size, B->size);
  double *distances = (double *) malloc(size * sizeof(double));

  double *R1 = A->magnitudes;
  double *R2 = B->magnitudes;
  double *T1 = A->directions;
  double *T2 = B->directions;
  for (int i = 0; i < size; i++) {
    distances[i] = sqrt(
      (R1[i] * R1[i]) + (R2[i] * R2[i]) - 2 * (R1[i] * R2[i]) * cos(T2 - T1)
    );
  }
  return distances;
}

void release_distances(double *distances) {
  free(distances);
}
