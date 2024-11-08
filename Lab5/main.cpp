#include "SafeBuffer.h"
#include "Event.h"
#include "Semaphore.h"
#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <atomic>

atomic<int> eventCounter(0);  // Atomic counter for unique event IDs
mutex cout_mutex;  // Mutex for protecting cout output

using namespace std;

static const int num_threads = 100;
const int buffer_size = 20;         // Updated buffer size name to avoid conflict
Semaphore spaces(buffer_size);      // Semaphore to track available buffer space
Semaphore items(0);                 // Semaphore to track available items to consume
mutex buffer_mutex;                 // Mutex to protect the buffer
/*! \fn producer
    \brief Creates events and adds them to buffer
*/
void producer(shared_ptr<SafeBuffer<shared_ptr<Event>>> theBuffer, int numLoops) {
    for (int i = 0; i < numLoops; ++i) {
        spaces.Wait();                               // Wait for an available space in buffer

        int uniqueId = eventCounter++;               // Get a unique event ID
        shared_ptr<Event> e = make_shared<Event>(uniqueId);  // Create a new event

        theBuffer->put(e);                           // Add event to the buffer
        items.Signal();                              // Signal that an item is available

        {
            lock_guard<mutex> lock(cout_mutex);      // Lock cout to prevent jumbled output
            cout << "Producing event " << uniqueId << endl;
        }
    }
}




/*! \fn consumer
    \brief Takes events from buffer and consumes them
*/
void consumer(shared_ptr<SafeBuffer<shared_ptr<Event>>> theBuffer, int numLoops) {
    for (int i = 0; i < numLoops; ++i) {
        items.Wait();                                 // Wait for an item to be available

        shared_ptr<Event> e = theBuffer->get();       // Get the event from the buffer

        spaces.Signal();                              // Signal that space is now available in buffer

        {
            lock_guard<mutex> lock(cout_mutex);       // Lock cout to prevent jumbled output
            cout << "Consuming event " << e->getID() << endl;
        }

        e->consume();                                 // Process (consume) the event
    }
}



int main(void) {
    vector<thread> vt(num_threads);     // Vector to hold threads
    auto aBuffer = make_shared<SafeBuffer<shared_ptr<Event>>>(buffer_size); // Create shared buffer

    /**< Launch the threads */
    int i = 0;
    for (thread& t : vt) {
        if (i < num_threads / 2) {
            t = thread(producer, ref(aBuffer), 10); // Launch producer threads, pass shared buffer by reference
        }
        else {
            t = thread(consumer, ref(aBuffer), 10); // Launch consumer threads, pass shared buffer by reference
        }
        ++i;
    }

    /**< Join the threads with the main thread */
    for (auto& v : vt) {
        v.join(); // Wait for all threads to complete
    }

    cout << "All threads have completed." << endl; // Print a simple message

    return 0;
}
