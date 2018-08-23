import java.util.function.Function;

public final class ExecutionTime {
  private final Function function;

  public ExecutionTime(Function function) {
    this.function = function;
  }

  double value() {
    final long start = System.currentTimeMillis();
    this.function.apply(null);
    return System.currentTimeMillis() - start;
  }
}
