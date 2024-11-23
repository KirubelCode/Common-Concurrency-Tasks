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

func getForks(index int, forks map[int]chan bool) {
	if index == 0 { // Philosopher 0 picks the right fork first
		forks[(index+1)%5] <- true
		forks[index] <- true
	} else { // Others pick the left fork first
		forks[index] <- true
		forks[(index+1)%5] <- true
	}
}

func putForks(index int, forks map[int]chan bool) {
	<-forks[index] //Release
	<-forks[(index+1)%5]
}

func doPhilStuff(index int, wg *sync.WaitGroup, forks map[int]chan bool) {
	defer wg.Done()
	for {
		think(index)
		getForks(index, forks)
		eat(index)
		putForks(index, forks)
	}
}

func main() {
	var wg sync.WaitGroup
	philCount := 5
	wg.Add(philCount)

	forks := make(map[int]chan bool) // Make a channel
	for i := 0; i < philCount; i++ {
		forks[i] = make(chan bool, 1) // Add forks
	}

	for i := 0; i < philCount; i++ {
		go doPhilStuff(i, &wg, forks)
	}

	wg.Wait()
} // Prevents Deadlock: Philosopher 0's reverse order(right fork first)
// breaks the circular wait condition, ensuring deadlock cannot occur.
