package main

import (
	"fmt"
	"time"
)

func boring(msg string){
	for i:=0; ; i++ {
		fmt.Println(msg, i)
		time.Sleep(time.Second)
	}
}

func main() {
	go boring("boring")
	fmt.Println("I'm listening")
	time.Sleep(2 * time.Second)
	fmt.Println("You are boring, I'm leaving")
}
// we can hang around a little, and on the way show that both the main and the launched goroutine are running

