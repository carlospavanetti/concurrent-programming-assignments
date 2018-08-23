public final class MultithreadMaxOfDistances implements MaxOfDistances {
  private final double[] values;

  public MultithreadMaxOfDistances(double[] values, int threads) {
    this.values = values;
  }

  /**
   * TODO
   */
  public double value() {
    return values[0];
  }
}
