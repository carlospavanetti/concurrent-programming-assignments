public final class PolarPoint {
  private double magnitude;
  private double direction;

  public PolarPoint(double magnitude, double direction) {
    this.magnitude = magnitude;
    this.direction = direction;
  }

  public double magnitude() {
    return this.magnitude;
  }

  public double direction() {
    return this.direction;
  }

  public double distance(PolarPoint target) {
    return Math.sqrt(
      Math.pow(this.magnitude(), 2) + Math.pow(target.magnitude(), 2) - 2 * (
        (this.magnitude() * target.magnitude())
        * Math.cos(this.direction() - target.direction())
      )
    );
  }
}
