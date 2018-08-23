public final class SerialRadialDistances implements RadialDistances {
  private PolarPoint[] references, targets;

  public SerialRadialDistances(PolarPoint[] references, PolarPoint[] targets) {
    this.references = references;
    this.targets = targets;
  }

  public double[] value() {
    final int size = Math.min(this.references.length, this.targets.length);
    double[] distances = new double[size];
    for (int i = 0; i < size; i++) {
      distances[i] = references[i].distance(targets[i]);
    }
    return distances;
  }
}
