// Dining Philosophers Template Code
// Author: Joseph Kehoe
// Created: 21/10/24
// Maintained by: Kirubel Temesgen
// ID: C00260396
// Purpose: Concurrent execution wihtout deadlock

package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

func think(index int) {
	var X time.Duration
	X = time.Duration(rand.IntN(5))
	time.Sleep(X * time.Second) //wait random time amount
	fmt.Println("Phil: ", index, "was thinking")
}

func eat(index int) {
	var X time.Duration
	X = time.Duration(rand.IntN(5))
	time.Sleep(X * time.Second) //wait random time amount
	fmt.Println("Phil: ", index, "was eating")
}

func getForks(index int, forks map[int]chan bool, footman chan bool) {
	footman <- true // Blocks until theres space else
	// 4 Enter simultaneously (footman allows 4 max)
	forks[index] <- true
	forks[(index+1)%5] <- true // If in use phil waits, else continues
}

func putForks(index int, forks map[int]chan bool, footman chan bool) {
	<-forks[index]       // Releases left fork for another phil
	<-forks[(index+1)%5] // Releases right fork
	<-footman            // Allows the waiting phil to proceed
}

func doPhilStuff(index int, wg *sync.WaitGroup, forks map[int]chan bool, footman chan bool) {
	defer wg.Done()
	for {
		think(index)
		getForks(index, forks, footman)
		eat(index)
		putForks(index, forks, footman)
	}
}

func main() {
	var wg sync.WaitGroup
	philCount := 5
	wg.Add(philCount)

	forks := make(map[int]chan bool) // Make a map of channels
	for k := 0; k < philCount; k++ {
		forks[k] = make(chan bool, 1) // Each fork is its own channel esuring only one
		// can hold it at any given time
	}

	footman := make(chan bool, 4)

	for n := 0; n < philCount; n++ {
		go doPhilStuff(n, &wg, forks, footman)
	}

	wg.Wait()
}
