/*
   Écriture/lecture d'un fichier (LITTLEFS) sur ESP8266
*/
#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>
#ifndef  __FILE_H
#include "files.h"


FileLittleFS::FileLittleFS(const char *fileName) {
  // path = malloc(strlen(fileName)+1);
  strcpy(path, fileName);
  connectFs();
}

void FileLittleFS::connectFs() {
  if (!LittleFS.begin()) {
    Serial.println("Echec du montage LittleFS");
    return;
  }
}
// lecture d'un fichier
String FileLittleFS::readFile() {
  // Serial.printf("Lecture du fichier: %s  ", path);
  file = LittleFS.open(FileLittleFS::path, "r");
  if (!file || file.isDirectory()) {
    Serial.println("Echec de la lecture");
    return String("");
  }
  return file.readString();
}

void FileLittleFS::writeFile(const char * message, const char* mode) {
  // Serial.printf("Ajout de donnees dans le fichier: %s ", path);
  // if (!LittleFS.begin()) {
  //   Serial.println("Echec du montage LittleFS");
  //   return;
  // }
  file = LittleFS.open(path, mode);
  if (!file) {
    Serial.println("Echec de l'ouverture du fichier!");
    return;
  }
  if (!file.print(message)) {
      Serial.println("Erreur ecriture");
  }
  file.close();
}

void FileLittleFS::writeFile(String message, const char *mode) {
  // Serial.printf("Ajout de donnees dans le fichier: %s ", path);
  // if (!LittleFS.begin()) {
  //   Serial.println("Echec du montage LittleFS");
  //   return;
  // }
  file = LittleFS.open(path, mode);
  if (!file) {
    Serial.println("Echec de l'ouverture du fichier!");
    return;
  }
  if (!file.print(message)) {
    Serial.println("Erreur ecriture");
  }
  file.close();
}


// Liste des fichiers présents
void FileLittleFS::listDir() {
  Serial.println("Liste des fichiers:");
  Dir dir = LittleFS.openDir ("");
  while (dir.next ()) {
    Serial.print("  Nom: ");
    Serial.print (dir.fileName ());
    Serial.print("\tTaille: ");
    Serial.println (dir.fileSize ());
  }
  Serial.println();
}

int FileLittleFS::fileSize() {
  Dir dir = LittleFS.openDir ("");
  while (dir.next ()) {
    // Serial.print (dir.fileName ());
    if (strcmp(dir.fileName().c_str(), path)==0) {
       return static_cast<int>(dir.fileSize());
    }
  }
  return -1;
}

void FileLittleFS::purge(unsigned size) {
  Dir dir = LittleFS.openDir ("");
  while (dir.next ()) {
    // Serial.print (dir.fileName ());
    if (strcmp(dir.fileName().c_str(), path)==0) {
       unsigned file_size = static_cast<unsigned>(dir.fileSize());
      if (file_size > size) {
        deleteFile();
        // Serial.printf("%s %s\n", "purge", dir.fileName().c_str());
      }
    }
  }
}

boolean FileLittleFS::exist() {
  Dir dir = LittleFS.openDir ("");
  while (dir.next ()) {
    // Serial.print (dir.fileName ());
  if (strcmp(dir.fileName().c_str(), path)==0)
    return true;
   }
  return false;
}

void FileLittleFS::deleteFile() {
  // Serial.printf("Suppression du fichier: %s ", path);
  if (!LittleFS.remove(path)) {
    Serial.println(" : echec de la suppression!");
  }
}

void FileLittleFS::close() {
   file.close();
}

#endif