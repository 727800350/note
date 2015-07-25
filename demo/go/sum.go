// [go语言的协程和通道通信](http://ju.outofmemory.cn/entry/49445)

package main
import "fmt"

func sum(values []int, myChan chan int){
	sum := 0
	for _, value := range values{
		sum += value
	}
	//值传到通道
	myChan <- sum 
}

func main(){
	// 包含两个int 值得pipe
	myChan := make(chan int, 2)  

	values := []int {1, 2, 3, 5, 5, 4}

	//协程1
	go sum(values, myChan)  

	//协程2
	go sum(values[:3], myChan) 

	// 从pipe 中取出两个int, 分别赋值给sum1, sum2
	sum1, sum2 := <-myChan, <-myChan 

	fmt.Println("Result:",sum1, sum2, sum1 + sum2)
}

