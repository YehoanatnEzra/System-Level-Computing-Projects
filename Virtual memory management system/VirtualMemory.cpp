#include "VirtualMemory.h"
#include "PhysicalMemory.h"

#define ADDR_VALIDATE if (virtualAddress >= VIRTUAL_MEMORY_SIZE) return 0;
#define LOG_PAGESIZE OFFSET_WIDTH

void dfs(uint64_t index, uint64_t virtual_index, uint64_t path, uint64_t in_use,
         int num_rows, uint64_t *parent_type1, uint64_t *solution_type1, uint64_t *solution_type2,
         uint64_t *parent_type3, uint64_t *solution_type3, uint64_t *virtual_index_type3);
uint64_t findFrameToEvict(uint64_t virtualAddr, uint64_t framesInUse);
uint64_t generatePhysicalAddr(uint64_t virtualAddress);


void VMinitialize() {
    for (uint64_t i = 0; i < PAGE_SIZE; ++i) {
        PMwrite(i, 0);
    }
}

int VMread(uint64_t virtualAddress, word_t *value) {
    ADDR_VALIDATE;
    uint64_t physicalAddress = generatePhysicalAddr(virtualAddress);
    PMread(physicalAddress, value);

    return 1;
}

int VMwrite(uint64_t virtualAddress, word_t value) {
    ADDR_VALIDATE
    uint64_t physicalAddress = generatePhysicalAddr(virtualAddress);
    PMwrite(physicalAddress, value);
    return 1;
}

uint64_t generatePhysicalAddr(uint64_t virtualAddress) {
    uint64_t offset = 0;
    word_t addr1 = 0;
    uint64_t nextFrame = 0;
    offset = virtualAddress % PAGE_SIZE;
    uint64_t currentBaseAddress = 0;
    for (int d = 0; d < TABLES_DEPTH; ++d) {
        uint64_t pageIndex = (virtualAddress >> ((TABLES_DEPTH - d) * OFFSET_WIDTH)) % (PAGE_SIZE);

        PMread(currentBaseAddress + pageIndex, &addr1);
        if (addr1 != 0) {
            nextFrame = addr1;
            currentBaseAddress = PAGE_SIZE * addr1;
        } else {
            uint64_t evicted = findFrameToEvict(virtualAddress, nextFrame);
            if (d != TABLES_DEPTH - 1) { // reached tree leaves - can restore evicted
                for (uint64_t i = 0; i < PAGE_SIZE; ++i) {
                    PMwrite(evicted * PAGE_SIZE + i, 0);
                }
            } else {
                PMrestore(evicted, virtualAddress >> OFFSET_WIDTH);
            }
            addr1 = evicted;
            PMwrite(pageIndex + currentBaseAddress, evicted);

            nextFrame = addr1;
            currentBaseAddress = addr1 * PAGE_SIZE;
        }
    }
    return PAGE_SIZE * addr1 + offset;
}

uint64_t findFrameToEvict(uint64_t virtualAddr, uint64_t framesInUse) {
    int numLayers = CEIL((((double) VIRTUAL_ADDRESS_WIDTH - OFFSET_WIDTH) / LOG_PAGESIZE));
    uint64_t currentFrame = 0;
    uint64_t initialFrame = 0;
    uint64_t evictionCandidate1 = 0;
    uint64_t evictionCandidate3 = 0;
    uint64_t parentCandidate3 = 0;
    uint64_t parentCandidate1 = 0;
    uint64_t evictionPath3 = 0;

    dfs(initialFrame, virtualAddr, initialFrame, framesInUse, numLayers,
        &parentCandidate1, &evictionCandidate1, &currentFrame,
        &parentCandidate3, &evictionCandidate3, &evictionPath3);

    if (evictionCandidate1 == 0) {
        if (currentFrame != NUM_FRAMES - 1) {
            return currentFrame + 1;
        }

        if (evictionCandidate3 != 0) {
            for (uint64_t i = 0; i < PAGE_SIZE; i++) {
                word_t value = 0;
                PMread((parentCandidate3 << LOG_PAGESIZE) + i, &value);
                if ((uint64_t) value != evictionCandidate3) {
                    continue;
                }
                PMwrite((parentCandidate3 << LOG_PAGESIZE) + i, 0);
                break;
            }
            PMevict(evictionCandidate3, evictionPath3);
            return evictionCandidate3;
        }

        return (uint64_t) -1;
    }

    for (uint64_t i = 0; i < PAGE_SIZE; i++) {
        word_t value = 0;
        PMread((parentCandidate1 << LOG_PAGESIZE) + i, &value);
        if ((uint64_t) value != evictionCandidate1) {
            continue;
        }
        PMwrite((parentCandidate1 << LOG_PAGESIZE) + i, 0);
        break;
    }

    return evictionCandidate1;
}

uint64_t calculateFowardDist(uint64_t first, uint64_t second) {
    uint64_t secondFirst = second - first;
    uint64_t firstSecond = first - second;
    return (secondFirst) * ((secondFirst) / (secondFirst + 1) + (firstSecond) / (firstSecond + 1));
}

uint64_t calculateDistance(uint64_t first, uint64_t second) {

    uint64_t forwardDist = calculateFowardDist(first, second);
    uint64_t backwardDist = NUM_PAGES - forwardDist;

    if (forwardDist >= backwardDist) {
        return backwardDist;
    }
    return forwardDist;
}


void readPageAndCheckEmpty(uint64_t nodeIndex, uint64_t i, bool &isEmpty, word_t &readValue) {
    PMread((nodeIndex << LOG_PAGESIZE) + i, &readValue);
    if (readValue != 0) {
        isEmpty = false;
    }
}

void processPageEntry(uint64_t nodeIndex, uint64_t vIndex, uint64_t currentPath, uint64_t activeIndex, int depth,
                      uint64_t i, bool &isEmpty, uint64_t &tempSol1, uint64_t &tempParent1, uint64_t &tempSol2,
                      uint64_t &tempSol3, uint64_t &tempParent3, uint64_t &tempVIndex3) {
    word_t readValue;
    readPageAndCheckEmpty(nodeIndex, i, isEmpty, readValue);
    if (readValue != 0) {
        dfs(readValue, vIndex, (currentPath << LOG_PAGESIZE) + i, activeIndex, depth - 1,
            &tempParent1, &tempSol1, &tempSol2, &tempParent3, &tempSol3, &tempVIndex3);
    }
}

void updateSolutions(uint64_t nodeIndex, uint64_t vIndex, uint64_t tempParent1, uint64_t tempSol1,
                     uint64_t tempSol2, uint64_t &maxSol2, uint64_t tempSol3, uint64_t tempParent3,
                     uint64_t tempVIndex3, uint64_t &maxSol3, uint64_t &vSol,
                     uint64_t *outParent1, uint64_t *outSol1, uint64_t *outParent3) {
    if (tempSol1 != 0) {
        if (tempParent1 == 0)
            tempParent1 = nodeIndex;
        *outParent1 = tempParent1;
        *outSol1 = tempSol1;
        return;
    }

    if (tempSol2 != 0) {
        if(maxSol2<tempSol2){
            maxSol2=tempSol2;
        }
    }

    if (tempSol3 != 0) {
        if (maxSol3 == 0) {
            if (tempParent3 == 0) {
                tempParent3 = nodeIndex;
            }
            *outParent3 = tempParent3;
            vSol = tempVIndex3;
            maxSol3 = tempSol3;
        } else {
            uint64_t distCurrent = calculateDistance(vSol, vIndex);
            uint64_t distTemp = calculateDistance(tempVIndex3, vIndex);
            if (distCurrent < distTemp) {
                if (tempParent3 == 0) {
                    tempParent3 = nodeIndex;
                }
                *outParent3 = tempParent3;
                vSol = tempVIndex3;
                maxSol3 = tempSol3;
            }
        }
    }
}

void finalizeSolutionsWhenDepthZero(uint64_t nodeIndex, uint64_t currentPath,
                                    uint64_t *outSol2, uint64_t *outSol3, uint64_t *outVIndex3) {
    *outSol2 = nodeIndex;
    *outSol3 = nodeIndex;
    *outVIndex3 = currentPath;
}


void dfs(uint64_t nodeIndex, uint64_t vIndex, uint64_t currentPath, uint64_t activeIndex,
         int depth, uint64_t *outParent1, uint64_t *outSol1, uint64_t *outSol2,
         uint64_t *outParent3, uint64_t *outSol3, uint64_t *outVIndex3) {

    if (depth != 0) {
        bool isEmpty = true;
        uint64_t maxSol2 = nodeIndex;
        uint64_t vSol = 0;
        uint64_t maxSol3 = 0;

        for (uint64_t i = 0; i < PAGE_SIZE; i++) {
            uint64_t tempSol1 = 0;
            uint64_t tempParent1 = 0;
            uint64_t tempSol2 = 0;
            uint64_t tempSol3 = 0;
            uint64_t tempVIndex3 = 0;
            uint64_t tempParent3 = 0;

            processPageEntry(nodeIndex, vIndex, currentPath, activeIndex, depth, i,
                             isEmpty, tempSol1, tempParent1, tempSol2, tempSol3, tempParent3, tempVIndex3);

            if (tempSol1 != 0 || tempSol2 != 0 || tempSol3 != 0) {
                updateSolutions(nodeIndex, vIndex, tempParent1, tempSol1, tempSol2,
                                maxSol2, tempSol3, tempParent3, tempVIndex3,
                                maxSol3, vSol, outParent1, outSol1, outParent3);
                if (tempSol1 != 0) {
                    return;
                }
            }
        }

        if (isEmpty && (nodeIndex != activeIndex)) {
            *outSol1 = nodeIndex;
            return;
        }

        *outSol2 = maxSol2;
        *outVIndex3 = vSol;
        *outSol3 = maxSol3;
    } else {
        finalizeSolutionsWhenDepthZero(nodeIndex, currentPath, outSol2, outSol3, outVIndex3);
    }
}






