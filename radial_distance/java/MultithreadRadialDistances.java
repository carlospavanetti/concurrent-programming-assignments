public final class MultithreadRadialDistances implements RadialDistances {
  private final PolarPoint[] references;
  private final PolarPoint[] targets;
  private final int threads;

  public MultithreadRadialDistances(
    PolarPoint[] references, PolarPoint[] targets, int threads
  ) {
    this.references = references;
    this.targets = targets;
    this.threads = threads;
  }

  public double[] value() {
    final Thread[] threads = new Thread[this.threads];
    final int size = Math.min(references.length, targets.length);
    double[] distances = new double[size];
    for (int i = 0; i < threads.length; i++) {
      threads[i] = new Thread(
        new RadialDistancesThread(i, this, distances)
      );
      threads[i].start();
    }
    try {
      for (int i = 0; i < threads.length; i++) {
        threads[i].join();
      }
    } catch (InterruptedException exception) {}
    return distances;
  }

  private final class RadialDistancesThread implements Runnable {
    private final int id;
    private final MultithreadRadialDistances distances;
    private double[] output;

    public RadialDistancesThread(
      int id, MultithreadRadialDistances distances, double[] output
    ) {
      this.id = id;
      this.output = output;
      this.distances = distances;
    }

    public void run() {
      for (
        int i = this.id; i < this.output.length; i += this.distances.threads
      ) {
        this.output[i] = this.distances.references[i].distance(
          this.distances.targets[i]
        );
      }
    }
  }
}
