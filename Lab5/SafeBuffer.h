/* SafeBuffer.h --- 
 * 
 * Filename: SafeBuffer.h
 * Description: 
 * Author: Joseph
 * Maintainer: 
 * Created: Tue Jan  8 12:30:23 2019 (+0000)
 * Version: 
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
using namespace std;

template <typename T>
class SafeBuffer {
private:
    queue<T> buffer;                // Queue to hold items in the buffer
    mutex mtx;                      // Mutex to protect access to the buffer

public:
    SafeBuffer() = default;

    void put(const T& item) {
        lock_guard<mutex> lock(mtx); // Lock the buffer before modifying
        buffer.push(item);
    }

    T get() {
        lock_guard<mutex> lock(mtx); // Lock the buffer before accessing
        T item = buffer.front();
        buffer.pop();
        return item;
    }
};

#endif // SAFE_BUFFER_H




/* SafeBuffer.h ends here */
