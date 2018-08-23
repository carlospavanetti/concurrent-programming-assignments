public final class RadialDistancesArguments {
  public RadialDistancesArguments(String[] arguments) {
    this.arguments = arguments;
  }

  public int size() {
    for (String argument : arguments) {
      if (argument.startsWith("-size=")) {
        return Integer.parseInt(argument.substring(6));
      }
    }
    return 0;
  }

  public int threads() {
    for (String argument : arguments) {
      if (argument.startsWith("-threads=")) {
        return Integer.parseInt(argument.substring(9));
      }
    }
    return 1;
  }

  private final String[] arguments;
}
