#include "SafeBuffer.h"
#include "Event.h"
#include "Semaphore.h"
#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <atomic>

using namespace std;

atomic<int> eventCounter(0);  // Unique event IDs
mutex cout_mutex;             // Synchronise output
mutex buffer_mutex;           // Protect buffer access
const int buffer_size = 20; // Define the buffer size
Semaphore spaces(buffer_size); // Tracks available buffer space
Semaphore items(0);            // Tracks available items in the buffer

const int num_threads = 100;


// Produces events and adds them to the buffer
void producer(shared_ptr<SafeBuffer<shared_ptr<Event>>> theBuffer, int numLoops) {
    for (int i = 0; i < numLoops; ++i) {
        shared_ptr<Event> e = make_shared<Event>(eventCounter++); // Create event

        spaces.Wait();          // Wait for space in the buffer
        buffer_mutex.lock();    // Lock buffer
        theBuffer->put(e);      // Add event to buffer
        buffer_mutex.unlock();  // Unlock buffer
        items.Signal();         // Signal item availability

        // Output production
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Producing event " << e->getID() << endl;
        }
    }
}

// Retrieves events from the buffer and processes them
void consumer(shared_ptr<SafeBuffer<shared_ptr<Event>>> theBuffer, int numLoops) {
    for (int i = 0; i < numLoops; ++i) {
        items.Wait();           // Wait for an available item
        buffer_mutex.lock();    // Lock buffer
        shared_ptr<Event> e = theBuffer->get(); // Retrieve event
        buffer_mutex.unlock();  // Unlock buffer
        spaces.Signal();        // Signal space availability

        // Output consumption
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Consuming event " << e->getID() << endl;
        }

        e->consume();           // Process event
    }
}

int main() {
    vector<thread> threads(num_threads);
    auto buffer = make_shared<SafeBuffer<shared_ptr<Event>>>(buffer_size);

    // Alternate producer and consumer threads
    for (int i = 0; i < num_threads; ++i) {
        if (i < num_threads / 2) {
            threads[i] = thread(producer, ref(buffer), 10);
        }
        else {
            threads[i] = thread(consumer, ref(buffer), 10);
        }
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    cout << "All threads have completed." << endl;
    return 0;
}
