public final class Max100RandomPolarPoints implements PolarPoints {
  private final PolarPoints points;

  public Max100RandomPolarPoints(int size) {
    this.points = new RandomPolarPoints(size, 100);
  }

  public PolarPoint[] value() {
    return this.points.value();
  }

  public int size() {
    return this.points.size();
  }
}
