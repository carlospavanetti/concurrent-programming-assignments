public final class Client extends Thread {
  private final int id;
  private final Server server;

  public Client(int id, Server server) {
    this.id = id;
    this.server = server;
  }

  public void run() {
    while (true) {
      this.requestAuthorization();
      this.criticalSection();
      this.releaseAuthorization();
    }
  }

  private void requestAuthorization() {
    this.server.register(this);
  }

  private void criticalSection() {
    System.out.println("Use critical section at " + this.id);
  }

  private void releaseAuthorization() {
    this.server.release(this);
  }
}
