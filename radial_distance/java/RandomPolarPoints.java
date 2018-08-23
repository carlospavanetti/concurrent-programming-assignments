import java.util.Random;

public final class RandomPolarPoints implements PolarPoints {
  private final PolarPoint[] points;

  public RandomPolarPoints(int size, int maximum) {
    final Random generator = new Random();
    this.points = new PolarPoint[size];
    for (int i = 0; i < size; i++) {
      this.points[i] = new PolarPoint(
        generator.nextInt(maximum),
        generator.nextInt(360) * Math.PI / 180
      );
    }
  }

  public PolarPoint[] value() {
    return this.points;
  }

  public int size() {
    return this.points.length;
  }
}
