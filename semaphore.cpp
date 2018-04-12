#include <iostream>
#include <thread>
#include <unistd.h>
#include <mutex>
#include <condition_variable>

// Primitive Semaphore
class Semaphore {
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

  int getCount() {
    return this->count;
  }
};

// thread one function
void threadone(Semaphore* semaphore) {
  for (unsigned int i = 0; i < 7; i++) {
    semaphore->sem_wait();
    sleep(5);
  };
}


// thread two function
void threadtwo(Semaphore* semaphore) {
  for (unsigned int i = 0; i < 2; i++) {
    semaphore->sem_signal();
    sleep(5);
  };
}

int main() {
  Semaphore* semaphore = new Semaphore(5);

  std::thread thread_1(threadone, semaphore);
  std::thread thread_2(threadtwo, semaphore);

  thread_1.join();
  thread_2.join();

  // ...
}
