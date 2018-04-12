
  // Primitive Semaphore
  class Primitive_Semaphore {
  private:
    std::mutex mtx;
    std::condition_variable conditionVariable;
    // count is shared and should be protected
    mutable int count;

  public:
    explicit Semaphore(const int count)
    : count(count) {};

    void sem_wait() {
      std::unique_lock<std::mutex> locker(mtx);
      this->count--;
      if (this->count < 0) {
        // block the process
        conditionVariable.wait(locker);
      }
      // ...
    }

    void sem_signal() {
      std::unique_lock<std::mutex> locker(mtx);
      this->count++;
      if (this->count <= 0) {
        // unblock a process
        conditionVariable.notify_one();
      }
      // ...
    }
  };
