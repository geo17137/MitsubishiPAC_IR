#ifndef __FILE_H
#define __FILE_H
#include <string>

class FileLittleFS {
  char path[32];
  File file;
  public:
    FileLittleFS(const char *filePath);
    //~FileLittleFS() {free(path); }
    void    connectFs();
    void    listDir();
    boolean exist();
    void    purge(unsigned size);
    String  readFile();
    int     fileSize();
    void    writeFile(const char * message, const char* mode);
    void    writeFile(String message, const char *mode);
    void    deleteFile();
    void    close();
};
#endif