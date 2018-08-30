#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#include <pthread.h>

/*
 * pRNG based on http://www.cs.wm.edu/~va/software/park/park.html
 */
#define MODULUS    2147483647
#define MULTIPLIER 48271
#define DEFAULT    123456789

static long seed = DEFAULT;

/* ----------------------------------------------------------------
 * Random returns a pseudo-random real number uniformly distributed
 * between 0.0 and 1.0.
 * ----------------------------------------------------------------
 */
double Random(void) {
  const long Q = MODULUS / MULTIPLIER;
  const long R = MODULUS % MULTIPLIER;
  long t;

  t = MULTIPLIER * (seed % Q) - R * (seed / Q);
  if (t > 0)
    seed = t;
  else
    seed = t + MODULUS;
  return ((double) seed / MODULUS);
}

/*
 * End of the pRNG algorithm
 */

typedef struct {
  double x, y, z;
  double mass;
} Particle;

typedef struct {
  double xold, yold, zold;
  double fx, fy, fz;
} ParticleV;

void InitParticles(Particle[], ParticleV [], int);
double ComputeForces(Particle [], Particle [], ParticleV [], int);
double ComputeNewPositions(Particle [], ParticleV [], int, double);

int threads;
pthread_t *workers;
void AllocateWorkers(int number);
void RunWorkers();

typedef struct {
  Particle *particles;
  ParticleV *pv;
  double a0, a1, a2;
  int npart, id;
} NBArguments;

int main() {
  Particle  * particles;   /* Particulas */
  ParticleV * pv;          /* Velocidade da Particula */
  int         npart;
  double      sim_t;       /* Tempo de Simulacao */

  // scanf("%d", &npart);
  npart = 25000;
  /* Allocate memory for particles */
  particles = (Particle *) malloc(
    npart * sizeof(Particle)
  );
  pv = (ParticleV *) malloc(
    npart * sizeof(ParticleV)
  );
  /* Generate the initial values */
  InitParticles(particles, pv, npart);

  struct timeval inicio, final2;
  int tmili;
  gettimeofday(&inicio, NULL);

  sim_t = 0.0;
  double max_f;

  /* Compute forces (2D only) */
  max_f = ComputeForces(particles, particles, pv, npart);
  /* Once we have the forces, we compute the changes in position */
  sim_t = ComputeNewPositions(particles, pv, npart, max_f);

  gettimeofday(&final2, NULL);
  tmili = (int) (
    1000 * (final2.tv_sec - inicio.tv_sec) +
    (final2.tv_usec - inicio.tv_usec) / 1000
  );

  printf("%g\n", max_f);
  printf("%g\n", sim_t);
  printf("%d\n", tmili);

  free(particles);
  free(pv);

  return 0;
}

void InitParticles(Particle particles[], ParticleV pv[], int npart) {
  for (int i = 0; i < npart; i++) {
    particles[i].x    = Random();
    particles[i].y    = Random();
    particles[i].z    = Random();
    particles[i].mass = 1.0;
    pv[i].xold        = particles[i].x;
    pv[i].yold        = particles[i].y;
    pv[i].zold        = particles[i].z;
    pv[i].fx          = 0;
    pv[i].fy          = 0;
    pv[i].fz          = 0;
  }
}

double ComputeForces(
  Particle myparticles[], Particle others[], ParticleV pv[], int npart
) {
  double max_f = 0.0;
  double xi, yi, rx, ry, mj, r, fx, fy, rmin;

  for (int i = 0; i < npart; i++) {
    rmin = 100.0;
    xi   = myparticles[i].x;
    yi   = myparticles[i].y;
    fx   = 0.0;
    fy   = 0.0;
    for (int j = 0; j < npart; j++) {
      rx = xi - others[j].x;
      ry = yi - others[j].y;
      mj = others[j].mass;
      r  = rx * rx + ry * ry;
      /* ignore overlap and same particle */
      if (r == 0.0) continue;
      if (r < rmin) rmin = r;
      r  = r * sqrt(r);
      fx -= mj * rx / r;
      fy -= mj * ry / r;
    }
    pv[i].fx += fx;
    pv[i].fy += fy;
    fx = sqrt(fx * fx + fy * fy) / rmin;
    if (fx > max_f) max_f = fx;
  }
  return max_f;
}

double ComputeNewPositions(
  Particle particles[], ParticleV pv[], int npart, double max_f
) {
  double a0, a1, a2;
  static double dt_old = 0.001, dt = 0.001;
  double dt_new;
  a0 = 2.0 / (dt * (dt + dt_old));
  a2 = 2.0 / (dt_old * (dt + dt_old));
  a1 = -(a0 + a2);

  AllocateWorkers(4);
  RunWorkers();

  dt_new = 1.0 / sqrt(max_f);
  /* Set a minimum: */
  if (dt_new < 1.0e-6) dt_new = 1.0e-6;
  /* Modify time step */
  if (dt_new < dt) {
    dt_old = dt;
    dt     = dt_new;
  }
  else if (dt_new > 4.0 * dt) {
    dt_old = dt;
    dt    *= 2.0;
  }
  return dt_old;
}

void *process(void *args);
void RunWorkers(Particle* particles, ParticleV *pv, double a0, double a1, double a2, int npart) {
  NBArguments *arguments = (NBArguments *) malloc(threads * sizeof(NBArguments));
  for (int i = 0; i < threads; i++) {
    arguments[i].particles = particles;
    arguments[i].pv = pv;
    arguments[i].a0 = a0;
    arguments[i].a1 = a1;
    arguments[i].a2 = a2;
    arguments[i].npart = npart;
    arguments[i].id = i;
    pthread_create(&workers[i], NULL, process, &arguments[i]);
  }
  for (int i = 0; i < threads; i++) {
    pthread_join(workers[i], NULL);
  }
}
void *process(void *args) {
  NBArguments *arguments = (NBArguments *) args;
  Particle *particles = arguments->particles;
  ParticleV *pv = arguments->pv;
  double a0 = arguments->a0;
  double a1 = arguments->a1;
  double a2 = arguments->a2;
  int npart = arguments->npart;
  int id = arguments->id;

  for (int i = id; i < npart; i += threads) {
    double xi, yi;
    xi             = particles[i].x;
    yi             = particles[i].y;
    particles[i].x = (pv[i].fx - a1 * xi - a2 * pv[i].xold) / a0;
    particles[i].y = (pv[i].fy - a1 * yi - a2 * pv[i].yold) / a0;
    pv[i].xold     = xi;
    pv[i].yold     = yi;
    pv[i].fx       = 0;
    pv[i].fy       = 0;
  }

  return NULL;
}

void AllocateWorkers(int number) {
  threads = threads;
  pthread_t *workers = (pthread_t *) malloc(threads * sizeof(pthread_t));
}
