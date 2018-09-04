public final class Value {
  private int internal = 0;

  public Value(int value) {
    this.internal = value;
  }

  public int value() {
    synchronized (this) {
      return this.internal;
    }
  }

  public void update(int value) {
    synchronized (this) {
      this.internal = value;
    }
  }
}
