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
	boring("boring")
}
// the program prints out boring for  infinity times

