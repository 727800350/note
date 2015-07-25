package main

import (
	"fmt"
	"time"
	"math/rand"
)

// function returns a string channel
func boring(msg string) <-chan string{
	c := make(chan string)
	go func(){
		for i:=0; ; i++ {
			c <- fmt.Sprintf("%s %d", msg, i)
			time.Sleep(time.Duration(rand.Intn(1e3)) * time.Millisecond)
		}
	}()
	return c
}

func main() {
	c := boring("boring")
	for i:= 0; i < 5 ; i++ {
		fmt.Printf("You say: %q\n", <- c)
	}
	fmt.Println("You are boring, I'm leaving")


}
/**
call the goroutine in the boring function

channels as a handle on a service
Our boring function returns a channel that lets us communicate with the boring service it provides
	joe := boring("Joe")
	ann := boring("Ann")
	for i:= 0; i < 5 ; i++ {
		fmt.Println(<- joe)
		fmt.Println(<- ann)
	}
**/
