/*
 *  glfem.h
 *  Library for MECA1120 : Finite Elements for dummies
 *
 *  Copyright (C) 2013 UCL-IMMC : Vincent Legat
 *  All rights reserved.
 *
 *  Pour GLFW (version utilisée 2.7.7)
 *  Pour l'installation de la librairie, voir http://www.glfw.org/
 *
 */

#ifndef _GLFEM_H_
#define _GLFEM_H_

#include <GL/glfw.h>
#include "delaunay.h"

void glfemDrawColorElement(float *x, float *y, double *u, int n);
void glfemDrawElement(float *x, float *y, int n);
void glfemDrawNodes(double* x, double* y,int n);

void glfemReshapeWindows(const char *PlotName, int width, int heigh);
//void glfemPlotField(femMesh *theMesh, double *u); ON UTILISE PAS
void glfemPlotMesh(const char *PlotName);
void glfemMessage(char *message);

void glfemDrawMessage(int h, int v, char *message);
void glfemSetRasterSize(int width, int height);
void glfemInit(char *windowName);

#endif
