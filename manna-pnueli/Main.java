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

class Server extends Thread {
  private final Value request;
  private final Value response;

  public Server(Value request, Value response) {
    this.request = request;
    this.response = response;
  }

  public void run() {
    while (true) {
      System.out.println("Wait for a request...");
      while (this.request.value() == 0) { }

      final int client = this.request.value();
      System.out.println("Authorize client " + client);
      this.theresOne = true;
      this.response.update(client);

      System.out.println("Wait release...");
      while (this.response.value() != 0) {}

      this.request.update(0);
    }
  }
}

class Client extends Thread {
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
      System.out.println("Ask authorization");
      while (this.response.value() != this.id) {
        this.request.update(this.id);
      }

      System.out.println("Use critical section at " + this.id);

      System.out.println("Release authorization");
      this.response.update(0);
    }
  }
}

class Value {
  private int internal = 0;

  public Value(int value) {
    this.internal = value;
  }

  public int value() {
    synchronized(this) {
      return this.internal;
    }
  }

  public void  update(int value) {
    synchronized(this) {
      this.internal = value;
    }
  }
}
