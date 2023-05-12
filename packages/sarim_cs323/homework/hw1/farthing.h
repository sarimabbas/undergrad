// farthing.h                                     Stan Eisenstat (08/20/18)
// System header files and macros for farthing

#define _GNU_SOURCE
#include "list.h" 
// #include <linux/limits.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>


// Write message to stderr using format FORMAT
#define WARN(format,...) fprintf (stderr, "farthing: " format "\n", __VA_ARGS__)

// Write message to stderr using format FORMAT and exit.
#define DIE(format,...)  WARN(format,__VA_ARGS__), exit (EXIT_FAILURE)

// Change allocated size of array X of unspecified type to S elements.
// X may be NULL; assumes that realloc() never fails.
#define REALLOC(x,s) x = realloc (x, (s) * sizeof(*(x)))

/* KEYS */

int tKey(char *archive_file_path, char **names, int names_count);

int rKey(char* archive_file_path, char** names, int names_count);

int dKey(char *archive_file_path, char **names, int names_count);

int xKey(char* archive_file_path, char** names, int names_count);

/* WORKING WITH DIRECTORIES */

/**
 * @brief : checks if 
 * 
 * @param archive_file_path 
 * @return true 
 * @return false 
 */
bool isStdin(char* archive_file_path);

/**
 * @brief : tests if a name corresponds to an openable file
 * 
 * @param local_name : the path to the file
 * @return true : if exists
 * @return false : if otherwise
 */
bool isValidName(char* local_name);

/**
 * @brief : write a list item to an archive file in the specified format:
 * 
 * @param li : the list item to write
 * @param name_file : the file corresponding to the list item to write
 * @param archive_file : the file stream to write to
 */
void writeArchiveItem(list_item* li, FILE* name_file, FILE *archive_file);

/**
 * @brief : writes some number of bytes to a file
 * 
 * @param li : the list item to read metadata from 
 * @param src_file : the source file to read bytes from 
 * @param dest_file : the destination file to write bytes to
 */
void writeBytes(list_item* li, FILE* src_file, FILE* dest_file);

/* STRING HELPERS */

/**
 * @brief : check if a str is exact match to a list of strs
 * 
 * @param str : the str to check
 * @param strList : the list to check against
 * @param strListLen : the length of the list to check against
 * @param mark: an array of booleans that mark whether a name is successfully operated on
 * @param mode: if "reverse", then it marks the items as false instead
 * @return true : if found
 * @return false : if not
 */
bool strIsContained(char *str, char **strList, int strListLen, bool* mark, char* mode);

/**
 * @brief : uses strIsChild() to see if a string is a child of some path 
 * 
 * @param test : the str to check
 * @param strList : the list to check against
 * @param strListLen : the length of the list to check against
 * @param mark: an array of booleans that mark whether a name is successfully operated on
 * @param mode : if "reverse", then it marks the items as false instead
 * @return true : if found
 * @return false : if not
 */
bool strIsChildList(char* test, char** strList, int strListLen, bool* mark, char* mode);

/**
 * @brief : helper function for strIsChildList()
 * 
 * @param test : the str to check
 * @param against : the str to check against
 * @return true : if child
 * @return false : if not
 */
bool strIsChild(char* test, char* against);

/**
 * @brief : advances cursor in the file to throw away bytes
 *
 * @param byte_size : the no of bytes to throw away
 * @param file : the file to advance the cursor in
 */
bool wasteBytes(int byte_size, FILE* file);

/**
 * @brief : given a file path, it returns a heap string: file_path.bak
 * 
 * @param archive_file_path : the path to use as a template
 * @return char* : the string with .bak postfix
 */
char* genTmpFileName(char* archive_file_path);

/**
 * @brief : check if a name is a directory or not
 * 
 * @param name : the str path to check
 * @return true : if a directory
 * @return false : if not
 */
bool isDir(char* name);

/**
 * @brief : recursively find names in a directory 
 * 
 * @param dir : the starting directory
 * @param l : the list to push newly found names to
 */
void getDirNames(char *dir, list *l);

/**
 * @brief : find the directory or path to a file 
 * 
 * @param name : the full path to a file
 * @return char* : the path to the directory containing the file
 */
char* getDirPath(char* name);

/* PARSE ARCHIVE ITEMS */

/**
 * @brief : parses one item from an archive file and stores information in a list item
 * 
 * @param archive_file : the archive file to parse
 * @param li : the list item to store metadata in
 * @return true : if success
 * @return false : if not
 */
bool parseArchiveMeta(FILE* archive_file, list_item* li);

/**
 * @brief : extracts a list item onto the file system
 * 
 * @param li : list item containing file metadata
 * @param archive : archive file containing bytes
 * @return true : if successful 
 * @return false : if not
 */
bool extractItem(list_item li, FILE* archive);

