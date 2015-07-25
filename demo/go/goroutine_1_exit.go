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
}
// when main returns, the program exits and takes the boring function down with it.

