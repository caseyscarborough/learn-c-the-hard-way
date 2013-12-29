#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
    int id;
    int set;
    char name[MAX_DATA];
    char email[MAX_DATA];
};

struct Database {
    struct Address rows[MAX_ROWS];
};

struct Connection {
    /**
     * FILE is a struct, just like we are creating here, but it is defined by
     * the C Standard Library.
     */
    FILE *file;
    struct Database *db;
};

void Database_close(struct Connection *conn);

void die(const char *message, struct Connection *conn)
{
    if(errno) {
        perror(message);
    } else {
        printf("ERROR: %s\n", message);
    }
    Database_close(conn);
    exit(1);
}

void Address_print(struct Address *addr)
{
    printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
    /**
     * fread is a function in stdio.h that reads data in from from a given 
     * stream into the array pointed to by ptr. The following is its 
     * declaration:
     *    size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
     * Parameters:
     *   ptr - This is the pointer to a block of memory with a minimum size of size*nmemb bytes.
     *   size - This is the size in bytes of each element to be read.
     *   nmemb - This is the number of elements, each one with a size of size bytes.
     *   stream - This is the pointer to a FILE object that specifies an input stream.
     * The total number of elements successfully read is returned as a size_t object, in
     * this case it should be 1.
     * See http://www.cplusplus.com/reference/cstdio/fread/
     */
    int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
    if(rc != 1) die("Failed to load database.", conn);
}

struct Connection *Database_open(const char *filename, char mode)
{
    struct Connection *conn = malloc(sizeof(struct Connection));
    if(!conn) die("Memory error", conn);

    conn->db = malloc(sizeof(struct Database));
    if(!conn->db) die("Memory error", conn);

    if(mode == 'c') {
        /**
         * fopen is a file function declared in stdio.h that opens and reads
         * a file in different modes, such as write (w), read (r), etc.
         * See http://www.cplusplus.com/reference/cstdio/fopen/
         */
        conn->file = fopen(filename, "w");
    } else {
        conn->file = fopen(filename, "r+");

        if(conn->file) {
            Database_load(conn);
        }
    }
    if(!conn->file) die("Failed to open the file", conn);

    return conn;
}

void Database_close(struct Connection *conn)
{
    if(conn) {
        /**
         * fclose closes a stream, and flushes all buffers.
         * Declaration:
         *   int fclose(FILE *stream)
         * Parameters:
         *   stream - This is the pointer to a FILE object that specifies
         *   the stream to be closed.
         * This method returns zero if the stream is successfully closed.
         * On failure, EOF is returned.
         * See http://www.cplusplus.com/reference/cstdio/fclose/
         */
        if(conn->file) fclose(conn->file);
        if(conn->db) free(conn->db);
        free(conn);
    }
}

void Database_write(struct Connection *conn)
{
    rewind(conn->file);

    /**
     * fwrite writes data from a given array into the given stream.
     * Declaration:
     *   size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
     * Parameters:
     *   ptr - This is the pointer to the array of elements to be written.
     *   size - This is the size in bytes of each element to be written.
     *   nmemb - This is the number of elements, each one with a size of size bytes.
     *   stream - This is the pointer to a FILE object that specifies an output stream.
     * Returns the total number of elements successfully written, in our case, 1.
     * See http://www.cplusplus.com/reference/cstdio/fwrite/
     */
    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    if(rc != 1) die("Failed to write database.", conn);

    /**
     * fflush flushes the output buffer of a stream, and returns 0 on success.
     * See http://www.cplusplus.com/reference/cstdio/fflush/
     */
    rc = fflush(conn->file);
    if(rc == -1) die("Cannot flush database.", conn);
}

void Database_create(struct Connection *conn)
{
    int i = 0;

    for(i = 0; i < MAX_ROWS; i++) {
        // make a prototype to initialize it
        struct Address addr = {.id = i, .set = 0};
        // then just assign it
        conn->db->rows[i] = addr;
    }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
    struct Address *addr = &conn->db->rows[id];
    if(addr->set) die("Already set, delete it first", conn);

    addr->set = 1;
    /**
     * strncpy copies the first num characters of source to destination.
     * WARNING: bug, read the "How To Break It" and fix this
     * No null-character is implicitly appended at the end of destination
     * if source is longer than num. Thus, in this case, destination shall
     * not be considered a null terminated C string (reading it as such
     * would overflow).
     * See http://www.cplusplus.com/reference/cstring/strncpy/
     */
    char *res = strncpy(addr->name, name, MAX_DATA);
    // Demonstrate the strncpy bug.
    if(!res) die("Name copy failed", conn);

    res = strncpy(addr->email, email, MAX_DATA);
    if(!res) die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id)
{
    struct Address *addr = &conn->db->rows[id];

    if(addr->set) {
        Address_print(addr);
    } else {
        die("ID is not set", conn);
    }
}

void Database_delete(struct Connection *conn, int id)
{
    struct Address addr = {.id = id, .set = 0};
    conn->db->rows[id] = addr;
}

void Database_list(struct Connection *conn)
{
    int i = 0;
    struct Database *db = conn->db;

    for(i = 0; i < MAX_ROWS; i++) {
        struct Address *cur = &db->rows[i];
        if(cur->set) {
            Address_print(cur);
        }
    }
}

int main(int argc, char *argv[])
{
    struct Connection *conn;
    if(argc < 3) die("USAGE: ex17 <dbfile> <action> [action params]", conn);

    char *filename = argv[1];
    char action = argv[2][0];
    conn = Database_open(filename, action);
    int id = 0;

    if(argc > 3) id = atoi(argv[3]);
    if(id >= MAX_ROWS) die("There aren't that many records.", conn);

    switch(action) {
        case 'c':
            Database_create(conn);
            Database_write(conn);
            break;

        case 'g':
            if(argc != 4) die("Need an id to get", conn);

            Database_get(conn, id);
            break;

        case 's':
            if(argc != 6) die("Need id, name, email to set", conn);

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if(argc != 4) die("Need an id to delete", conn);

            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;

        default:
            die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
    }

    Database_close(conn);

    return 0;
}