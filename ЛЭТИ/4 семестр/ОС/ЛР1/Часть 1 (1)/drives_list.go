package main

import (
	"fmt"
	"strings"
	"unicode/utf16"

	"golang.org/x/sys/windows"
)

func DrivesList() {
	var buffer = [STRING_BUFFER_SIZE]uint16{}

	_, err := windows.GetLogicalDriveStrings(STRING_BUFFER_SIZE, &buffer[0])
	if err != nil {
		fmt.Println("Error getting logical drives:", err.Error())
		return
	}
	fmt.Println(
		"Logical drives list:",
		strings.Join(
			strings.FieldsFunc(
				strings.TrimRight(
					string(utf16.Decode(buffer[:])),
					string(rune(0)),
				),
				func(r rune) bool { return r == 0 },
			), " "),
	)
}

/*
func DrivesList_v2() {
	drivesBitMasks, err := windows.GetLogicalDrives()
	if err != nil {
		fmt.Println("Error getting logical drives:", err.Error())
		return
	}
	if drivesBitMasks == 0 {
		fmt.Println("There are no logical drives in system")
	} else {
		fmt.Print("Logical drives list: ")
	}
	is_first := true
	for i := 0; i < 26; i++ {
		if drivesBitMasks&(1<<i) > 0 {
			if is_first {
				is_first = false
				fmt.Print(string(rune('A' + i)))
			} else {
				fmt.Print(", ", string(rune('A'+i)))
			}
		}
	}
	fmt.Println()

}
*/
