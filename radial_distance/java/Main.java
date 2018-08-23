public class Main {
  public static void main(String[] args) {
    final RadialDistancesArguments arguments =
      new RadialDistancesArguments(args);

    final PolarPoint[] references = new Max100RandomPolarPoints(
      arguments.size()
    ).value();
    final PolarPoint[] targets = new Max100RandomPolarPoints(
      arguments.size()
    ).value();

    final RadialDistances distances = (
      (arguments.threads() <= 1) ?
      new SerialRadialDistances(references, targets) :
      new MultithreadRadialDistances(references, targets, arguments.threads())
    );

    System.out.println(
      new ExecutionTime(
        (x) -> (
          arguments.maximum() ?
          (
            (arguments.threads() <= 1) ?
            new SerialMaxOfDistances(distances.value()) :
            new MultithreadMaxOfDistances(distances.value(), arguments.threads())
          ) : distances.value()
        )
      ).value()
    );
  }
}
