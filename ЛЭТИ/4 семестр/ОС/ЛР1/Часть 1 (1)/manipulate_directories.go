package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"

	"golang.org/x/sys/windows"
)

func ManipulateDirectories() {
	fmt.Println("Do you want to remove (r) or create (c) directory: ")
	var choice string
	for choice != "r" && choice != "c" {
		fmt.Scanln(&choice)
	}
	fmt.Print("Enter directory path: ")
	reader := bufio.NewReader(os.Stdin)
	var directoryPath string
	directoryPath, _ = reader.ReadString('\n')
	directoryPath = strings.TrimRight(directoryPath, "\n\r")
	utf16Path := windows.StringToUTF16(directoryPath)
	switch choice {
	case "c":
		err := windows.CreateDirectory(&utf16Path[0], nil)
		if err != nil {
			fmt.Println("Error creating directory:", err.Error())
		}
	case "r":
		err := windows.RemoveDirectory(&utf16Path[0])
		if err != nil {
			fmt.Println("Error removing directory:", err.Error())
		}
	}
}
