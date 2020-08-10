#include "mp3_function.h"
#include <stdbool.h>

bool mp3__is_mp3_file(const char *file_name) {
  if (strlen(file_name) > 4) {
    if ((strstr(file_name, ".mp3") != NULL) && (strstr(file_name, "._") == NULL)) {
      printf("File name: %s\n", file_name);
      return 1;
    }
  } else {
    return 0;
  }
}