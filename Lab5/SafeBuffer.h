/* SafeBuffer.h --- 
 * 
 * Filename: SafeBuffer.h
 * Description: 
 * Author: Joseph
 * Maintainer: Kirubel Temesgen
 * Created: Tue Jan  8 12:30:23 2019 (+0000)
 * Version: 2
 * Package-Requires: ()
 * Last-Updated: Tue Jan  8 12:30:25 2019 (+0000)
 *           By: Joseph
 *     Update #: 1
 * URL: 
 * Doc URL: 
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change Log:
 * 
 * 
 */

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Code: */
#ifndef SAFE_BUFFER_H
#define SAFE_BUFFER_H

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class SafeBuffer {
private:
    std::queue<T> buffer;                // Queue to hold items in the buffer
    std::mutex mtx;                      // Mutex to protect access to the buffer
    std::condition_variable not_empty;   // Condition variable for empty buffer
    std::condition_variable not_full;    // Condition variable for full buffer
    size_t max_size;                     // Maximum size of the buffer

public:
    SafeBuffer(size_t size) : max_size(size) {}

    void put(const T& item) {
        std::unique_lock<std::mutex> lock(mtx);          // Lock the buffer
        not_full.wait(lock, [this]() {                  // Wait until buffer is not full
            return buffer.size() < max_size;
            });
        buffer.push(item);                              // Add item to the buffer
        not_empty.notify_one();                         // Notify a waiting consumer
    }

    T get() {
        std::unique_lock<std::mutex> lock(mtx);          // Lock the buffer
        not_empty.wait(lock, [this]() {                 // Wait until buffer is not empty
            return !buffer.empty();
            });
        T item = buffer.front();                        // Get the front item
        buffer.pop();                                   // Remove it from the buffer
        not_full.notify_one();                          // Notify a waiting producer
        return item;
    }
};

#endif // SAFE_BUFFER_H







/* SafeBuffer.h ends here */
