//Barrier.go Template Code
//Copyright (C) 2024 Dr. Joseph Kehoe

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// --------------------------------------------
// Author: Joseph Kehoe (Joseph.Kehoe@setu.ie)
// Created on 30/9/2024
// Modified by: Kirubel Temesgen C00260396
// Description:
// A simple barrier implemented using mutex and unbuffered channel
// Issues:
// None
// --------------------------------------------
package main

import (
	"fmt"
	"sync"
	"sync/atomic"
	"time"
)

// Place a barrier in this function -- use atomic variable instead of Mutex
func doStuff(goNum int, arrived *int32, max int, wg *sync.WaitGroup, theChan chan bool) bool {
	time.Sleep(time.Second)
	fmt.Println("Part A", goNum)
	// All enter and increnent the arrived counter safely
	atomic.AddInt32(arrived, 1)                  // Increment 'arrived' atomically
	if atomic.LoadInt32(arrived) == int32(max) { // Last to arrive - signal others to go
		theChan <- true // Signal to start releasing waiting goroutines
		<-theChan       // Last goroutine waits to ensure orderly release
	} else { // Not all here yet, we wait until signal
		<-theChan       // Each thread waits here until all have arrived
		theChan <- true // Once we get through, send signal to next routine to continue
	}
	atomic.AddInt32(arrived, -1) // Decrement 'arrived' atomically
	fmt.Println("PartB", goNum)  // Print Part B for the goroutine
	wg.Done()                    // Indicating it reached its destination
	return true
}

func main() {
	totalRoutines := 10
	var arrived int32 = 0
	var wg sync.WaitGroup
	wg.Add(totalRoutines)
	// Chan obj
	theChan := make(chan bool)           // Use unbuffered channel in place of semaphore
	for i := 0; i < totalRoutines; i++ { // Create the go Routines here
		go doStuff(i, &arrived, totalRoutines, &wg, theChan) // Launch each goroutine
	}
	wg.Wait() // Wait for everyone to finish before exiting
}
