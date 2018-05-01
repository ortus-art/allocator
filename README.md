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

[travis-badge]:    https://travis-ci.org/ortus-art/allocator.svg?branch=master
[travis-link]:     https://travis-ci.org/ortus-art/allocator
[license-badge]:   https://img.shields.io/badge/License-GPL%20v3-blue.svg
[codecov-badge]:   https://codecov.io/gh/ortus-art/allocator/branch/master/graph/badge.svg
[codecov-link]:    https://codecov.io/gh/ortus-art/allocator
[bintray-badge]:   https://api.bintray.com/packages/ortus-art/course/allocator/images/download.svg
[bintray-link]:    https://bintray.com/ortus-art/course/allocator/_latestVersion