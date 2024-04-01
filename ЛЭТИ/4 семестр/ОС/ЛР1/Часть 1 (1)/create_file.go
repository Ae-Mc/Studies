package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"

	"golang.org/x/sys/windows"
)

func CreateFile() {
	fmt.Print("Enter file path: ")
	var filePath string
	reader := bufio.NewReader(os.Stdin)
	filePath, _ = reader.ReadString('\n')
	filePath = strings.TrimRight(filePath, "\r\n")
	utf16path, err := windows.UTF16FromString(filePath)
	if err != nil {
		fmt.Println("Error converting path to utf16:", err.Error())
		return
	}
	handle, err := windows.CreateFile(
		&utf16path[0],
		0,
		0,
		nil,
		windows.CREATE_NEW,
		windows.FILE_ATTRIBUTE_NORMAL,
		windows.Handle(windows.GetShellWindow()),
	)
	if err != nil {
		fmt.Println("Error creating file:", err.Error())
		return
	}
	windows.CloseHandle(handle)
}
