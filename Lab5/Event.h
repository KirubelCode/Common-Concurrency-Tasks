/* Event.h --- 
 * 
 * Filename: Event.h
 * Description: 
 * Author: Joseph
 * Maintainer: Kirubel Temesgen
 * Created: Tue Jan  8 12:30:05 2019 (+0000)
 * Version: 2
 * Package-Requires: ()
 * Last-Updated: Tue Jan  8 12:30:14 2019 (+0000)
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
#ifndef EVENT_H
#define EVENT_H

#include <iostream>
using namespace std;

class Event {
private:
    int id;

public:
    Event(int id) : id(id) {}
    void consume() {
        cout << "Consuming event " << id << endl;
    }

    int getID() const
    {
        return id;
    }
};

#endif // EVENT_H


/* Event.h ends here */
