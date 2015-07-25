package main

import (
	"fmt"
	"time"
	"math/rand"
)

func boring(msg string, c chan string){
	for i:=0; ; i++ {
		c <- fmt.Sprintf("%s %d", msg, i)
		time.Sleep(time.Duration(rand.Intn(1e3)) * time.Millisecond)
	}
}

func main() {
	c := make(chan string)
	go boring("boring!", c)
	for i:= 0; i < 5 ; i++ {
		fmt.Printf("You say: %q\n", <- c)
	}
	fmt.Println("You are boring, I'm leaving")
}
/**
a channel connects the main and boring goroutines so they can communicate
when the main function executes <- c, it will wait for a value to be sent
similarly, when the boring function executs c <- value, it waits for a receiver to be ready.
A sender and receiver must both be ready to play their part in the communication. Otherwise we wait unitl they are.
Thus channels both communicate and synchronize
**/
