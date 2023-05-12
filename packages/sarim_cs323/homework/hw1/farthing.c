#define _GNU_SOURCE
#include "farthing.h"
#include <stdlib.h>

int main(int argc, char* argv[]) {
    char* operations[] = {"r", "x", "d", "t"};
    int operations_count = 4;

    // 1. key must exist
    // 2. key must be one of operations
    // 3. arg count must be > 3
    char* key = argv[1];
    if (key == NULL || !strIsContained(argv[1], operations, operations_count, NULL, NULL) ||
        argc < 3) {
        DIE("farthing r|x|d|t archive [name]*%s", "");
    }

    // make arguments easier to deal with
    char* archive_file_path = argv[2];
    char** names = NULL;
    int names_count = 0;
    if (argc > 3) {
        names = &argv[3];
        names_count = argc - 3;
    }
    
    // send to correct function
    switch (key[0]) {
        case 'r':
            if (argc < 4) {
                DIE("farthing r|x|d|t archive [name]*%s", "");
            }
            rKey(archive_file_path, names, names_count);
            break;
        case 'x':
            xKey(archive_file_path, names, names_count);
            break;
        case 'd':
            dKey(archive_file_path, names, names_count);
            break;
        case 't':
            tKey(archive_file_path, names, names_count);
            break;
        default:
            DIE("unexpected key %s", key);
            break;
    }
    // return
    return 0;
}

/* KEYS */

int rKey(char* archive_file_path, char** names, int names_count) {
    // list setup 
    list l;
    lSetup(&l);

    // add user names
    eliminateTrailingSlashesList(names, names_count);
    for (int i = 0; i < names_count; i++) {
        if (isValidName(names[i])) { 
            list_item li; liSetup(&li);
            liRead(&li, names[i]);
            lPush(&l, li);
            if(li.is_dir) {
                char dir[PATH_MAX] = {'\0'};
                strcat(dir, names[i]);
                getDirNames(dir, &l);
            }
        } else {
            WARN("cannot add %s", names[i]);
        }
    }
    // all list items guaranteed to end in slash if dir
    // all list items guaranteed to not end in slash if regular file

    // open tmp file
    FILE* tmp_file;
    char* tmp_file_path;
    if (isStdin(archive_file_path)) { tmp_file = stdout; } 
    else {
        tmp_file_path = genTmpFileName(archive_file_path);
        tmp_file = fopen(tmp_file_path, "w");
        if (tmp_file == NULL) {
            free(tmp_file_path);
            lDestroy(&l);
            DIE("cannot create temporary file%s", "");
        }
    }

    // read from existing archive, if it exists
    FILE* archive_file;
    if (isStdin(archive_file_path)) { archive_file = stdin; } 
    else {
        archive_file = fopen(archive_file_path, "r");
    }
    if (archive_file != NULL) {
        while (true) {
            list_item li;
            liSetup(&li);
            bool parse_result = parseArchiveMeta(archive_file, &li);
            if (!parse_result) { break; }
            // if item in user list, then waste it (we will write a fresh version)
            // otherwise preserve it
            if (lFind(&l, li)) { 
                wasteBytes(li.byte_size, archive_file); 
            } else { 
                writeArchiveItem(&li, archive_file, tmp_file); 
            }
        }
        fclose(archive_file);       // close the archive
        unlink(archive_file_path);  // delete the archive
    }

    // write the files input by the user to temporary file
    for (int i = 0; i < l.count; i++) {
        list_item* li = lGet(&l, i);
        FILE* name_file = fopen(li->local_name, "r");
        if (name_file != NULL) {
            writeArchiveItem(li, name_file, tmp_file);
            fclose(name_file);
        }
    }

    // cleanup
    lDestroy(&l);
    if (!isStdin(archive_file_path)) {
        rename(tmp_file_path, archive_file_path);   // rename the tmp file to archive       
        fclose(tmp_file);                           // close the tmp file
        free(tmp_file_path);                        // free the tmp file string
    }

    // return
    return 0;
}

int xKey(char *archive_file_path, char **names, int names_count) {
    // sanitize input and add user names to list
    if (names_count > 0 ) { eliminateTrailingSlashesList(names, names_count); }

    // create a bool array to check which names were able to cause an extraction
    bool mark[names_count];
    for (int i = 0; i < names_count; i++) {
        mark[i] = false;
    }

    // open archive
    FILE* archive_file;
    if (isStdin(archive_file_path)) { archive_file = stdin; } 
    else { archive_file = fopen(archive_file_path, "r");}
    if (archive_file == NULL) { DIE("cannot open archive %s", archive_file_path); }

    // parse
    while (true) {
        // parse one item
        list_item li;
        liSetup(&li);

        bool parse_result = parseArchiveMeta(archive_file, &li);
        if (!parse_result) { break; }

        // make a copy of local name
        char name_cmp[PATH_MAX] = {'\0'};
        strcat(name_cmp, li.local_name);
        eliminateTrailingSlashes(name_cmp);

        // extract user names only
        if(names_count > 0) {
            // extract exact matches and child matches
            if (strIsContained(name_cmp, names, names_count, mark, NULL) ||
                strIsChildList(name_cmp, names, names_count, mark, NULL)) {
                if (!extractItem(li, archive_file)) {
                    strIsContained(name_cmp, names, names_count, mark, "reverse");
                    strIsChildList(name_cmp, names, names_count, mark, "reverse");
                }
            }
            else {
                wasteBytes(li.byte_size, archive_file);
            }
        // extract all
        } else {
            extractItem(li, archive_file);
        }
    }

    // cleanup
    if (!isStdin(archive_file_path)) {
        fclose(archive_file);
    }
    
    // note which names did not cause an extraction
    for(int i = 0; i < names_count; i++) {
        if(mark[i] != true) {
            WARN("cannot find %s", names[i]);
        }
    }
    
    // return
    return 0;
}

int tKey(char* archive_file_path, char** names, int names_count) {
    // open archive
    FILE* archive_file;
    if (isStdin(archive_file_path)) { archive_file = stdin; } 
    else { archive_file = fopen(archive_file_path, "r");}
    if (archive_file == NULL) { DIE("cannot open archive %s", archive_file_path); }
    // create archive list
    list archive_list;
    lSetup(&archive_list);
    // parse
    while (true) {
        // parse one item
        list_item li;
        liSetup(&li);
        bool parse_result = parseArchiveMeta(archive_file, &li);
        if (!parse_result) { break; }
        bool waste_result = wasteBytes(li.byte_size, archive_file);
        if (!waste_result) { break; }
        // add to list
        lPush(&archive_list, li);
    }
    // create user list
    list user_list;
    lSetup(&user_list);
    if (names_count > 0) {
        // add items
        eliminateTrailingSlashesList(names, names_count);
        for (int i = 0; i < names_count; i++) {
            list_item li;
            liSetup(&li);
            strcat(li.local_name, names[i]);
            lPush(&user_list, li);
        }
        // mark matches of user list against archive list
        lMatch(&user_list, &archive_list);
    }
    // output the list contents
    if (names_count > 0) {
        lPrint(&archive_list, "mark");
    } else {
        lPrint(&archive_list, "all");
    }
    // note what wasn't matched
    for (int i = 0; i < lCount(&user_list); i++) {
        list_item* li = lGet(&user_list, i);
        if (!li->mark) {
            WARN("cannot find %s", li->local_name);
        }
    }
    // destroy the lists
    lDestroy(&archive_list);
    lDestroy(&user_list); 
    // cleanup
    if (!isStdin(archive_file_path)) { fclose(archive_file); } 
    // return
    return 0;
}

int dKey(char *archive_file_path, char **names, int names_count) {
    // whole file delete
    if (names_count == 0) { unlink(archive_file_path); return 0; }

    // sanitize input and add user names to list
    eliminateTrailingSlashesList(names, names_count);

    // create a bool array to check which names were able to cause a deletion
    bool mark[names_count]; 
    for(int i = 0; i < names_count; i++) { mark[i] = false; }

    // open the archive and tmp files
    FILE* archive_file;
    FILE* tmp_file;
    char* tmp_file_path;
    if (isStdin(archive_file_path)) { 
        archive_file = stdin; 
        tmp_file = stdout; 
    } else {
        archive_file = fopen(archive_file_path, "r");
        if (archive_file == NULL) { DIE("cannot open archive %s", archive_file_path); }
        tmp_file_path = genTmpFileName(archive_file_path);
        tmp_file = fopen(tmp_file_path, "w");
        if (tmp_file == NULL) {
            free(tmp_file_path);
            DIE("cannot create temporary file%s", "");
        }
    }

    // parse archive
    while(true) 
    {
        // create a list item
        list_item li;
        liSetup(&li);

        // parse one item
        int parse_result = parseArchiveMeta(archive_file, &li);
        if (!parse_result) { break; }

        // make a copy of local name
        char name_cmp[PATH_MAX] = {'\0'};
        strcat(name_cmp, li.local_name);
        eliminateTrailingSlashes(name_cmp);

        // delete exact matches
        if (strIsContained(name_cmp, names, names_count, mark, NULL)) {
            int waste_result = wasteBytes(li.byte_size, archive_file);
            if (!waste_result) { break; }
        }
        // delete if the item is a child of the user inputted names
        else if (strIsChildList(name_cmp, names, names_count, mark, NULL)) {
            int waste_result = wasteBytes(li.byte_size, archive_file);
            if (!waste_result) { break; }
        }
        // preserve
        else {
            writeArchiveItem(&li, archive_file, tmp_file);
        }
    }

    // cleanup
    if (!isStdin(archive_file_path)) {
        fclose(archive_file);
        fclose(tmp_file);
        unlink(archive_file_path);
        rename(tmp_file_path, archive_file_path);
        free(tmp_file_path);
    }
    
    // note which names did not cause a deletion
    for(int i = 0; i < names_count; i++) {
        if(mark[i] != true) {
            WARN("cannot delete %s", names[i]);
        }
    }
    // return
    return 0;
}


/* STRING UTILS */

bool strIsChildList(char* test, char** strList, int strListLen, bool* mark, char* mode) {
    bool found = false;
    for (int i = 0; i < strListLen; i++) {
        if (strIsChild(strList[i], test)) {
            found = true;
            if (mark != NULL) { 
                if (mode != NULL) {
                    if (strcmp(mode, "reverse") == 0) {
                        mark[i] = false;
                    } else {
                        mark[i] = true;
                    }
                } else {
                    mark[i] = true;
                }
            }
        }
    }
    return found;
}

bool strIsChild(char* test, char* against) {
    if (strlen(against) > strlen(test)) {
        if (strncmp(test, against, strlen(test)) == 0 &&
            against[strlen(test)] == '/') {
            return true;
        }
        else {
            return false;
        }
    } else {
        return false;
    }
}

bool strIsContained(char *str, char **strList, int strListLen, bool* mark, char* mode) {
    bool found = false;
    for (int i = 0; i < strListLen; i++) {
        if (strcmp(str, strList[i]) == 0) {
            found = true;
            if (mark != NULL) {
                if (mode != NULL) {
                    if (strcmp(mode, "reverse") == 0) {
                        mark[i] = false;
                    } else {
                        mark[i] = true;
                    }
                } else {
                    mark[i] = true;
                }
            }
        }
    }
    return found;
}


/* PARSING ARCHIVE FILES */

bool parseArchiveMeta(FILE* archive_file, list_item* li) {
    // get local name len
    int items_read = fscanf(archive_file, "%i", &(li->local_name_len));
    if (feof(archive_file) || ferror(archive_file) || items_read == 0 ||
        items_read == EOF) {
        return false;
    }

    // throw away pipe
    fgetc(archive_file);
    if (feof(archive_file) || ferror(archive_file)) {
        return false;
    }

    // get local name (fgets transfers only n - 1 chars)
    char* result = fgets(li->local_name, li->local_name_len + 1, archive_file);
    if (result == NULL || feof(archive_file) || ferror(archive_file)) {
        return false;
    }
    if (isTrailingSlash(li->local_name)) {
        li->is_dir = true;
    }

    // get byte size
    items_read = fscanf(archive_file, "%i", &(li->byte_size));
    if (feof(archive_file) || ferror(archive_file) || items_read == 0 ||
        items_read == EOF) {
        return false;
    }

    // throw away pipe
    fgetc(archive_file);
    if (feof(archive_file) || ferror(archive_file)) {
        return false;
    }

    // done
    return true;
}

bool wasteBytes(int byte_size, FILE* file) {
    for(int i = 0; i < byte_size; i++) {
        fgetc(file);
        if(feof(file) || ferror(file)) {
            return false;
        }
    }
    return true;
}

void writeBytes(list_item* li, FILE* src_file, FILE* dest_file) {
    // read/write bytes, no matter the position of the cursor
    if(src_file != NULL) {
        char byte;
        for(int i = 0; i < li->byte_size; i++) {
            byte = fgetc(src_file);
            fputc(byte, dest_file);
        }
    }
} 

/* WRITING AND EXTRACTING */

void writeArchiveItem(list_item* li, FILE* src_file, FILE* dest_file) {
    // <LOCAL_NAME_LEN><PIPE><LOCAL_NAME><NEWLINE><BYTE_SIZE><PIPE><CONTENT><LOCAL_NAME_LEN>...
    // local name len
    fprintf(dest_file, "%i", li->local_name_len);
    // pipe
    fputc('|', dest_file);
    // local name
    fprintf(dest_file, "%s", li->local_name);
    // new line
    fputc('\n', dest_file);
    // byte size
    fprintf(dest_file, "%i", li->byte_size);
    // pipe
    fputc('|', dest_file);
    // read/write bytes, no matter the position of the cursor
    writeBytes(li, src_file, dest_file);
} 

bool extractItem(list_item li, FILE* archive) {
    bool no_error = true;
    // if directory
    if (li.is_dir) {
        struct stat statbuf;
        // if the directory does not exist
        if (lstat(li.local_name, &statbuf) != 0) {
            // create the directory but don't write anything
            int result = mkdir(li.local_name, ACCESSPERMS);
            if (result != 0) { 
                WARN("cannot extract %s", li.local_name); 
                no_error = false;
            }
            wasteBytes(li.byte_size, archive);
        } else {
            // if the directory already exists just don't write anything
            wasteBytes(li.byte_size, archive);
        }
    }
    // if file
    else {
        char* create_dir = getDirPath(li.local_name);
        struct stat statbuf;
        // if the directory to the file does not exist
        if (lstat(create_dir, &statbuf) != 0) {
            // create the directory
            mkdir(create_dir, ACCESSPERMS);
            // if (result != 0) { WARN("cannot create %s", create_dir); }
        }
        free(create_dir);
        // place the file (now in a directory guaranteed to exist)
        FILE* dest_file = fopen(li.local_name, "w");
        // if the file pointer can be created (which should happen)
        if (dest_file != NULL) {
            // write the bytes
            writeBytes(&li, archive, dest_file);
            fclose(dest_file); 
        // if not, it is likely because it is write protected
        } else { 
            // warn and move the cursor forward anyway in the file
            WARN("cannot extract %s", li.local_name);
            no_error = false;
            wasteBytes(li.byte_size, archive);
        }
    }
    return no_error;
}

/* WORKING WITH DIRECTORIES */

void getDirNames(char* dir, list* l) {
    // sanitize
    eliminateTrailingSlashes(dir);
    // setup
    DIR* dp;
    struct dirent* entry;
    struct stat statbuf;
    // open directory
    if ((dp = opendir(dir)) == NULL) {
        DIE("cannot open directory: %s", dir);
        return;
    }
    // move through entries
    while ((entry = readdir(dp)) != NULL) {
        // ignore . and ..
        if (strcmp(".", entry->d_name) == 0 ||
            strcmp("..", entry->d_name) == 0) {
            continue;
        }
        // build up a name
        char builder[PATH_MAX] = {'\0'};
        strcat(builder, dir);
        strcat(builder, "/");
        strcat(builder, entry->d_name);
        eliminateTrailingSlashes(builder);
        // set up a list item
        list_item li;
        liSetup(&li);
        strcat(li.local_name, builder);
        li.local_name_len = strlen(li.local_name);
        // get other metadata
        int result = lstat(builder, &statbuf);
        if (result != 0) { DIE("cannot read child data %s", entry->d_name); }
        if (S_ISDIR(statbuf.st_mode)) {
            strcat(li.local_name, "/"); // add slash for dir
            li.local_name_len++;        // increment local name length
            li.byte_size = 0;           // set byte size to 0
            li.is_dir = true;           // set dir flag to true
            lPush(l, li);               // push onto list
            getDirNames(builder, l);    // recurse
        }
        else {
            li.byte_size = statbuf.st_size;
            lPush(l, li);               // push onto list
        }
    }
    closedir(dp);
}

bool isDir(char* name) {
    char cpy[PATH_MAX];
    strcat(cpy, name);
    eliminateTrailingSlashes(cpy);
    struct stat statbuf;
    if (lstat(cpy, &statbuf) != 0) {
        LIST_WARN("isDir for %s failed", cpy);
        return false;
    }
    if (S_ISDIR(statbuf.st_mode)) {
        return true;
    } else {
        return false;
    }
}

bool isStdin(char* archive_file_path) {
    if (strcmp(archive_file_path, "-") == 0) {
        return true;
    } else {
        return false;
    }
}

char* getDirPath(char* name) {
    char* dir = malloc(sizeof(char) * PATH_MAX);
    for(int i = 0; i < PATH_MAX; i++) { dir[i] = '\0'; }

    int count = 0;
    bool start = false;
    for (int i = strlen(name) - 1; i > -1; i--) {
        if (name[i] == '/') {
            start = true;
        }
        if(start) {
            count++;
        }
    }
    strncat(dir, name, count);
    return dir;
}

/* OTHER */

char* genTmpFileName(char* archive_name) {
    int len = strlen(archive_name) + 5;
    char* tmp_file_name = malloc(sizeof(char) * len);
    strcpy(tmp_file_name, archive_name);
    tmp_file_name[len - 5] = '.';
    tmp_file_name[len - 4] = 'b';
    tmp_file_name[len - 3] = 'a';
    tmp_file_name[len - 2] = 'k';
    tmp_file_name[len - 1] = '\0';
    return (tmp_file_name);
}

bool isValidName(char* local_name) {
    struct stat statbuf;
    // check file exists
    if (stat(local_name, &statbuf) == 0) {
        // accept other files and dirs
        if (S_ISREG(statbuf.st_mode) || S_ISDIR(statbuf.st_mode)) {
            return true;
        // ignore others
        } else {
            return false;
        }
    // if file does not exist
    } else {
        return false;
    }
}
