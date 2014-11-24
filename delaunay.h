
/*
 *  delaunay.h
 */

#ifndef _DELAUNAY_H_
#define _DELAUNAY_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define Error(a)   Error(a,__LINE__,__FILE__)
#define Warning(a) Warning(a,  __LINE__, __FILE__)

/* //////////////////////////// Les STRUCTURES ///////////////////////////////////// */

typedef struct {
double x; 
double y;
} Point; 

typedef struct {
int nodes[3];// les indices des 3 nodes du triangle
} triangle; 

typedef struct {
    int elem[2];// ce sont les indices du tableau de triangle elem de la structure Triangulation
    int node[2];
} Edge;

typedef struct {
    //int *elem; // on tente sans ca et avec un tableau de triangles a la place
    triangle* elem;
    Edge*     edges;
    double *X;// from data
    double *Y;// from data
    Point *points;// from data
    int nElem;// on met à jour quand on ajoute un triangule comme ça à la fin on sait dire combien de triangles on a construit
    int nNode;// from data
} Triangulation;

/*
typedef struct {
    femMesh *mesh;
    femEdge *edges;
    int nEdge;
    int nBoundary;
} femEdges;// inutile je crois 
*/

/* ////////////////////////////////// Les methodes //////////////////////////// */


void 		    triangulation(char *FileName, const char *ResultName);
void 		    test(int n);
Triangulation      *TriangulationCreate(char *FileName);
void                TriangulationWrite(const char *ResultName, Triangulation *theTriangulation);
void                TriangulationFree(Triangulation *theTriangulation);

void                findP0(Triangulation *theTriangulation);
void                AddPoint(Point *point, Triangulation *theTriangulation);
int 		    IsLegal(Edge *edge, Triangulation *theTriangulation);
void                LegalizeEdge(double X, double Y, Edge *edge, Triangulation *theTriangulation);
triangle           *PointLocate(Point *point,Triangulation *theTriangulation);
int                *ComputeRandom(int n);

/*

femMesh             *femMeshRead(const char *filename);
void                 femMeshWrite(const femMesh* myMesh, const char *filename);
void                 femMeshFree(femMesh *theMesh);

double               femMin(double *x, int n);
double               femMax(double *x, int n);
*/
//void                 Error(char *text, int line, char *file);
//void                 Warning(char *text, int line, char *file);

#endif
