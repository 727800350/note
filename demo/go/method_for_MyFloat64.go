package main

import (
	"fmt"
	"math"
)

// 如果不用MyFloat 封装一下float64, 就会报错, 因为float64 不是这个package的类型, 而不是math 包中的
type MyFloat float64

func (f MyFloat) Abs() float64 {
	if f < 0 {
		return float64(-f)
	}
	return float64(f)
}

func main() {
	f := MyFloat(-math.Sqrt2)
	fmt.Println(f.Abs())
}

