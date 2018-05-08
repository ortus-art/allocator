# STL chunk memory allocator project

[![Build Status][travis-badge]][travis-link]
[![codecov][codecov-badge]][codecov-link]
[![License: GPL v3][license-badge]](LICENSE.md)
[![Download][bintray-badge]][bintray-link]

## Requirements

Create a memory allocator that allows to allocate memory in chunks for std::map so it will be similar to 'reserve' call.

The allocator has to accept memory size parameter (expressed in number of items). All items placement and deletion is managed by the allocator. Optionally the allocator memory can support dynamic memory growth.

Initially the allocator is used by std::map. In addition project has to define own simple container that is parameterized with allocator and reuses the new defined allocator. The container has to implement at least forward iteration and addition of new elements. Optionally it can be STL compatible.

Steps:

1. Fill the std::map with factorial from 0 to 9
2. Fill the std::map with factorial from 0 to 9 and use the custom allocator
3. Fill the custom container with factorial from 0 to 9
4. Fill the custom with factorial from 0 to 9 and use the custom allocator
5. Output all data to the stadard output.

## Verification

1. All data has to visible in the standard output
2. The command 'ltrace allocator' has to show reduced number of memory allocations

## Implementation notes

### Allocator Algorithm

The allocator has a very basic implementation that is not thread aware.

It supports dynamic allocation of memory in chunks on the heap. The memory size is passed as the template parameter and it is expressed as a number times sizeof the stored type. The linked list is used as an underlying memory management container.

The memory is allocated one chunk at a time when needed. The memory is added to the end of the list. When the managed object is deleted the memory used by it is marked as free. When node get completely empty it may be deleted depending on the memory management model selected.

There are there memory management models:

* FIFO - The memory is checked and freed from the head of the list
* LIFO - The memory is checked and freed from the end of the list
* NONE (Default) - The memory is checked from the end of the list. The free memory blocks are not freed.

### Allocator Memory Consumtion and Layout

The allocator itself uses std::list.

Each chunk has memory map bit set, where one bit is used for each element stored. Plus each item contains a pointer pointing to the chunk manager. So generally memory overhead is sizeof(void *) + 1 bit.

## Forward Only List

There is as well implemented a singly listed forward only list. It has a very basic implementation and is used as one of use cases for the allocator.

[travis-badge]:    https://travis-ci.org/ortus-art/allocator.svg?branch=master
[travis-link]:     https://travis-ci.org/ortus-art/allocator
[license-badge]:   https://img.shields.io/badge/License-GPL%20v3-blue.svg
[codecov-badge]:   https://codecov.io/gh/ortus-art/allocator/branch/master/graph/badge.svg
[codecov-link]:    https://codecov.io/gh/ortus-art/allocator
[bintray-badge]:   https://api.bintray.com/packages/ortus-art/course/allocator/images/download.svg
[bintray-link]:    https://bintray.com/ortus-art/course/allocator/_latestVersion