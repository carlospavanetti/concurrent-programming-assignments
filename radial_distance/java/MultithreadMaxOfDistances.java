public final class MultithreadMaxOfDistances implements MaxOfDistances {
  private final double[] values;
  private final int threads;

  public MultithreadMaxOfDistances(double[] values, int threads) {
    this.values = values;
    this.threads = threads;
  }

  public double value() {
    final Thread[] threads = new Thread[this.threads];
    double[] possibles = new double[this.threads];
    for (int i = 0; i < threads.length; i++) {
      threads[i] = new Thread(new MaxOfDistancesThread(i, this, possibles));
      threads[i].start();
    }
    try {
      for (int i = 0; i < threads.length; i++) {
        threads[i].join();
      }
    } catch (InterruptedException exception) {}

    double possible = this.values[0];
    for (int i = 0; i < possibles.length; i++) {
      if (possible < possibles[i]) {
        possible = possibles[i];
      }
    }
    return possible;
  }

  private final class MaxOfDistancesThread implements Runnable {
    private final int id;
    private final MultithreadMaxOfDistances maxOf;
    private double[] output;

    public MaxOfDistancesThread(
      int id, MultithreadMaxOfDistances parent, double[] output
    ) {
      this.maxOf = parent;
      this.id = id;
      this.output = output;
    }

    public void run() {
      double possible = this.maxOf.values[this.id];
      for (int i = this.id; i < this.maxOf.values.length; i += this.maxOf.threads) {
        if (possible < this.maxOf.values[i]) {
          possible = this.maxOf.values[i];
        }
      }
      this.output[this.id] = possible;
    }
  }
}
