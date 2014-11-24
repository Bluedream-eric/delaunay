
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
Point *sommet0;
Point *sommet1;
Point *sommet2;
int indice; 
} Triangle; 

typedef struct {
Triangle *elem0;
Triangle *elem1;
    //int elem[2];// ce sont les indices du tableau de triangle elem de la structure Triangulation
    //int node[2];
Point *P0;
Point *P1;
int indice; 
} Edge;

typedef struct {
  Triangle* theTriangle;
  myLeaf* theChildren;
} myLeaf;

typedef struct {
  myLeaf theRoot;
} myTree;

typedef struct {
  //int *elem; // on tente sans ca et avec un tableau de triangles a la place
  Triangle* elem;
  Edge*     edges;
  Point *points;// from data
  int nElem;// on met à jour quand on ajoute un triangule comme ça à la fin on sait dire combien de triangles on a construit
  int nNode;// from data
  myTree* theTree;
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
void                LegalizeEdge(Point *point, Edge *edge, Triangulation *theTriangulation);
void                PointLocate(Edge *edge,Triangle *trig,Point *point,Triangulation *theTriangulation, myLeaf *leaves);
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
