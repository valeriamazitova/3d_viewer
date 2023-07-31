#include "parser.h"

int openFile(char *path, FILE **flowFile) {
  *flowFile = fopen(path, "r");
  return (*flowFile) ? OK : ERROR;
}

int parse(FILE *flowFile, viewer3D *key) {
  int result = OK;
  size_t size = 1;
  char *string = (char *)calloc(size, sizeof(char));
  if (!string) {
    result = ERROR;
  } else {
    while (getline(&string, &size, flowFile) != -1 && !result) {
      if (strlen(string) > 2)
        result = parsePoint(string, key) || parseFacete(string, key);
    }
    free(string);
  }
  if (errno == ENOMEM) result = ERROR;
  return result;
}

int parsePoint(char *string, viewer3D *key) {
  int count = 0;
  int result = OK;
  if (*string == 'v' && *(string + 1) == ' ') {
    string += 2;
    while (*string && !result && count < 3) {
      if ((*string >= '0' && *string <= '9') || *string == '-') {
        result = addPoint(strtod(string, &string), key);
        count++;
      }
      if (*string) string++;
    }
  }
  return result;
}

int parseFacete(char *string, viewer3D *key) {
  int result = OK;
  if (*string == 'f' && *(string + 1) == ' ') {
    int first = -1;
    string += 2;
    key->countFacete++;
    while (*string && !result) {
      if (*string >= '0' && *string <= '9') {
        if (first == -1) {
          first = (int)strtol(string, &string, 10);
          result = addFacete(first, key);
        } else {
          int other = (int)strtol(string, &string, 10);
          result = addFacete(other, key) || addFacete(other, key);
        }
      }
      while (*string && *string != ' ' && !result) {
        string++;
      }
      if (*string == ' ') string++;
    }
    if (!result) {
      result = addFacete(first, key);
    }
  }
  return result;
}

int addPoint(double point, viewer3D *key) {
  int result = increaseArrayPoint(key);
  if (!result) {
    key->arrayPoint[key->lastIndexPoint] = point;
    key->lastIndexPoint++;
  }
  return result;
}

int addFacete(int facete, viewer3D *key) {
  int result = increaseArrayFacete(key);
  if (!result) {
    key->arrayFacete[key->lastIndexFacete] = facete - 1;
    key->lastIndexFacete++;
  }
  return result;
}

int increaseArrayPoint(viewer3D *key) {
  int result = OK;
  if (key->sizeArrayPoint == key->lastIndexPoint) {
    changeSizeArray(key, 0);
    double *newArrayPoint = NULL;
    newArrayPoint = (double *)realloc(key->arrayPoint,
                                      key->sizeArrayPoint * sizeof(double));
    if (newArrayPoint) {
      key->arrayPoint = newArrayPoint;
    } else {
      result = ERROR;
    }
  }
  return result;
}

int increaseArrayFacete(viewer3D *key) {
  int result = OK;
  if (key->sizeArrayFacete == key->lastIndexFacete) {
    changeSizeArray(key, 1);
    int *newArrayFacete = NULL;
    newArrayFacete =
        (int *)realloc(key->arrayFacete, key->sizeArrayFacete * sizeof(int));
    if (newArrayFacete) {
      key->arrayFacete = newArrayFacete;
    } else {
      result = ERROR;
    }
  }
  return result;
}

void changeSizeArray(viewer3D *key, int option) {
  if (!option)
    key->sizeArrayPoint *= 2;
  else
    key->sizeArrayFacete *= 2;
}

void movingCoordinatePoint(viewer3D *key, double x, double y, double z) {
  int index = 0;
  while (index < key->lastIndexPoint - 2) {
    key->arrayPoint[index] += x;
    key->arrayPoint[index + 1] += y;
    key->arrayPoint[index + 2] += z;
    index += 3;
  }
}

void rotateViewer(viewer3D *key, double degree, char coordinate) {
  double radian = degree * M_PI / 180;
  for (int index = 0; index < key->lastIndexPoint - 2; index += 3) {
    double x = key->arrayPoint[index];
    double y = key->arrayPoint[index + 1];
    double z = key->arrayPoint[index + 2];
    switch (coordinate) {
      case 'x':
        key->arrayPoint[index + 1] = y * cos(radian) - sin(radian) * z;
        key->arrayPoint[index + 2] = y * sin(radian) + cos(radian) * z;
        break;
      case 'y':
        key->arrayPoint[index] = x * cos(radian) + sin(radian) * z;
        key->arrayPoint[index + 2] = -x * sin(radian) + cos(radian) * z;
        break;
      case 'z':
        key->arrayPoint[index] = x * cos(radian) - sin(radian) * y;
        key->arrayPoint[index + 1] = x * sin(radian) + cos(radian) * y;
        break;
      default:;
    }
  }
}

void scaleViewer(viewer3D *key, double scale) {
  for (int index = 0; index < key->lastIndexPoint; index++) {
    key->arrayPoint[index] *= scale;
  }
}
