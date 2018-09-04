public final class Client extends Thread {
  private final int id;
  private final Value request;
  private final Value response;

  public Client(int id, Value request, Value response) {
    this.id = id;
    this.request = request;
    this.response = response;
  }

  public void run() {
    while (true) {
      requestAuthorization();
      criticalSection();
      releaseAuthorization();
    }
  }

  private void requestAuthorization() {
    System.out.println("Ask authorization " + this.id);
    while (this.response.value() != this.id) {
      this.request.update(this.id);
    }
  }

  private void criticalSection() {
    System.out.println("Use critical section at " + this.id);
  }

  private void releaseAuthorization() {
    System.out.println("Release authorization");
    this.response.update(0);
  }
}
