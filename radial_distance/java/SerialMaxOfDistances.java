public final class SerialMaxOfDistances implements MaxOfDistances {
  private final double[] values;

  public SerialMaxOfDistances(double[] values) {
    this.values = values;
  }

  public double value() {
    double possible = this.values[0];
    for (int i = 1; i < this.values.length; i++) {
      if (possible < this.values[i]) {
        possible = this.values[i];
      }
    }
    return possible;
  }
}
