public final class MultithreadMaxOf<
  T extends Comparable<? super T>
> implements MaxOf<T> {
  private final T[] values;

  public MultithreadMaxOf(T[] values, int threads) {
    this.values = values;
  }

  /**
   * TODO
   */
  public T value() {

  }
}
