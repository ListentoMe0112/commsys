package main

import (
	"commsys/client/mainWindow"
	_ "fmt"
)

func main() {
	MainWindow := mainWindow.MainWindow{Loop: true}

	MainWindow.MainShow()
}
