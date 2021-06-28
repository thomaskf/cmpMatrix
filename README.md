# Software: cmpMatrix

This program is to compute the root-mean-square difference between two symmetric distance matrices

## Installation

The software was written in C++, and it has been tested under linux and MacOS platform. You need
to have C++ compiler installed in the machine in order to compile the source codes. The compilation
steps are shown as follows:

```
tar -zxvf cmpMatrix-1.X.tar.gz
cd cmpMatrix-1.X
make
```

Then an executable file named *combineSomGerm* will appear

## Usage

To compute the root-mean-square difference between two symmetric distance matrices:

Syntax:
```
./cmpMatrix [dist matrix 1] [dist matrix 2]
```

To reorder and output the first distance matrix as the order in the second distrance matrix:

Syntax:
```
./cmpMatrix [dist matrix 1] [dist matrix 2] -reorder > [out file]
```

Note: all distance matrices have to be in phylip format

## Example:

To compute the root-mean-square difference between two distance matrices:
```
./cmpMatrix 1000_test_ori.dist 1000_test_converted.dist
```

To reorder and output the first distance matrix as the order in the second distrance matrix:
```
./cmpMatrix 1000_test_ori.dist 1000_test_converted.dist > 1000_updated_ori.dist
```
