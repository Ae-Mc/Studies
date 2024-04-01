package main

import (
	"fmt"
)

const STRING_BUFFER_SIZE = 1024

type VolumeInformation struct {
	volumeNameBuffer       [STRING_BUFFER_SIZE]uint16
	volumeNameSerialNumber uint32
	maximumComponentLength uint32
	fileSystemFlags        uint32
	fileSystemNameBuffer   [STRING_BUFFER_SIZE]uint16
}

func main() {
	var choice string
	for choice != "exit" && choice != "e" {
		fmt.Println("Choose option")
		fmt.Println("1. Print logical drives list")
		fmt.Println("2. Print logical drive info")
		fmt.Println("3. Manipulate directories")
		fmt.Println("4. Create file")
		fmt.Println("5. Manipulate files")
		fmt.Println("6. Manipulate file attributes")
		fmt.Println("e. Exit")
		_, err := fmt.Scanln(&choice)
		if err != nil {
			fmt.Println(err.Error())
		}
		switch choice {
		case "1":
			DrivesList()
		case "2":
			DriveInfo()
		case "3":
			ManipulateDirectories()
		case "4":
			CreateFile()
		case "5":
			ManipulateFiles()
		case "6":
			ManipulateFileAttributes()
		}
	}
}
