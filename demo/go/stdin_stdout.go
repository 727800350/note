package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
)

func main() {
	inputReader := bufio.NewReader(os.Stdin)
	for {
		inputString, readerError := inputReader.ReadString('\n')
		if readerError == io.EOF {
			return
		}
		fmt.Printf(inputString)
	}
}

