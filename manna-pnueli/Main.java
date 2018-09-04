public final class Main {
  public static void main(String[] arguments) {
    final Server server = new Server();
    final Client[] clients = new Client[10];
    server.start();
    for (int i = 0; i < 10; i++) {
      clients[i] = new Client(i + 1, server);
      clients[i].start();
    }

    while (true) {}
  }
}
