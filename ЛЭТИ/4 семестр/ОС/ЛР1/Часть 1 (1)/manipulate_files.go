package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
	"syscall"
	"unsafe"

	"golang.org/x/sys/windows"
)

func ManipulateFiles() {
	fmt.Print("Do you want to copy (c) or move (m) file: ")
	var choice, sourcePath, destinationPath string
	for choice != "c" && choice != "m" {
		fmt.Scanln(&choice)
	}
	fmt.Print("Enter source path: ")
	reader := bufio.NewReader(os.Stdin)
	sourcePath, _ = reader.ReadString('\n')
	sourcePath = strings.TrimRight(sourcePath, "\r\n")
	fmt.Print("Enter destination path: ")
	destinationPath, _ = reader.ReadString('\n')
	destinationPath = strings.TrimRight(destinationPath, "\r\n")
	utf16sourcePath, err := windows.UTF16FromString(sourcePath)
	if err != nil {
		fmt.Println("Error encoding source file path:", err.Error())
		return
	}
	utf16destinationPath, err := windows.UTF16FromString(destinationPath)
	if err != nil {
		fmt.Println("Error encoding destination file path:", err.Error())
		return
	}
	switch choice {
	case "c":
		procCopyFile := syscall.NewLazyDLL("kernel32.dll").NewProc("CopyFileW")
		r1, _, err := syscall.SyscallN(
			procCopyFile.Addr(),
			uintptr(unsafe.Pointer(&utf16sourcePath[0])),
			uintptr(unsafe.Pointer(&utf16destinationPath[0])),
			uintptr(1),
		)
		if r1 == 0 {
			fmt.Println("Error copying file:", err.Error())
		}
	case "m":
		err = windows.MoveFileEx(
			&utf16sourcePath[0],
			&utf16destinationPath[0],
			windows.MOVEFILE_COPY_ALLOWED,
		)
		if err != nil {
			fmt.Println("Error moving file:", err.Error())
		}
	}
}
