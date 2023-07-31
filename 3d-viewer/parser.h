#ifndef PARSER_H
#define PARSER_H
#define _USE_MATH_DEFINES
#define _GNU_SOURCE
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 0
#define ERROR 1

typedef struct our {
  int sizeArrayPoint;
  int lastIndexPoint;
  double *arrayPoint;
  int sizeArrayFacete;
  int lastIndexFacete;
  int *arrayFacete;
  int countFacete;
} viewer3D;

int openFile(char *path, FILE **flowFile);
int increaseArrayPoint(viewer3D *key);
int increaseArrayFacete(viewer3D *key);
int addPoint(double point, viewer3D *key);
int addFacete(int facete, viewer3D *key);
int parsePoint(char *string, viewer3D *key);
int parseFacete(char *string, viewer3D *key);
int parse(FILE *flowFile, viewer3D *key);
void changeSizeArray(viewer3D *key, int option);
void movingCoordinatePoint(viewer3D *key, double x, double y, double z);
void rotateViewer(viewer3D *key, double degree, char coordinate);
void scaleViewer(viewer3D *key, double scale);

#endif  // PARSER_H
