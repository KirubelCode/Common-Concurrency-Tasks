#include "Semaphore.h"
#include <thread>
#include <vector>
#include <iostream>
#include <memory>

void task(std::shared_ptr<Semaphore> mutexSem, std::shared_ptr<Semaphore> barrierSem, int threadCount) {
    static int count = 0;

    std::cout << "first " << std::endl;

    // Critical section to safely increment count // E.g 5 threads try to enter
    mutexSem->Wait(); // Access now blocked  // 4 wait 1 goes through T1
    count++;
    if (count == threadCount) {
        // Signal the barrier for each thread, allowing them to proceed
        for (int i = 0; i < threadCount; ++i) {
            barrierSem->Signal(); // Access given
        }
    }
    mutexSem->Signal(); // Access unblocked // The other 4 may proceed

    barrierSem->Wait(); // All threads wait here until the barrier is released // T1 now waits until the barrier is signalled

    std::cout << "second" << std::endl;
}

int main(void) {
    std::shared_ptr<Semaphore> mutexSem = std::make_shared<Semaphore>(1);
    std::shared_ptr<Semaphore> barrierSem = std::make_shared<Semaphore>(0);
    int threadCount = 5;

    std::vector<std::thread> threadArray;

    for (int i = 0; i < threadCount; i++) {
        threadArray.emplace_back(task, mutexSem, barrierSem, threadCount);
    }

    for (auto& t : threadArray) {
        t.join(); // Wait for all threads to complete
    }

    return 0;
}
