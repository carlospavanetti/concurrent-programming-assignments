#include <math.h>
#include <stdio.h>
#include <stdbool.h>
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

void read_arguments(int argc, char **argv, size_t *size, int *threads, bool *verbose);
void initialize_with_random(RadialPoints *, int size);
void release_points(RadialPoints *);
void release_distances(double *);
double *points_distances(RadialPoints *, RadialPoints *, int threads);

int main(int argc, char **argv) {
  size_t size;
  int threads;
  bool verbose;
  RadialPoints points[2];
  read_arguments(argc, argv, &size, &threads, &verbose);

  srand(time(NULL));
  initialize_with_random(&points[0], size);
  initialize_with_random(&points[1], size);

  if (verbose) {
    for (int i = 0; i < size; i++) {
      printf("A.r = %8.3lf\tA.t = %4.2lf\n", points[0].magnitudes[i], points[0].directions[i]);
      printf("B.r = %8.3lf\tB.t = %4.2lf\n", points[1].magnitudes[i], points[1].directions[i]);
    }
  }

  double elapsed;
  struct timespec start, finish;
  clock_gettime(CLOCK_MONOTONIC, &start);

  double *distances = points_distances(&points[0], &points[1], threads);

  clock_gettime(CLOCK_MONOTONIC, &finish);
  elapsed  = (finish.tv_sec  - start.tv_sec);
  elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

  if (verbose) {
    for (int i = 0; i < size; i++) {
      printf("Distance = %8.3lf\n", distances[i]);
    }
  }

  printf("Elapsed time: %.6lf", elapsed);

  release_points(&points[0]);
  release_points(&points[1]);
  release_distances(distances);

  return 0;
}

void read_arguments(int argc, char **argv, size_t *size, int *threads, bool *verbose) {
  *size = 0;
  *threads = 1;
  *verbose = false;
  for (int i = 1; i < argc; i++) {
    if (strstr(argv[i], "-size=") == argv[i]) {
      *size = strtol(argv[i] + 6, NULL, 10);
    }
    if (strstr(argv[i], "-threads=") == argv[i]) {
      *threads = strtol(argv[i] + 9, NULL, 10);
    }
    if (strstr(argv[i], "-verbose") == argv[i]) {
      *verbose = true;
    }
  }
}


void initialize_with_random(RadialPoints *points, int size) {
  points->size = size;
  points->magnitudes = (double *) malloc(size * sizeof(double));
  points->directions = (double *) malloc(size * sizeof(double));

  for (int i = 0; i < size; i++) {
    points->magnitudes[i] = ((rand() % 7000) / 70.0);
    points->directions[i] = ((rand() % 360) * M_PI / 180 );
  }
}

void release_points(RadialPoints *points) {
  free(points->magnitudes);
  free(points->directions);
  points->size = 0;
}

void compute_distances_block(
  RadialPoints *A, RadialPoints *B,
  double *distances, int offset, int size
) {
  for (int i = offset; i < offset + size; i++) {
    distances[i] = sqrt(
      (A->magnitudes[i] * A->magnitudes[i]) +
      (B->magnitudes[i] * B->magnitudes[i]) -
      2 * (
        (A->magnitudes[i] * B->magnitudes[i]) *
        cos(A->directions[i] - B->directions[i])
      )
    );
  }
}

typedef struct {
  RadialPoints *first_set;
  RadialPoints *second_set;
  double *distances;
  int block_offset;
  int block_size;
} RDArguments;

void *radial_distances_process(void *amorph) {
  RDArguments *args = (RDArguments *) amorph;
  compute_distances_block(
    args->first_set, args->second_set, args->distances,
    args->block_offset, args->block_size
  );
  pthread_exit(NULL);
  return NULL;
}

double *points_distances(RadialPoints *A, RadialPoints *B, int threads) {
  size_t size = min(A->size, B->size);
  double *distances = (double *) malloc(size * sizeof(double));
  if (threads <= 1) {
    compute_distances_block(A, B, distances, 0, size);
  } else {
    pthread_t *workers = (pthread_t *) malloc(threads * sizeof(pthread_t));
    int block_size = (int) (size / min(size, threads));
    RDArguments *arguments = (RDArguments *) malloc(threads * sizeof(RDArguments));
    for (int i = 0; i < threads; i++) {
      arguments[i].first_set  = A;
      arguments[i].second_set = B;
      arguments[i].distances = distances;
      arguments[i].block_offset = min(block_size * i, size - 1);
      arguments[i].block_size = min(block_size, size);
      pthread_create(&workers[i], NULL, radial_distances_process, &arguments[i]);
    }
    for (int i = 0; i < threads; i++) {
      pthread_join(workers[i], NULL);
    }
    free(arguments);
    free(workers);
  }
  return distances;
}

void release_distances(double *distances) {
  free(distances);
}
