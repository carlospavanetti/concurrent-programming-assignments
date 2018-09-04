public final class Server extends Thread {
  private final Value request = new Value(0);
  private final Value response = new Value(0);

  public Server() {}

  public void register(Client client) {
    while (this.response.value() != client.hashCode()) {
      this.request.update(client.hashCode());
    }
  }

  public void release(Client client) {
    System.out.println("Release authorization");
    if (this.response.value() == client.hashCode()) {
      this.response.update(0);
    }
  }

  public void run() {
    while (true) {
      this.waitRequest();
      this.authorize();
      this.waitRelease();
      this.resetResquests();
    }
  }

  private void waitRequest() {
    System.out.println("\nWait for a request...");
    while (this.request.value() == 0) {}
  }

  private void authorize() {
    final int client = this.request.value();
    System.out.println("Authorize client " + client);
    this.response.update(client);
  }

  private void waitRelease() {
    System.out.println("Wait release...");
    while (this.response.value() != 0) {}
  }

  private void resetResquests() {
    this.request.update(0);
  }
}
