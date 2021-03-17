package main

import (
	"errors"
	"fmt"
	"io/ioutil"
	"os"
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

func FindEmpty(source []byte, max int) (byte, error) {
	var result byte = 0
	for i, elem := range source {
		if elem == 0x00 {
			return result, nil
		}
		result++
		if i > max-1 {
			break
		}
	}
	return 0, errors.New("full")
}

func main() {
	img, _ := os.OpenFile("system.img", os.O_RDWR, 0)
	defer img.Close()

	if len(os.Args) < 2 {
		fmt.Printf("Usage: %s <sourceFile>\n", os.Args[0])
		return
	}

	// Load source file
	fileName := os.Args[1]
	if len(fileName) > 13 {
		fmt.Println("Name file is too long. Max 13 characters.")
		return
	}

	source, err := os.Open(fileName)
	if err != nil {
		fmt.Printf("File %s is not found.\n", fileName)
		return
	}
	sourceData, _ := ioutil.ReadAll(source)
	if len(sourceData) > 8192 {
		fmt.Println("File size cannot be more than 8129 bytes.")
		return
	}

	// Allocate array
	data := make([]byte, 2880*512)
	mapData := make([]byte, 512)
	fileData := make([]byte, 1024)
	sectorData := make([]byte, 512)

	// Read data from image to data array
	img.Read(data)

	// Copy data from image
	mapDataLocation := 512 * 0x100
	fileDataLocation := 512 * 0x101
	sectorDataLocation := 512 * 0x103
	MoveTo(data, mapData, 512, mapDataLocation)
	MoveTo(data, fileData, 1024, fileDataLocation)
	MoveTo(data, sectorData, 512, sectorDataLocation)

	// Find empty file row from fileData
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

	for i, elem := range fileName {
		fileData[emptyFileRow+i+2] = byte(elem)
	}

	// FInd empty sector row from sectorData
	emptySectorRow := -1
	for i := 0; i < 32; i++ {
		if IsRowEmpty(sectorData, i*16) {
			emptySectorRow = i
			break
		}
	}
	if emptySectorRow == -1 {
		fmt.Println("There is not enough space for the file specified. (Sectors Problem)")
	}

	fileData[emptyFileRow] = 0xFF
	fileData[emptyFileRow+1] = byte(emptySectorRow)

	var sectorIndex byte
	var err2 error
	countSectorIdx := 0
	for i, partialData := range sourceData {
		if i%512 == 0 {
			sectorIndex, err2 = FindEmpty(mapData, 256)
			mapData[int(sectorIndex)] = 0xFF
			if err2 != nil {
				fmt.Println("There is not enough space for the file specified. (System Image Is Full)")
				return
			}
			sectorData[emptySectorRow*16+countSectorIdx] = sectorIndex
			countSectorIdx++
			fmt.Printf("#%d * 512 + %d -> %b\n", int(sectorIndex), i, partialData)
		}
		data[int(sectorIndex)*512+(i%512)] = partialData
	}
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
