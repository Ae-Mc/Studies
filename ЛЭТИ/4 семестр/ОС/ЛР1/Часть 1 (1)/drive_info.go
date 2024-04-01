package main

import (
	"fmt"
	"unicode/utf16"

	"golang.org/x/sys/windows"
)

func DriveInfo() {
	var drivePath string
	fmt.Print("Enter drive path (don't forget :\\ on the end of the path): ")
	fmt.Scanln(&drivePath)
	uint16DrivePath, err := windows.UTF16PtrFromString(drivePath)
	if err != nil {
		fmt.Println("Error converting string to utf16:", err.Error())
		return
	}
	if driveType(uint16DrivePath) == 1 {
		return
	}
	volumeInfo(uint16DrivePath)
	totalNumberOfFreeBytes, totalNumberOfBytes := driveFreeSpace(
		uint16DrivePath,
	)
	fmt.Printf(
		"Free space: %.3f/%.3f GB\n",
		float64(totalNumberOfFreeBytes)/(1<<30),
		float64(totalNumberOfBytes)/(1<<30),
	)
}

func driveType(drivePath *uint16) int {
	types := []string{
		"Unknown drive type (0)",
		"Wrong path (1)",
		"Removable drive (2)",
		"Fixed drive (3)",
		"Remote drive (4)",
		"CD-Rom (5)",
		"RAM-Disk (6)",
	}
	driveType := windows.GetDriveType(drivePath)
	if driveType >= 0 && int(driveType) <= len(types) {
		fmt.Println(types[driveType])
	} else {
		fmt.Printf("Unknown drive type (%d)", driveType)
	}
	return int(driveType)
}

func volumeInfo(drivePath *uint16) VolumeInformation {
	var info VolumeInformation
	err := windows.GetVolumeInformation(
		drivePath,
		&info.volumeNameBuffer[0],
		uint32(STRING_BUFFER_SIZE),
		&info.volumeNameSerialNumber,
		&info.maximumComponentLength,
		&info.fileSystemFlags,
		&info.fileSystemNameBuffer[0],
		STRING_BUFFER_SIZE,
	)
	if err != nil {
		fmt.Println("Error getting drive info:", err.Error())
	}

	fmt.Println("Volume name:", string(utf16.Decode(info.volumeNameBuffer[:])))
	fmt.Printf("Volume serial number: %X\n", info.volumeNameSerialNumber)
	fmt.Println("Maximum path component length:", info.maximumComponentLength)
	fmt.Println(
		"File system:",
		string(utf16.Decode(info.fileSystemNameBuffer[:])),
	)
	fileSystemFlags := fmt.Sprintf("%X", info.fileSystemFlags)
	fileSystemFlags = "0x" + "00000000"[:8-len(fileSystemFlags)] + fileSystemFlags
	fmt.Println("File system flags:", fileSystemFlags)
	FLAGS_MAP := map[int]string{
		0x00000001: "FILE_CASE_SENSITIVE_SEARCH",
		0x00000002: "FILE_CASE_PRESERVED_NAMES",
		0x00000004: "FILE_UNICODE_ON_DISK",
		0x00000008: "FILE_PERSISTENT_ACLS",
		0x00000010: "FILE_FILE_COMPRESSION",
		0x00000020: "FILE_VOLUME_QUOTAS",
		0x00000040: "FILE_SUPPORTS_SPARSE_FILES",
		0x00000080: "FILE_SUPPORTS_REPARSE_POINTS",
		0x00000100: "FILE_SUPPORTS_REMOTE_STORAGE",
		0x00000200: "FILE_RETURNS_CLEANUP_RESULT_INFO",
		0x00000400: "FILE_SUPPORTS_POSIX_UNLINK_RENAME",
		0x00008000: "FILE_VOLUME_IS_COMPRESSED",
		0x00010000: "FILE_SUPPORTS_OBJECT_IDS",
		0x00020000: "FILE_SUPPORTS_ENCRYPTION",
		0x00040000: "FILE_NAMED_STREAMS",
		0x00080000: "FILE_READ_ONLY_VOLUME",
		0x00100000: "FILE_SEQUENTIAL_WRITE_ONCE",
		0x00200000: "FILE_SUPPORTS_TRANSACTIONS",
		0x00400000: "FILE_SUPPORTS_HARD_LINKS",
		0x00800000: "FILE_SUPPORTS_EXTENDED_ATTRIBUTES",
		0x01000000: "FILE_SUPPORTS_OPEN_BY_FILE_ID",
		0x02000000: "FILE_SUPPORTS_USN_JOURNAL",
		0x04000000: "FILE_SUPPORTS_INTEGRITY_STREAMS",
		0x08000000: "FILE_SUPPORTS_BLOCK_REFCOUNTING",
		0x10000000: "FILE_SUPPORTS_SPARSE_VDL",
		0x20000000: "FILE_DAX_VOLUME",
		0x40000000: "FILE_SUPPORTS_GHOSTING",
	}

	print_flags := func(flags uint32) {
		for key, value := range FLAGS_MAP {
			if int(flags)&key != 0 {
				fmt.Println("   ", value)
			}
		}
	}
	print_flags(info.fileSystemFlags)
	return info
}

func driveFreeSpace(
	drivePath *uint16,
) (totalNumberOfFreeBytes uint64, totalNumberOfBytes uint64) {
	var freeBytesAvailableToCaller uint64
	err := windows.GetDiskFreeSpaceEx(
		drivePath,
		&freeBytesAvailableToCaller,
		&totalNumberOfBytes,
		&totalNumberOfFreeBytes,
	)
	if err != nil {
		fmt.Println("Error getting drive free space:", err.Error())
	}
	return
}
