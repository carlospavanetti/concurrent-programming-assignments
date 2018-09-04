public final class Main {
  public static void main(String[] arguments) {
    final Value request = new Value(0);
    final Value response = new Value(0);
    final Server server = new Server(request, response);
    final Client[] clients = new Client[10];
    server.start();
    for (int i = 0; i < 10; i++) {
      clients[i] = new Client(i + 1, request, response);
      clients[i].start();
    }

    while (true) {}
  }
}
