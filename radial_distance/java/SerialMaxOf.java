public final class SerialMaxOf<
  T extends Comparable<? super T>
> implements MaxOf<T> {
  private final T[] values;

  public SerialMaxOf(T[] values) {
    this.values = values;
  }

  public T value() {
    T possible = this.values[0];
    for (int i = 1; i < this.values.length; i++) {
      if (possible.compareTo(values[i]) < 0) {
        possible = this.values[i];
      }
    }
    return possible;
  }
}
