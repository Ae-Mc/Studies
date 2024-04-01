package main

import (
	"bufio"
	"encoding/hex"
	"fmt"
	"os"
	"strings"
	"syscall"
	"time"

	"golang.org/x/sys/windows"
)

func ManipulateFileAttributes() {
	fmt.Print("Enter file path: ")
	var filePath string
	reader := bufio.NewReader(os.Stdin)
	filePath, _ = reader.ReadString('\n')
	filePath = strings.TrimRight(filePath, "\r\n")
	utf16filePath, err := windows.UTF16FromString(filePath)
	if err != nil {
		fmt.Println("Error converting file path:", err.Error())
		return
	}
	attributes, err := windows.GetFileAttributes(&utf16filePath[0])
	if err != nil {
		fmt.Println("Error getting file attributes:", err.Error())
		return
	}
	ATTRIBUTES_MAP := map[int]string{
		0x00000001: "FILE_ATTRIBUTE_READONLY",
		0x00000002: "FILE_ATTRIBUTE_HIDDEN",
		0x00000004: "FILE_ATTRIBUTE_SYSTEM",
		0x00000010: "FILE_ATTRIBUTE_DIRECTORY",
		0x00000020: "FILE_ATTRIBUTE_ARCHIVE",
		0x00000040: "FILE_ATTRIBUTE_DEVICE",
		0x00000080: "FILE_ATTRIBUTE_NORMAL",
		0x00000100: "FILE_ATTRIBUTE_TEMPORARY",
		0x00000200: "FILE_ATTRIBUTE_SPARSE_FILE",
		0x00000400: "FILE_ATTRIBUTE_REPARSE_POINT",
		0x00000800: "FILE_ATTRIBUTE_COMPRESSED",
		0x00001000: "FILE_ATTRIBUTE_OFFLINE",
		0x00002000: "FILE_ATTRIBUTE_NOT_CONTENT_INDEXED",
		0x00004000: "FILE_ATTRIBUTE_ENCRYPTED",
		0x00008000: "FILE_ATTRIBUTE_INTEGRITY_STREAM",
		0x00010000: "FILE_ATTRIBUTE_VIRTUAL",
		0x00020000: "FILE_ATTRIBUTE_NO_SCRUB_DATA",
		0x00080000: "FILE_ATTRIBUTE_PINNED",
		0x00100000: "FILE_ATTRIBUTE_UNPINNED",
		0x00040000: "FILE_ATTRIBUTE_RECALL_ON_OPEN",
		0x00400000: "FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS",
	}
	print_attributes := func(attributes uint32) {
		for key, value := range ATTRIBUTES_MAP {
			if int(attributes)&key != 0 {
				fmt.Println("   ", value)
			}
		}
	}

	fmt.Printf("File attributes: 0x%X\n", attributes)
	print_attributes(attributes)

	get_file_handle := func(access_rights uint32) (windows.Handle, error) {
		return windows.CreateFile(
			&utf16filePath[0],
			access_rights,
			windows.FILE_SHARE_READ,
			nil,
			windows.OPEN_EXISTING,
			windows.FILE_FLAG_BACKUP_SEMANTICS,
			windows.Handle(windows.GetShellWindow()),
		)
	}

	handle, err := get_file_handle(windows.GENERIC_READ)
	if err != nil {
		fmt.Println("Error opening file:", err.Error())
		return
	}
	var (
		creationTimestamp   windows.Filetime
		lastAccessTimestamp windows.Filetime
		lastWriteTimestamp  windows.Filetime
	)
	err = windows.GetFileTime(
		handle,
		&creationTimestamp,
		&lastAccessTimestamp,
		&lastWriteTimestamp,
	)
	if err != nil {
		fmt.Println("Error reading file times:", err.Error())
		return
	}
	creationTime, lastAccessTime, lastWriteTime := time.Unix(
		0,
		creationTimestamp.Nanoseconds(),
	), time.Unix(
		0,
		lastAccessTimestamp.Nanoseconds(),
	), time.Unix(
		0,
		lastWriteTimestamp.Nanoseconds(),
	)
	fmt.Println("Creation time: ", creationTime)
	fmt.Println("Last access time: ", lastAccessTime)
	fmt.Println("Last write time: ", lastWriteTime)
	var choice string
	windows.CloseHandle(handle)
	fmt.Print("Do you want to change attributes (y/n)? ")
	fmt.Scanln(&choice)
	if choice == "y" {
		fmt.Print("Enter new attributes mask (hex without prefix): ")
		var hex_attributes string
		fmt.Scanln(&hex_attributes)
		if len(hex_attributes)%2 == 1 {
			hex_attributes = "0" + hex_attributes
		}
		bytes_attr, err := hex.DecodeString(hex_attributes)
		if err != nil {
			fmt.Println("Error! Malformed hex string:", err.Error())
			return
		}
		attributes = 0
		for i, val := range bytes_attr {
			attributes |= uint32(val) << ((len(bytes_attr) - i - 1) * 8)
		}
		fmt.Printf("Resulting attributes (0x%X):\n", attributes)
		print_attributes(attributes)
		err = windows.SetFileAttributes(&utf16filePath[0], attributes)
		if err != nil {
			fmt.Println("Error changing file attributes:", err.Error())
			return
		}
	}
	fmt.Print("Do you want to change file times (y/n)? ")
	fmt.Scanln(&choice)
	if choice == "y" {
		handle, err = get_file_handle(windows.FILE_WRITE_ATTRIBUTES)
		if err != nil {
			fmt.Println("Error opening file:", err.Error())
			return
		}
		fmt.Println(
			"Enter creation, last access and last write times in format YYYY-MM-DDThh-mm-ss separated by space.",
		)
		fmt.Println("To skip changing one of values enter 0 instead:")
		timeStrings := make([]string, 3)
		_, err = fmt.Scanln(&timeStrings[0], &timeStrings[1], &timeStrings[2])
		if err != nil {
			fmt.Println("Error! Wrong times count:", err.Error())
			windows.CloseHandle(handle)
			return
		}
		Filetime := func(t time.Time) windows.Filetime {
			syscallFiletime := syscall.NsecToFiletime(t.UnixNano())
			return windows.Filetime{
				LowDateTime:  syscallFiletime.LowDateTime,
				HighDateTime: syscallFiletime.HighDateTime,
			}
		}
		timestamps := []*windows.Filetime{
			&creationTimestamp,
			&lastAccessTimestamp,
			&lastWriteTimestamp,
		}
		for i, timeString := range timeStrings {
			if timeString != "0" {
				tempTime, err := time.Parse("2006-01-02T15:04:05", timeString)
				if err == nil {
					*timestamps[i] = Filetime(tempTime)
				} else {
					fmt.Printf("Wrong time format: %s\n", timeString)
				}
			}
		}

		err = windows.SetFileTime(
			handle,
			&creationTimestamp,
			&lastAccessTimestamp,
			&lastWriteTimestamp,
		)
		windows.CloseHandle(handle)
		if err != nil {
			fmt.Println("Error changing file times:", err.Error())
			return
		}
	}
}
