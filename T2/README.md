# File Management System

In this project we were tasked with developing an application capable of emulating a simplified FAT-like file management system. More specifically, this includes understanding the data structure for organizing such a system, implementing I/O routines for accessing it, and implementing a set of higher-level utilities for manipulating files and directories.

A FAT-like file management system can be seen as a contiguous set of blocks in which three main regions stand out: the superblock (corresponding to block 0), the FAT (corresponding to the following blocks, starting at 1) and the data region itself (corresponding to the remaining blocks).

The superblock serves to store general information about the filesystem:

```C
  #define CHECK_NUMBER 9999

  typedef struct superblock_entry {
    int check_number; // number to identify the system as valid
    int block_size; // block size {128, 256 (default), 512 or 1024 bytes}
    int fat_type; // FAT type {7, 8 (default), 9 or 10}
    int root_block; // number of the 1st block corresponding to the root directory
    int free_block; // number of the 1st block from the list of unused blocks
    int n_free_blocks; // total number of unused blocks
  } superblock;
)
```

The FAT (file allocation table) is a contiguous set of entries, numbered 0 through 2fat_type - 1, that allows you to reference the free data blocks and the data blocks used by each file/directory. Using the value of free_block you can access the first unused data block. To scroll through the complete list of unused blocks you must follow the reference stored in the FAT entry of equal order. A value of -1 in a FAT entry marks the end of the list.

Data blocks are used to store file and directory data. The number of data blocks equals the number of entries in the FAT, and each data block corresponds to the FAT entry of equal order. Like the unused blocks, the data blocks used by a file/directory are kept via a list in the FAT. A file is nothing more than a set of data blocks. The FAT specifies which data blocks are used by the file, and the order of the blocks in the file is the order represented in the FAT. The implementation of directories is somewhat identical to that of files (the first block of the root directory "/" is defined by the root_block field of the superblock), the difference lies in the fact that data blocks for directories follow a predefined format. A data block for a directory can be seen as an array of data structures of the following type, where each of these structures corresponds to a file/subdirectory of the directory in question.

```C
  #define TYPE_DIR  'D'
  #define TYPE_FILE 'F'
  #define MAX_NAME_LENGHT 20

  typedef struct directory_entry {
    char type;                   // tipo da entrada (TYPE_DIR ou TYPE_FILE)
    char name[MAX_NAME_LENGHT];  // nome da entrada
    unsigned char day;           // dia em que foi criada (entre 1 e 31)
    unsigned char month;         // mes em que foi criada (entre 1 e 12)
    unsigned char year;          // ano em que foi criada (entre 0 e 255 - 0 representa o ano de 1900)
    int size;                    // tamanho em bytes (0 se TYPE_DIR)
    int first_block;             // primeiro bloco de dados
  } dir_entry;
  ```
  
After starting the file management system emulator, the user should be able to communicate with the emulator via a command line where a number of utilities for manipulating files and directories can be invoked.

