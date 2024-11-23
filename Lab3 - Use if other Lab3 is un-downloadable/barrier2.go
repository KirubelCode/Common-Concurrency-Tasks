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
// Modified by: Kirubel Temesgen
// ID: C00260396
// Issues:
// Prints "Part B" once at the start after the first 'Part A'
// --------------------------------------------
package main

import (
	"context"
	"fmt"
	"sync"
	"time"

	"golang.org/x/sync/semaphore"
)

var totalRoutines = 10
var count int // Counter to track arriving goroutines

// Place a barrier in this function -- use Mutex's and Semaphores
func doStuff(goNum int, wg *sync.WaitGroup, sem *semaphore.Weighted, mutexSem *sync.Mutex, totalRoutines int) {
	defer wg.Done() // Ensure wg.Done() is called when the goroutine ends

	time.Sleep(time.Second)
	fmt.Println("Part A", goNum)

	// Use mutex to safely increment count
	mutexSem.Lock()
	count++
	if count == totalRoutines {
		// Last goroutine to arrive releases all permits
		sem.Release(int64(totalRoutines - 1))
	}
	mutexSem.Unlock()

	// Acquire semaphore to proceed to Part B (wait until all permits are released)
	sem.Acquire(context.TODO(), 1)

	// Print Part B after all goroutines have reached the barrier
	mutexSem.Lock()
	fmt.Println("Part B", goNum)
	mutexSem.Unlock()
}

func main() {
	var wg sync.WaitGroup
	wg.Add(totalRoutines)

	// Semaphore to control access to Part B (starts with 1 permit to allow the first goroutine through)
	sem := semaphore.NewWeighted(int64(totalRoutines)) //ISSUE HERE PERMITS ALL TO GO

	// Mutex to ensure "Part B" is printed in order
	var mutexSem sync.Mutex

	// Launch goroutines
	for i := 0; i < totalRoutines; i++ {
		go doStuff(i, &wg, sem, &mutexSem, totalRoutines)
	}

	// Wait for all goroutines to finish
	wg.Wait()
	fmt.Println("All goroutines finished")
}
