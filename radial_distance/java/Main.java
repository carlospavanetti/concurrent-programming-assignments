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

    System.out.println(
      new ExecutionTime(
        (x) -> (
          (arguments.threads() > 1) ?
          new MultithreadRadialDistances(
            references, targets, arguments.threads()
          ).value() : new SerialRadialDistances(references, targets).value()
        )
      ).value()
    );
  }
}
