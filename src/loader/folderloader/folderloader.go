package main

import (
	"fmt"
	"os"
	"strconv"
)

func IsRowEmpty(data []byte, offset int) bool {
	for i := offset; i < offset+16; i++ {
		if data[i] != 0x00 {
			return false
		}
	}
	return true
}

func MoveBack(target, source []byte, size, offset int) {
	j := 0
	for i := offset; i < offset+size; i++ {
		target[i] = source[j]
		j++
	}
}

func MoveTo(target, source []byte, size, offset int) {
	j := 0
	for i := offset; i < offset+size; i++ {
		source[j] = target[i]
		j++
	}
}

func main() {
	img, _ := os.OpenFile("system.img", os.O_RDWR, 0)
	defer img.Close()

	if len(os.Args) < 3 {
		fmt.Println("Usage: %s <sourceFile> <parentIndex>", os.Args[0])
		return
	}

	folderName := os.Args[1]
	parentIndex := os.Args[2]

	if len(folderName) > 13 {
		fmt.Println("Name file is too long. Max 13 characters.")
		return
	}

	data := make([]byte, 2880*512)
	mapData := make([]byte, 512)
	fileData := make([]byte, 1024)
	sectorData := make([]byte, 512)

	img.Read(data)

	mapDataLocation := 512 * 0x100
	fileDataLocation := 512 * 0x101
	sectorDataLocation := 512 * 0x103

	MoveTo(data, mapData, 512, mapDataLocation)
	MoveTo(data, fileData, 1024, fileDataLocation)
	MoveTo(data, sectorData, 512, sectorDataLocation)

	emptyFileRow := -1
	for i := 0; i < 1024; i = i + 16 {
		if IsRowEmpty(fileData, i) {
			emptyFileRow = i
			break
		}
	}
	if emptyFileRow == -1 {
		fmt.Printf("There is not enough space for the files specified. (Files Image Problem)")
		return
	}

	for i := 0; i < 14; i++ {
		fileData[emptyFileRow+i+2] = 0x00
	}

	for i, elem := range folderName {
		fileData[emptyFileRow+i+2] = byte(elem)
	}
	parentIndexInteger, err := strconv.Atoi(parentIndex)

	if err != nil {
		fmt.Println("Parent index is not a valid index")
		return
	}

	fileData[emptyFileRow] = 0xFF
	fileData[emptyFileRow+1] = byte(parentIndexInteger)

	MoveBack(data, mapData, 512, mapDataLocation)
	MoveBack(data, fileData, 1024, fileDataLocation)
	MoveBack(data, sectorData, 512, sectorDataLocation)

	_, werr := img.WriteAt(data, 0)

	if werr != nil {
		fmt.Println(werr)
		return
	}

	fmt.Println("Done!")

}
