/*
 *  delaunay.c
 *  
 */

#include "delaunay.h"
#define Error(a)   Error(a,__LINE__,__FILE__)
#define Warning(a) Warning(a,  __LINE__, __FILE__)

int triangulation(char *FileName, const char *ResultName,const char *PlotName)
{
Triangulation *theTriangulation = TriangulationCreate(FileName);

// TODO: ComputeRandom: un vecteur avec les nombres de 1 à n+1 mélangés pour voir dans quel ordre on ajoute les pts
// ce vecteur s'appelle de random. 
int *random = ComputeRandom(theTriangulation->nNode-1);
TriangulationWriteIter(PlotName,theTriangulation, 0);
int i=0;
for(i=0; i<theTriangulation->nNode-2; i++)
{
    printf(" ajout du point number %d \n",i+1);
    AddPoint(&(theTriangulation->points[random[i]]), theTriangulation,i);
    TriangulationWriteIter(PlotName,theTriangulation, i+1);
    //if(i==3){break;}
}

/*printf("edge 12: indice: %d \n",theTriangulation->edges[12].indice);
printf("edges 12: elem0->indice: %d \n",theTriangulation->edges[12].elem0->indice);
printf("edges 12: elem1->indice: %d \n",theTriangulation->edges[12].elem1->indice);*/


/*printf("triangle 7: edge0->indice: %d \n",theTriangulation->elem[7].edge0->indice);
printf("triangle 7: edge1->indice: %d \n",theTriangulation->elem[7].edge1->indice);
printf("triangle 7: edge2->indice: %d \n",theTriangulation->elem[7].edge2->indice);*/


// TODO: RemoveExtraPoints (enlever les points -1 et -2 ainsi que les edges qui les touchent)
TriangulationWritePlot(PlotName,theTriangulation);
TriangulationWriteAll(ResultName, theTriangulation);

TriangulationFree(theTriangulation); // ECRIRE COMME IL FAUT
return 5;// TODO ajuster cette valeur en fonction de nNode quand on ajoutera tous les points
}
/////////////////////////////////////////////////////////////////////////////////
void AddPoint(Point *point, Triangulation *theTriangulation,int i)
{
	theTriangulation->trigGlobal =NULL;
	theTriangulation->edgeGlobal=NULL;

	int nEdge=theTriangulation->nEdge;
	int nElem=theTriangulation->nElem;

	PointLocate(point,theTriangulation,theTriangulation->theTree->theRoot); 
	
	if(theTriangulation->edgeGlobal == NULL)
	{	 	
	printf(" le point %d en (%f,%f) est dans le triangle de sommets (%f,%f),(%f,%f),(%f,%f) \n",i,point->x,point->y,
			theTriangulation->trigGlobal->sommet0->x,theTriangulation->trigGlobal->sommet0->y,
			theTriangulation->trigGlobal->sommet1->x,theTriangulation->trigGlobal->sommet1->y,
			theTriangulation->trigGlobal->sommet2->y,theTriangulation->trigGlobal->sommet2->y);
	 	 
	/* AJOUT DES 3 TRIANGLES */
	
	theTriangulation->elem[nElem].sommet0=point;
	theTriangulation->elem[nElem].sommet1=theTriangulation->trigGlobal->sommet0;
	theTriangulation->elem[nElem].sommet2=theTriangulation->trigGlobal->sommet1;
	theTriangulation->elem[nElem].indice=nElem;
	theTriangulation->elem[nElem+1].sommet0=point;
	theTriangulation->elem[nElem+1].sommet1=theTriangulation->trigGlobal->sommet1;
	theTriangulation->elem[nElem+1].sommet2=theTriangulation->trigGlobal->sommet2;
	theTriangulation->elem[nElem+1].indice=nElem+1;
	theTriangulation->elem[nElem+2].sommet0=point;
	theTriangulation->elem[nElem+2].sommet1=theTriangulation->trigGlobal->sommet2;
	theTriangulation->elem[nElem+2].sommet2=theTriangulation->trigGlobal->sommet0;
	theTriangulation->elem[nElem+2].indice=nElem+2;
	
	theTriangulation->nElem=nElem+3;// check
	theTriangulation->nElemReal=theTriangulation->nElemReal+2;
	
	/* FEUILLES */
	theTriangulation->trigGlobal->theLeaf->nChildren=3;
	theTriangulation->trigGlobal->theLeaf->theChildren=malloc(3*sizeof(myLeaf));
	theTriangulation->trigGlobal->theLeaf->theChildren[0].theTriangle=&theTriangulation->elem[nElem];
	theTriangulation->trigGlobal->theLeaf->theChildren[0].theChildren=NULL;
	theTriangulation->trigGlobal->theLeaf->theChildren[0].nChildren=0;
	theTriangulation->trigGlobal->theLeaf->theChildren[1].theTriangle=&theTriangulation->elem[nElem+1];
	theTriangulation->trigGlobal->theLeaf->theChildren[1].theChildren=NULL;
	theTriangulation->trigGlobal->theLeaf->theChildren[1].nChildren=0;
	theTriangulation->trigGlobal->theLeaf->theChildren[2].theTriangle=&theTriangulation->elem[nElem+2];
	theTriangulation->trigGlobal->theLeaf->theChildren[2].theChildren=NULL;
	theTriangulation->trigGlobal->theLeaf->theChildren[2].nChildren=0;
	theTriangulation->elem[nElem].theLeaf=&theTriangulation->trigGlobal->theLeaf->theChildren[0];
	theTriangulation->elem[nElem+1].theLeaf=&theTriangulation->trigGlobal->theLeaf->theChildren[1];
	theTriangulation->elem[nElem+2].theLeaf=&theTriangulation->trigGlobal->theLeaf->theChildren[2];
	
	/* AJOUT DES EDGES */
	theTriangulation->edges[nEdge].P0=point;
	theTriangulation->edges[nEdge].P1=theTriangulation->trigGlobal->sommet0;
	theTriangulation->edges[nEdge].indice=nEdge;	
	theTriangulation->edges[nEdge].elem0=&theTriangulation->elem[nElem];
	theTriangulation->edges[nEdge].elem1=&theTriangulation->elem[nElem+2];
	theTriangulation->edges[nEdge+1].P0=point;
	theTriangulation->edges[nEdge+1].P1=theTriangulation->trigGlobal->sommet1;
	theTriangulation->edges[nEdge+1].indice=nEdge+1;
	theTriangulation->edges[nEdge+1].elem0=&theTriangulation->elem[nElem+1];
	theTriangulation->edges[nEdge+1].elem1=&theTriangulation->elem[nElem];
	theTriangulation->edges[nEdge+2].P0=point;
	theTriangulation->edges[nEdge+2].P1=theTriangulation->trigGlobal->sommet2;
	theTriangulation->edges[nEdge+2].indice=nEdge+2;
	theTriangulation->edges[nEdge+2].elem0=&theTriangulation->elem[nElem+2];
	theTriangulation->edges[nEdge+2].elem1=&theTriangulation->elem[nElem+1];
	
	theTriangulation->nEdge=nEdge+3;
	
	
	/* UPDATE des edges du contour de trigGlobal */
	if(theTriangulation->trigGlobal->edge0->P0->indice==theTriangulation->trigGlobal->sommet0->indice)
	{
		theTriangulation->trigGlobal->edge0->elem0=&theTriangulation->elem[nElem];
	}
	else
	{
		theTriangulation->trigGlobal->edge0->elem1=&theTriangulation->elem[nElem];
	}
	if(theTriangulation->trigGlobal->edge1->P0->indice==theTriangulation->trigGlobal->sommet1->indice)
	{
		theTriangulation->trigGlobal->edge1->elem0=&theTriangulation->elem[nElem+1];
	}
	else
	{
		theTriangulation->trigGlobal->edge1->elem1=&theTriangulation->elem[nElem+1];
	}
	if(theTriangulation->trigGlobal->edge2->P0->indice==theTriangulation->trigGlobal->sommet2->indice)
	{
		theTriangulation->trigGlobal->edge2->elem0=&theTriangulation->elem[nElem+2];
	}
	else
	{
		theTriangulation->trigGlobal->edge2->elem1=&theTriangulation->elem[nElem+2];
	}
	
	//// Pointeurs triangles->edges ///
	theTriangulation->elem[nElem].edge0= &theTriangulation->edges[nEdge]; 
	theTriangulation->elem[nElem].edge1= theTriangulation->trigGlobal->edge0;
	theTriangulation->elem[nElem].edge2= &theTriangulation->edges[nEdge+1];
	theTriangulation->elem[nElem+1].edge0= &theTriangulation->edges[nEdge+1];
	theTriangulation->elem[nElem+1].edge1= theTriangulation->trigGlobal->edge1;
	theTriangulation->elem[nElem+1].edge2= &theTriangulation->edges[nEdge+2];
	theTriangulation->elem[nElem+2].edge0= &theTriangulation->edges[nEdge+2]; 
	theTriangulation->elem[nElem+2].edge1= theTriangulation->trigGlobal->edge2;
	theTriangulation->elem[nElem+2].edge2= &theTriangulation->edges[nEdge];
	
	LegalizeEdge(point, theTriangulation->trigGlobal->edge0, theTriangulation);
	LegalizeEdge(point, theTriangulation->trigGlobal->edge1, theTriangulation);
	LegalizeEdge(point, theTriangulation->trigGlobal->edge2, theTriangulation); 
	/////////////////////////////////////////////////////////////////////
	}
	else if(theTriangulation->edgeGlobal != NULL)
	{	
	printf(" le point %d en (%f,%f) est dans l'edge de nodes (%f,%f),(%f,%f) \n",i,point->x,point->y,
			theTriangulation->edgeGlobal->P0->x,theTriangulation->edgeGlobal->P0->y,
			theTriangulation->edgeGlobal->P1->x,theTriangulation->edgeGlobal->P1->y);
	//////////////////////////////// 1) triangle1 ///////////////////
	Triangle *triangle1=theTriangulation->edgeGlobal->elem1;
	
	triangle1->theLeaf->nChildren=2;
	triangle1->theLeaf->theChildren=malloc(2*sizeof(myLeaf));
	triangle1->theLeaf->theChildren[0].theTriangle=&theTriangulation->elem[nElem];
	triangle1->theLeaf->theChildren[0].theChildren=NULL;
	triangle1->theLeaf->theChildren[0].nChildren=0;
	triangle1->theLeaf->theChildren[1].theTriangle=&theTriangulation->elem[nElem+2];
	triangle1->theLeaf->theChildren[1].theChildren=NULL;
	triangle1->theLeaf->theChildren[1].nChildren=0;
	theTriangulation->elem[nElem].theLeaf=&triangle1->theLeaf->theChildren[0];
	theTriangulation->elem[nElem+2].theLeaf=&triangle1->theLeaf->theChildren[1];
	
	
	Edge *edgeA = triangle1->edge0;
	Edge *edgeB = triangle1->edge1;
	if (edgeA->indice == theTriangulation->edgeGlobal->indice)
	{
		edgeA = triangle1->edge2;// A et B sont ok 
	}
	else if (edgeB->indice == theTriangulation->edgeGlobal->indice)
	{
		edgeB = triangle1->edge2;// A et B sont ok 
	}
	if (edgeA->P0->indice != theTriangulation->edgeGlobal->P1->indice && edgeA->P1->indice != theTriangulation->edgeGlobal->P1->indice)
	{// switch si edgeA ne touche pas le P1 de l'edge centrale
		Edge *trash=edgeA;
		edgeA=edgeB;
		edgeB=trash;
	}
		/* Find point_exterieur1 */
	Point *point_exterieur1=NULL;
	if(  triangle1->sommet0->indice != theTriangulation->edgeGlobal->P0->indice 
			&& triangle1->sommet0->indice != theTriangulation->edgeGlobal->P1->indice )
	{
		point_exterieur1=triangle1->sommet0; 
	}
	else if(  triangle1->sommet1->indice != theTriangulation->edgeGlobal->P0->indice 
			&& triangle1->sommet1->indice != theTriangulation->edgeGlobal->P1->indice )
	{
		point_exterieur1=triangle1->sommet1; 
	}
	else
	{
		point_exterieur1=triangle1->sommet2;
	}	
	
	/* AJOUT DE 2 TRIG */
	theTriangulation->elem[nElem].indice=nElem;
	theTriangulation->elem[nElem].sommet0=point;
	theTriangulation->elem[nElem].sommet1=theTriangulation->edgeGlobal->P0;	
	theTriangulation->elem[nElem].sommet2=point_exterieur1;	
	theTriangulation->elem[nElem].edge0= &theTriangulation->edges[nEdge+2];
	theTriangulation->elem[nElem].edge1= edgeB;
	theTriangulation->elem[nElem].edge2= &theTriangulation->edges[nEdge];  
	theTriangulation->elem[nElem+2].indice=nElem+2;
	theTriangulation->elem[nElem+2].sommet0=point;
	theTriangulation->elem[nElem+2].sommet2=theTriangulation->edgeGlobal->P1;
	theTriangulation->elem[nElem+2].sommet1=point_exterieur1;  	
	theTriangulation->elem[nElem+2].edge2= &theTriangulation->edges[theTriangulation->edgeGlobal->indice];
	theTriangulation->elem[nElem+2].edge1= edgeA;
	theTriangulation->elem[nElem+2].edge0= &theTriangulation->edges[nEdge]; 
	
	/* AJOUT DE 1 EDGE */
	theTriangulation->edges[nEdge].indice=nEdge;
	theTriangulation->edges[nEdge].P0=point;
	theTriangulation->edges[nEdge].P1=point_exterieur1;
	theTriangulation->edges[nEdge].elem0=&theTriangulation->elem[nElem+2];
	theTriangulation->edges[nEdge].elem1=&theTriangulation->elem[nElem];
	
	
	/* Update les elems de adgeA et edgeB */
	if(edgeA->P0->indice==theTriangulation->edgeGlobal->P1->indice)
	{
		edgeA->elem1=&theTriangulation->elem[nElem+2];
	}	
	else
	{
		edgeA->elem0=&theTriangulation->elem[nElem+2];
	}
	if(edgeB->P0->indice==theTriangulation->edgeGlobal->P0->indice)
	{
		edgeB->elem0=&theTriangulation->elem[nElem];
	}	
	else
	{
		edgeB->elem1=&theTriangulation->elem[nElem];
	}
	
	//////////////////////////////// 2) triangle0 ///////////////////
	Triangle *triangle0=theTriangulation->edgeGlobal->elem0;
	
	
	triangle0->theLeaf->nChildren=2;
	triangle0->theLeaf->theChildren=malloc(2*sizeof(myLeaf));
	triangle0->theLeaf->theChildren[0].theTriangle=&theTriangulation->elem[nElem+1];
	triangle0->theLeaf->theChildren[0].theChildren=NULL;
	triangle0->theLeaf->theChildren[0].nChildren=0;
	triangle0->theLeaf->theChildren[1].theTriangle=&theTriangulation->elem[nElem+3];
	triangle0->theLeaf->theChildren[1].theChildren=NULL;
	triangle0->theLeaf->theChildren[1].nChildren=0;
	theTriangulation->elem[nElem+1].theLeaf=&triangle0->theLeaf->theChildren[0];
	theTriangulation->elem[nElem+3].theLeaf=&triangle0->theLeaf->theChildren[1];
		
	Edge *edgeC=triangle0->edge0;
	Edge *edgeD=triangle0->edge1;
	if (edgeC->indice == theTriangulation->edgeGlobal->indice)
	{
		edgeC=triangle0->edge2;
	}
	else if (edgeD->indice == theTriangulation->edgeGlobal->indice)
	{
		edgeD=triangle0->edge2;
	}
	if (edgeC->indice != theTriangulation->edgeGlobal->P1->indice && edgeC->indice != theTriangulation->edgeGlobal->P1->indice)		
	{
	Edge *trashh = edgeC;
	edgeC = edgeD;
	edgeD = trashh;
	}

	/* Find point_exterieur0 */
	Point *point_exterieur0=NULL;
	if(  triangle0->sommet0->indice != theTriangulation->edgeGlobal->P0->indice
			 && triangle0->sommet0->indice != theTriangulation->edgeGlobal->P1->indice )
	{
		point_exterieur0=triangle0->sommet0;
	}
	else if(  triangle0->sommet1->indice != theTriangulation->edgeGlobal->P0->indice 
			&& triangle0->sommet1->indice != theTriangulation->edgeGlobal->P1->indice )
	{
		point_exterieur0=triangle0->sommet1; 
	}
	else
	{
		point_exterieur0=triangle0->sommet2;
	}
	
	/* AJOUT DE 2 TRIG */
	theTriangulation->elem[nElem+1].indice=nElem+1;
	theTriangulation->elem[nElem+1].sommet0=point;
	theTriangulation->elem[nElem+1].sommet2=theTriangulation->edgeGlobal->P0;
	theTriangulation->elem[nElem+1].sommet1=point_exterieur0;
	theTriangulation->elem[nElem+1].edge2= &theTriangulation->edges[nEdge+2];
	theTriangulation->elem[nElem+1].edge1= edgeD;
	theTriangulation->elem[nElem+1].edge0= &theTriangulation->edges[nEdge+1]; 
	theTriangulation->elem[nElem+3].indice=nElem+3;
	theTriangulation->elem[nElem+3].sommet0=point;
	theTriangulation->elem[nElem+3].sommet1=theTriangulation->edgeGlobal->P1;
	theTriangulation->elem[nElem+3].sommet2=point_exterieur0;
	theTriangulation->elem[nElem+3].edge0= &theTriangulation->edges[theTriangulation->edgeGlobal->indice];
	theTriangulation->elem[nElem+3].edge1= edgeC;
	theTriangulation->elem[nElem+3].edge2= &theTriangulation->edges[nEdge+1]; 
		
	/* AJOUT DE 1 EDGE */
	theTriangulation->edges[nEdge+1].indice=nEdge+1;
	theTriangulation->edges[nEdge+1].P0=point;
	theTriangulation->edges[nEdge+1].P1=point_exterieur0;
	theTriangulation->edges[nEdge+1].elem0=&theTriangulation->elem[nElem+1];
	theTriangulation->edges[nEdge+1].elem1=&theTriangulation->elem[nElem+3];
				
				/* Update les elems de edgeC et edgeD */
	if(edgeC->P0->indice==theTriangulation->edgeGlobal->P1->indice)
	{
		edgeC->elem0=&theTriangulation->elem[nElem+3];
	}	
	else
	{
		edgeC->elem1=&theTriangulation->elem[nElem+3];
	}
	if(edgeD->P0->indice==theTriangulation->edgeGlobal->P0->indice)
	{
		edgeD->elem1=&theTriangulation->elem[nElem+1];
	}	
	else
	{
		edgeD->elem0=&theTriangulation->elem[nElem+1];
	}	
				
	////////////////////// 3) scinder EdgeCentrale  /////////////////////////////
		
	theTriangulation->edges[nEdge+2].indice=nEdge+2;
	theTriangulation->edges[nEdge+2].P0=point;
	theTriangulation->edges[nEdge+2].P1=theTriangulation->edgeGlobal->P0;
	theTriangulation->edges[nEdge+2].elem0=&theTriangulation->elem[nElem];
	theTriangulation->edges[nEdge+2].elem1=&theTriangulation->elem[nElem+1];
	//////////////////////////////////////////////////////////////
	theTriangulation->edges[theTriangulation->edgeGlobal->indice].indice=theTriangulation->edgeGlobal->indice;
	theTriangulation->edges[theTriangulation->edgeGlobal->indice].P0=theTriangulation->edgeGlobal->P1;
	theTriangulation->edges[theTriangulation->edgeGlobal->indice].P1=point;
	theTriangulation->edges[theTriangulation->edgeGlobal->indice].elem0=&theTriangulation->elem[nElem+2];
	theTriangulation->edges[theTriangulation->edgeGlobal->indice].elem1=&theTriangulation->elem[nElem+3];
		
	theTriangulation->nEdge=nEdge+3;
	theTriangulation->nElem=nElem+4;// check
	theTriangulation->nElemReal=theTriangulation->nElemReal+2;
	////////////////////////////////////////////////////////////////////
	LegalizeEdge(point,edgeA,theTriangulation);
	LegalizeEdge(point,edgeB,theTriangulation);
	LegalizeEdge(point,edgeC,theTriangulation);
	LegalizeEdge(point,edgeD,theTriangulation); 
}
}

//////////////////////////////////////////////////

int IsLegal(Point *point, Edge *edge, Point *PK, Triangulation *theTriangulation)
{// on regarde si pk est dans le cercle circonscit au triangle formé par pijr
	
	// les 3 points exterieurs sont en dehros de tout cercle :-)
	if(PK->indice==0 || PK->indice== theTriangulation->nNode || PK->indice==theTriangulation->nNode+1)
	{
		return 1;//legal
	}
	
	// Calculer le centre C et rayon R: dist(C,PK)> R == Legal ?
	//double Ax= PK->x; FAUX JE PENSE, j'avais mis PK au lieu de point un peu partout
	//double Ay= PK->y; FAUX JE PENSE
	double Ax=point->x;
	double Ay=point->y;	
	double Bx= edge->P0->x;
	double By= edge->P0->y;
	double Cx= edge->P1->x;
	double Cy= edge->P1->y;
	double D=2*(Ax*(By-Cy) + Bx*(Cy-Ay) + Cx*(Ay-By));
	double centreX = ( (Ax*Ax + Ay*Ay)*(By-Cy) 
			+  (Bx*Bx + By*By)*(Cy-Ay)
			+  (Cx*Cx + Cy*Cy)*(Ay-By))/D;
	double centreY = ( (Ax*Ax + Ay*Ay)*(Cx-Bx) 
			+  (Bx*Bx + By*By)*(Ax-Cx)
			+  (Cx*Cx + Cy*Cy)*(Bx-Ax))/D;
	
	double Rayon =   (sqrt((Ax-Bx)*(Ax-Bx) + (Ay-By)*(Ay-By)))
			*(sqrt((Ax-Cx)*(Ax-Cx) + (Ay-Cy)*(Ay-Cy)))
			//*(sqrt((Bx-Cx)*(Bx-Cx) + (By-Cy)*(By-Cy)))/(4*triArea(*(edge->P0),*(edge->P1),*PK));// FAUX JE PENSE
			*(sqrt((Bx-Cx)*(Bx-Cx) + (By-Cy)*(By-Cy)))/(4*triArea(*(edge->P0),*(edge->P1),*point));
	if (sqrt((centreX-(PK->x))*(centreX-(PK->x)) + (centreY-(PK->y))*(centreY-(PK->y))) >=Rayon)
	{	
		 return 1; // c'est legal
	} 
	else
	{
		 return 0;// possibilite de mettre une tolerance pour pas faire des flips inutiles proches du carre TODO
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////

void PointLocate(Point *point,Triangulation *theTriangulation,myLeaf *leaves)
{ 

/*if(i==0)
{
theTriangulation->trigGlobal=&theTriangulation->elem[0];
//trigGlobal = leaves->theTriangle;// vieux truc surement foireux
theTriangulation->edgeGlobal = NULL;
}
else if (i==1)
{
theTriangulation->edgeGlobal = &theTriangulation->edges[3];
}*/
if (leaves->theChildren != NULL) // not the real leaves: check child by child
{
	int i = 0;
	int ind=0;
	for(i=0;i< leaves->nChildren;i++ )
	{
		if(withinTriangle(point,leaves->theChildren[i].theTriangle))
		{	
			printf("Le point %d est dans le fond du triangle %d\n",point->indice,leaves->theChildren[i].theTriangle->indice);
			ind=i;	
			break;
		}
	}
	PointLocate(point,theTriangulation,&(leaves->theChildren[ind]));
	//while (!(withinTriangle(point,leaves->theChildren[i].theTriangle)) && (i < leaves->nChildren))
	//{i++;} // si tu regarde bien la condition elle fait d'office une seg fault quand i==nChildren :D
	
}
else//theTriangulation->trigGlobal = leaves->theTriangle;
{	
	printf("Le point %d est-il onside du triangle %d \n",point->indice,leaves->theTriangle->indice);
	onSide(point,leaves->theTriangle,theTriangulation); 
}
}

int withinTriangle(Point *point,Triangle *triangle)
{
return ( fabs( triArea(*(triangle->sommet0),*(triangle->sommet1),*(triangle->sommet2)) - ( triArea(*point,*(triangle->sommet0),*(triangle->sommet1)) + triArea(*point,*(triangle->sommet1),*(triangle->sommet2))+ triArea(*point,*(triangle->sommet0),*(triangle->sommet2))  )    )  <= 0.0  );



//return (  triArea(*(triangle->sommet0),*(triangle->sommet1),*(triangle->sommet2)) == ( triArea(*(triangle->sommet0),*(triangle->sommet1),*point) + triArea(*(triangle->sommet1),*(triangle->sommet2),*point) + triArea(*(triangle->sommet1),*(triangle->sommet2),*point) )) ;
}

//---------------
double ptNorm(Point point1, Point point2)
{
	return sqrt( (point2.x-point1.x)*(point2.x-point1.x) + (point2.y-point1.y)*(point2.y-point1.y) );
}
//-------------
double crossProd(Point point0, Point point1, Point point2)
{
	return ( (point2.x-point0.x)*(point1.y-point0.y) - (point1.x-point0.x)*(point2.y-point0.y) );
}
///////////////////////////////
double triArea(Point point0, Point point1, Point point2)
{
	return (fabs(crossProd(point0,point1,point2)/2));
}
/////////////////////////////////////////////////////////
void onSide(Point *point, Triangle *triangle,Triangulation *theTriangulation)
{
	if ( crossProd(*point,*(triangle->sommet0),*(triangle->sommet1) ) == 0 )
	{
		theTriangulation->trigGlobal = NULL;
		theTriangulation->edgeGlobal = triangle->edge0;
	}
	else if( crossProd(*point,*(triangle->sommet1),*(triangle->sommet2) ) == 0 )
	{
		theTriangulation->trigGlobal = NULL;
		theTriangulation->edgeGlobal = triangle->edge1;
	}
	else if( crossProd(*point,*(triangle->sommet2),*(triangle->sommet0) ) ==0 )
	{
		theTriangulation->trigGlobal = NULL;
		theTriangulation->edgeGlobal = triangle->edge2;
	}
	else
	{
		theTriangulation->trigGlobal = triangle;
		theTriangulation->edgeGlobal = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
void LegalizeEdge(Point *point, Edge *edge, Triangulation *theTriangulation)
{
	printf(" l'indice de l'edge a legalize est %d \n",edge->indice);
	// les 3 edges du bord sont toujours legales :-)
	if(edge->indice==0 || edge->indice==1 || edge->indice==2)
	{
		printf("on legalize le bord\n");
		return;	
	}
	
	// 1) trouver pointeur vers PK
	Point *PK=NULL;
	Triangle *triBack=NULL;
	Triangle *triFront=NULL;
	if(edge->elem0->sommet0->indice != point->indice 
		&& edge->elem0->sommet1->indice != point->indice 
		&& edge->elem0->sommet2->indice != point->indice )
	{// PK est dans le triangle elem0
		triBack = edge->elem0;
		triFront = edge->elem1;
		if(edge->elem0->sommet0->indice != edge->P0->indice && edge->elem0->sommet0->indice != edge->P1->indice )
		{PK = edge->elem0->sommet0;}
		else if(edge->elem0->sommet1->indice != edge->P0->indice && edge->elem0->sommet1->indice != edge->P1->indice )
		{PK = edge->elem0->sommet1;}
		else
		{PK = edge->elem0->sommet2;}
	}
	else
	{// PK est dans triangle elem1
		triBack = edge->elem1;
		triFront = edge->elem0;
		if(edge->elem1->sommet0->indice != edge->P0->indice && edge->elem1->sommet0->indice != edge->P1->indice )
		{PK = edge->elem1->sommet0;}
		else if(edge->elem1->sommet1->indice != edge->P0->indice && edge->elem1->sommet1->indice != edge->P1->indice )
		{PK = edge->elem1->sommet1;}
		else
		{PK = edge->elem1->sommet2;}
	}
	
	if( IsLegal(point, edge,PK,theTriangulation)==0 ) 
	{					
	// 1) on switch
	printf("SWITCH ON !! \n");
	
	// les edge A B C D
	
	Edge *edgeA=NULL;
	Edge *edgeC=NULL;
	if (triBack->edge0->P0->indice != edge->P0->indice && triBack->edge0->P1->indice != edge->P0->indice)
	{// triBack->edge0 c'est edgeA
		edgeA=triBack->edge0;	
		if (triBack->edge1->P0->indice != edge->P1->indice && triBack->edge1->P1->indice != edge->P1->indice)	
		{// on veut voir si edgeC c'est triBack->edge1 
			edgeC=triBack->edge1;
		}
		else{edgeC=triBack->edge2;}

	}
	else if (triBack->edge1->P0->indice != edge->P0->indice && triBack->edge1->P1->indice != edge->P0->indice)
	{// triBack->edeg1 c'est edgeA
		edgeA=triBack->edge1;
		if (triBack->edge0->P0->indice != edge->P1->indice && triBack->edge0->P1->indice != edge->P1->indice  )
		{// on veut voir si triBack->edeg0 c'est edgeC
			edgeC = triBack->edge0;
		}
		else{edgeC = triBack->edge2;}	
	}
	else
	{
		edgeA=triBack->edge2;
		if(triBack->edge0->P0->indice != edge->P1->indice && triBack->edge0->P1->indice != edge->P1->indice)
		{// voir si triBack->edge0 est en edgeC 
			edgeC=triBack->edge0;
		}
		else{edgeC=triBack->edge1;}
	}
	
	Edge *edgeB=NULL;
	Edge *edgeD=NULL;
	if (triFront->edge0->P0->indice != edge->P0->indice && triFront->edge0->P1->indice != edge->P0->indice)
	{// triFront->edge0 c'est edgeB
		edgeB=triFront->edge0;	
		if (triFront->edge1->P0->indice != edge->P1->indice && triFront->edge1->P1->indice != edge->P1->indice)	
		{// on veut voir si edgeD c'est triFront->edge1 
			edgeD=triFront->edge1;
		}
		else{edgeD=triFront->edge2;}

	}
	else if (triFront->edge1->P0->indice != edge->P0->indice && triFront->edge1->P1->indice != edge->P0->indice)
	{// triFront->edeg1 c'est edgeB
		edgeB=triFront->edge1;
		if (triFront->edge0->P0->indice != edge->P1->indice && triFront->edge0->P1->indice != edge->P1->indice  )
		{// on veut voir si triFront->edeg0 c'est edgeD
			edgeD = triFront->edge0;
		}
		else{edgeD = triFront->edge2;}	
	}
	else
	{
		edgeB=triFront->edge2;
		if(triFront->edge0->P0->indice != edge->P1->indice && triFront->edge0->P1->indice != edge->P1->indice)
		{// voir si triFront->edge0 est en edgeD 
			edgeD=triFront->edge0;
		}
		else{edgeD=triFront->edge1;}
	}// TODO relire le chopage des edges ABCD
	
	
	// TODO leaf a check
	triBack->theLeaf->nChildren=2;
	triBack->theLeaf->theChildren=malloc(2*sizeof(myLeaf));
	triBack->theLeaf->theChildren[0].theTriangle=&theTriangulation->elem[theTriangulation->nElem];
	triBack->theLeaf->theChildren[0].theChildren=NULL;
	triBack->theLeaf->theChildren[0].nChildren=0;
	triBack->theLeaf->theChildren[1].theTriangle=&theTriangulation->elem[theTriangulation->nElem+1];
	triBack->theLeaf->theChildren[1].theChildren=NULL;
	triBack->theLeaf->theChildren[1].nChildren=0;
	theTriangulation->elem[theTriangulation->nElem].theLeaf=&triBack->theLeaf->theChildren[0];// TODO problem voir + bas 
	theTriangulation->elem[theTriangulation->nElem+1].theLeaf=&triBack->theLeaf->theChildren[1];
	////////////////////////////////////////////
	triFront->theLeaf->nChildren=2;
	triFront->theLeaf->theChildren=triBack->theLeaf->theChildren;// ? IDEE :-) faut être sur que ça modifie les deux mais normalementok
	/*triFront->theLeaf->theChildren=malloc(2*sizeof(myLeaf));
	triFront->theLeaf->theChildren[0].theTriangle=&theTriangulation->elem[theTriangulation->nElem];
	triFront->theLeaf->theChildren[0].theChildren=NULL;
	triFront->theLeaf->theChildren[0].nChildren=0;
	triFront->theLeaf->theChildren[1].theTriangle=&theTriangulation->elem[theTriangulation->nElem+1];
	triFront->theLeaf->theChildren[1].theChildren=NULL;
	triFront->theLeaf->theChildren[1].nChildren=0;
	theTriangulation->elem[theTriangulation->nElem].theLeaf=&triFront->theLeaf->theChildren[0];// TODO je ne sais plus trop pourquoi les triangles
	theTriangulation->elem[theTriangulation->nElem+1].theLeaf=&triFront->theLeaf->theChildren[1];// on un pointeur sur leur feuille, il en faut 2 ??
	// j'ai quand même le sentiment que cela risque de poser un problem si on fait un test sur le trig qui correpsond a l'autre 
	//	feuille.. a voir ..
	*/
	
	// les sommets des 2 nouveaux triangles
	theTriangulation->elem[theTriangulation->nElem].indice=theTriangulation->nElem;
	theTriangulation->elem[theTriangulation->nElem].sommet0=point;
	theTriangulation->elem[theTriangulation->nElem].sommet1=PK;
	theTriangulation->elem[theTriangulation->nElem].sommet2=edge->P1;
	theTriangulation->elem[theTriangulation->nElem+1].indice=theTriangulation->nElem+1;
	theTriangulation->elem[theTriangulation->nElem+1].sommet0=point;
	theTriangulation->elem[theTriangulation->nElem+1].sommet1=edge->P0;
	theTriangulation->elem[theTriangulation->nElem+1].sommet2=PK;
	
	 
	// l'edge qui flip
	theTriangulation->edges[edge->indice].P0=point;
	theTriangulation->edges[edge->indice].P1=PK;
	theTriangulation->edges[edge->indice].elem0=&theTriangulation->elem[edge->elem0->indice];// devrait être 
	theTriangulation->edges[edge->indice].elem1=&theTriangulation->elem[edge->elem1->indice];// inutile
		
	// les edges des 2 nouveaux triangles 
	theTriangulation->elem[theTriangulation->nElem].edge0= &theTriangulation->edges[edge->indice];// normalement on peut mettre ca
	theTriangulation->elem[theTriangulation->nElem].edge1= edgeA;
	theTriangulation->elem[theTriangulation->nElem].edge2= edgeB;
	theTriangulation->elem[theTriangulation->nElem+1].edge0= edgeD;
	theTriangulation->elem[theTriangulation->nElem+1].edge1= edgeC;
	theTriangulation->elem[theTriangulation->nElem+1].edge2= &theTriangulation->edges[edge->indice];// avant "edge qui flip" ca change rien
		
	// mettre à jour les pointeurs edge->triangle pour les edges du bord qui bougent pas: ABCD
	if (edgeA->P0->indice == edge->P1->indice )// ici edge->P1 c'est PK car on a update
	{
		edgeA->elem0=&theTriangulation->elem[theTriangulation->nElem];	
	}
	else
	{
		edgeA->elem1=&theTriangulation->elem[theTriangulation->nElem];		
	}
	
	if (edgeC->P0->indice == edge->P1->indice)
	{
		edgeC->elem1=&theTriangulation->elem[theTriangulation->nElem+1];
	}
	else
	{
		edgeC->elem0=&theTriangulation->elem[theTriangulation->nElem+1];
	}
	
	
	if (edgeB->P0->indice == edge->P0->indice)
	{
		edgeB->elem1=&theTriangulation->elem[theTriangulation->nElem];
	}
	else
	{
		edgeB->elem0=&theTriangulation->elem[theTriangulation->nElem];
	}
	if (edgeD->P0->indice == edge->P0->indice)
	{
		edgeD->elem0=&theTriangulation->elem[theTriangulation->nElem+1];
	}
	else
	{
		edgeD->elem1=&theTriangulation->elem[theTriangulation->nElem+1];
	}
	
	theTriangulation->nElem=theTriangulation->nElem+2;// TODO chekc mais semble ok
	// 2) on refait des LegalizeEdge 
	LegalizeEdge(point, edgeA , theTriangulation);
	LegalizeEdge(point, edgeC , theTriangulation);
	}

}
//////////////////////////////////// CREATE  ////////////////////////////////////////////////////////////
Triangulation *TriangulationCreate(char *FileName)
{
// lire le fichier data :-)

Triangulation *theTriangulation = malloc(sizeof(Triangulation));

    int i,trash;
    
    FILE* file = fopen(FileName,"r");

    trash = fscanf(file, "Number of nodes %d \n", &theTriangulation->nNode);
    int nNode=theTriangulation->nNode;
    
    theTriangulation->points = malloc(sizeof(Point)*(nNode+2));
    for (i = 0; i < nNode; ++i) {
	trash = fscanf(file,"%d : %le %le \n",&theTriangulation->points[i].indice,
					      &theTriangulation->points[i].x,
					      &theTriangulation->points[i].y);
         }
    theTriangulation->nElem = 1;
    theTriangulation->nElemReal = 1;    // valeur au départ
    
    findP0(theTriangulation);
        
    theTriangulation->elem  = malloc(sizeof(Triangle)*(9*nNode +1));
    theTriangulation->edges = malloc(sizeof(Edge)*(3*nNode -3));
    
    
    // TODO initialiser p-1 et p-2 en fonction des données
	
	// init p-1 en (0.0 ,-1.0)
	theTriangulation->points[nNode].x= 0.0;
	theTriangulation->points[nNode].y=-1.0;
	theTriangulation->points[nNode].indice=nNode;
	
	// init p-2 en (-10.0 , 10.0 )
	theTriangulation->points[nNode+1].x=-10.0;
	theTriangulation->points[nNode+1].y= 10.0;
	theTriangulation->points[nNode+1].indice=nNode+1;
	
	// Anti-horlogique 
	// init le triangle[0]	
	theTriangulation->elem[0].indice= 0;
	theTriangulation->elem[0].sommet0= &theTriangulation->points[0];     // pointe vers p0;
	theTriangulation->elem[0].sommet1= &theTriangulation->points[nNode+1];
	theTriangulation->elem[0].sommet2= &theTriangulation->points[nNode];
	theTriangulation->elem[0].edge0 = &theTriangulation->edges[0];
	theTriangulation->elem[0].edge1 = &theTriangulation->edges[1];
	theTriangulation->elem[0].edge2 = &theTriangulation->edges[2]; 

    // initialiser les 3 first edges:     
    theTriangulation->edges[0].indice=0;
    theTriangulation->edges[0].elem0=&theTriangulation->elem[0];
    theTriangulation->edges[0].elem1=NULL;
    theTriangulation->edges[0].P0=&theTriangulation->points[0];
    theTriangulation->edges[0].P1=&theTriangulation->points[nNode+1];

    theTriangulation->edges[1].indice=1;
    theTriangulation->edges[1].elem0=&theTriangulation->elem[0];
    theTriangulation->edges[1].elem1=NULL;
    theTriangulation->edges[1].P0=&theTriangulation->points[nNode+1];
    theTriangulation->edges[1].P1=&theTriangulation->points[nNode];

    theTriangulation->edges[2].indice=2;
    theTriangulation->edges[2].elem0=&theTriangulation->elem[0];
    theTriangulation->edges[2].elem1=NULL;
    theTriangulation->edges[2].P0=&theTriangulation->points[nNode];
    theTriangulation->edges[2].P1=&theTriangulation->points[0];  // FIN de anti-horlogique
    								
    theTriangulation->nEdge=3;
    
       // tree initialization
    // myLeaf *triangleZero = malloc(sizeof(myLeaf*)); SEG FAULT
     myLeaf *triangleZero = malloc(sizeof(myLeaf));
     triangleZero->theTriangle = theTriangulation->elem;
     triangleZero->theChildren = NULL;
     triangleZero->nChildren = 0;
     //theTriangulation->theTree = malloc(sizeof(myTree*)); SEG FAULT
     theTriangulation->theTree = malloc(sizeof(myTree));
     theTriangulation->theTree->theRoot = triangleZero;    
     theTriangulation->elem[0].theLeaf=theTriangulation->theTree->theRoot;
    fclose(file);
    
    return theTriangulation;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void findP0(Triangulation *theTriangulation)
{

//1) on cherche l'indice de P0
int ind=0;
int i;
for(i=1;i<theTriangulation->nNode; i++)
{
	if(theTriangulation->points[i].y > theTriangulation->points[ind].y)
	{
		ind =i;
	}
	else if(theTriangulation->points[i].y == theTriangulation->points[ind].y && theTriangulation->points[i].x > theTriangulation->points[ind].x ){
		ind = i;
	}
}

// 2) switch pour mettre P0 devant :-)
Point trash = theTriangulation->points[0]; 
theTriangulation->points[0]=theTriangulation->points[ind];
theTriangulation->points[ind]=trash;

theTriangulation->points[0].indice=0;
theTriangulation->points[ind].indice=ind;
}

////////////////////////////////////////////////////////////////////////////////////////////////
int *ComputeRandom(int n)
{// returns a vector of int values from 1 to n in a random order
int *tab= malloc(sizeof(int)*n);
int i;
for(i=0;i<n;i++)// valeur par defaut non aléatoire :-)
	{tab[i]=i+1;}
return tab; 
}

void TriangulationFree(Triangulation *theTriangulation)
{
	// TODO PA: je te laisse free ton Tree ;-)
    free(theTriangulation->edges);
    free(theTriangulation->points);
    free(theTriangulation->elem);
    free(theTriangulation);
}



void TriangulationWriteIter(const char *PlotName,Triangulation *theTriangulation, int iter)
{    const char *basename = "%s-%08d.txt";
    char filename[256];
    sprintf(filename,basename,PlotName,iter);
//    FILE* file = fopen(filename,"w");

   TriangulationWritePlot(filename,theTriangulation);
 }  
   

void TriangulationWrite(const char *ResultName,Triangulation *theTriangulation)
{
    int i;
    
    FILE* file = fopen(ResultName,"w");
    
    fprintf(file, "Number of nodes %d \n", theTriangulation->nNode);
    for (i = 0; i < theTriangulation->nNode+2; i++) {
        fprintf(file,"%6d : %14.7e %14.7e \n",i,theTriangulation->points[i].x,theTriangulation->points[i].y); }
    
  	     fprintf(file, "Number of triangles %d \n", theTriangulation->nElemReal);  
        for (i = 0; i < theTriangulation->nElem; i++) 
        {
            if(theTriangulation->elem[i].theLeaf->theChildren==NULL)	
            {fprintf(file,"%6d : %6d %6d %6d \n", i,theTriangulation->elem[i].sommet0->indice,
            				           theTriangulation->elem[i].sommet1->indice,
            				           theTriangulation->elem[i].sommet2->indice);}
        }   
     fprintf(file, "Number of edges %d \n", theTriangulation->nEdge);
     	for(i=0; i < theTriangulation->nEdge; i++){
     		fprintf(file,"%6d : %6d %6d \n",i,theTriangulation->edges[i].P0->indice,
     						  theTriangulation->edges[i].P1->indice);}
     	
    fclose(file);
}



void TriangulationWritePlot(const char *PlotName,Triangulation *theTriangulation)
{// n'ecrit que les triangles vivants dans le fichier, donc les morts ne sont aps sur le dessin opengl
     int i;
    
     FILE* file = fopen(PlotName,"w");
    fprintf(file,"Number of triangles %d \n",theTriangulation->nElemReal);
     for (i = 0; i < theTriangulation->nElem; i++) 
        {
            if(theTriangulation->elem[i].theLeaf->theChildren==NULL)	
            {fprintf(file,"%6d : %6f %6f %6f %6f %6f %6f \n", i,theTriangulation->elem[i].sommet0->x,
            					theTriangulation->elem[i].sommet1->x,
            					theTriangulation->elem[i].sommet2->x,
            					theTriangulation->elem[i].sommet0->y,
            					theTriangulation->elem[i].sommet1->y,
            					theTriangulation->elem[i].sommet2->y);}
        }   
    fclose(file);
}


void TriangulationWriteAll(const char *ResultName,Triangulation *theTriangulation)
{
    int i;
    
    FILE* file = fopen(ResultName,"w");
    
    fprintf(file, "Number of nodes %d \n", theTriangulation->nNode);
       for (i = 0; i < theTriangulation->nNode+2; i++) {
        fprintf(file,"%6d : %14.7e %14.7e \n",i,theTriangulation->points[i].x,theTriangulation->points[i].y); }
    
  	     fprintf(file, "Number of triangles %d \n", theTriangulation->nElem);  
        for (i = 0; i < theTriangulation->nElem; i++) {
            fprintf(file,"%6d : %6d %6d %6d \n", i,theTriangulation->elem[i].sommet0->indice,
            				           theTriangulation->elem[i].sommet1->indice,
            				           theTriangulation->elem[i].sommet2->indice);}
     
     fprintf(file, "Number of edges %d \n", theTriangulation->nEdge);
    
     	for(i=0; i < theTriangulation->nEdge; i++){
     		fprintf(file,"%6d : %6d %6d \n",i,theTriangulation->edges[i].P0->indice,
     						  theTriangulation->edges[i].P1->indice);}     	
    printf("okay edges\n");
    fclose(file);
}
  
  
  /*
void femEdgesPrint(femEdges *theEdges)
{
    int i;    
    for (i = 0; i < theEdges->nEdge; ++i) {
        printf("%6d : %4d %4d : %4d %4d \n",i,
               theEdges->edges[i].node[0],theEdges->edges[i].node[1],
               theEdges->edges[i].elem[0],theEdges->edges[i].elem[1]); }
}

*/




/*
void Error(char *text, int line, char *file)                                  
{ 
    printf("\n-------------------------------------------------------------------------------- ");
    printf("\n  Error in %s at line %d : \n  %s\n", file, line, text);
    printf("--------------------------------------------------------------------- Yek Yek !! \n\n");
    exit(69);                                                 
}

void femWarning(char *text, int line, char *file)                                  
{ 
    printf("\n-------------------------------------------------------------------------------- ");
    printf("\n  Warning in %s at line %d : \n  %s\n", file, line, text);
    printf("--------------------------------------------------------------------- Yek Yek !! \n\n");                                              
}

*/


