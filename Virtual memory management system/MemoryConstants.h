#pragma once

#include <climits>
#include <stdint.h>

/**
 * @file MemoryConstants.h
 * @brief Defines memory-related constants and macros for the virtual memory system.
 *
 * This file provides essential constants for the virtual memory framework, including
 * page and frame sizes, virtual and physical address widths, memory limits, and
 * parameters for the page replacement algorithm.
 */

// Type definition for a single memory word
typedef int word_t;

/**
 * Number of bits used for the offset within a page/frame.
 * The offset determines the position of a word within a page.
 */
#define OFFSET_WIDTH 4

/**
 * The size of a page/frame in words.
 * Since pages and frames are of equal size, this also represents
 * the number of entries in a page table.
 */
#define PAGE_SIZE (1LL << OFFSET_WIDTH)

/**
 * Number of bits in a physical memory address.
 * Determines the total number of addressable words in RAM.
 */
#define PHYSICAL_ADDRESS_WIDTH 10

/**
 * Total size of RAM in words.
 * The number of addressable locations in physical memory.
 */
#define RAM_SIZE (1LL << PHYSICAL_ADDRESS_WIDTH)

/**
 * Number of bits in a virtual memory address.
 * Determines the total address space for virtual memory.
 */
#define VIRTUAL_ADDRESS_WIDTH 20

/**
 * Total size of virtual memory in words.
 * Defines the maximum addressable space in the virtual memory system.
 */
#define VIRTUAL_MEMORY_SIZE (1LL << VIRTUAL_ADDRESS_WIDTH)

/**
 * Number of frames available in the RAM.
 * The RAM is divided into fixed-size frames.
 */
#define NUM_FRAMES (RAM_SIZE / PAGE_SIZE)

/**
 * Number of pages in virtual memory.
 * The virtual memory is divided into pages, each of which maps to a frame in RAM.
 */
#define NUM_PAGES (VIRTUAL_MEMORY_SIZE / PAGE_SIZE)

/**
 * Macro to compute the ceiling value of a division.
 * Used to determine the required depth of the hierarchical page table.
 */
#define CEIL(VARIABLE) ( (VARIABLE - (int)VARIABLE) == 0 ? (int)VARIABLE : (int)VARIABLE + 1 )

/**
 * Depth of the hierarchical page table tree.
 * This is computed based on the number of bits required to translate a virtual address.
 * It represents the number of levels required in the page table structure.
 */
#define TABLES_DEPTH CEIL((((VIRTUAL_ADDRESS_WIDTH - OFFSET_WIDTH) / (double)OFFSET_WIDTH)))

/**
 * Weight used in the page eviction algorithm for even pages.
 * Higher weight increases the priority of selecting even-numbered pages for eviction.
 */
#define WEIGHT_EVEN 4

/**
 * Weight used in the page eviction algorithm for odd pages.
 * Lower weight means odd-numbered pages are less likely to be evicted.
 */
#define WEIGHT_ODD 2

