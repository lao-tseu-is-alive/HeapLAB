package main

import (
	"fmt"
	"runtime"
)

func main() {
	fmt.Printf("Welcome from go version %s", runtime.Version())
}
