public final class Server extends Thread {
  private final Value request;
  private final Value response;

  public Server(Value request, Value response) {
    this.request = request;
    this.response = response;
  }

  public void run() {
    while (true) {
      System.out.println("\nWait for a request...");
      while (this.request.value() == 0) {
      }

      final int client = this.request.value();
      System.out.println("Authorize client " + client);
      this.response.update(client);

      System.out.println("Wait release...");
      while (this.response.value() != 0) {
      }

      this.request.update(0);
    }
  }
}
