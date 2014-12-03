/*
 *  delaunay.c
 *  
 */

#include "delaunay.h"
#define Error(a)   Error(a,__LINE__,__FILE__)
#define Warning(a) Warning(a,  __LINE__, __FILE__)

Triangulation *triangulation(char *FileName, const char *ResultName)
{
printf("on est dans triangulation \n");

Triangulation *theTriangulation = TriangulationCreate(FileName);
TriangulationWrite(ResultName, theTriangulation);

// TODO: ComputeRandom: un vecteur avec les nombres de 1 à n+1 mélangés pour voir dans quel ordre on ajoute les pts
// ce vecteur s'appelle de random. 
int *random = ComputeRandom(theTriangulation->nNode-1);

int i=0;
for(i=0; i<theTriangulation->nNode; i++)
{
    AddPoint(&(theTriangulation->points[random[i]]), theTriangulation);
    if(i==0){break;}
}

/*printf("triangle 0: indice: %d \n",theTriangulation->elem[0].indice);
printf("triangle 0: sommet0->indice: %d \n",theTriangulation->elem[0].sommet0->indice);
printf("triangle 0: sommet1->indice: %d \n",theTriangulation->elem[0].sommet1->indice);
printf("triangle 0: sommet2->indice: %d \n",theTriangulation->elem[0].sommet2->indice);

printf("triangle 0: edge0->indice: %d \n",theTriangulation->elem[0].edge0->indice);
printf("triangle 0: edge1->indice: %d \n",theTriangulation->elem[0].edge1->indice);
printf("triangle 0: edge2->indice: %d \n",theTriangulation->elem[0].edge2->indice);*/


// TODO: RemoveExtraPoints (enlever les points -1 et -2 ainsi que les edges qui les touchent)

TriangulationWrite(ResultName, theTriangulation);
return theTriangulation;
//TriangulationFree(theTriangulation);
}
/////////////////////////////////////////////////////////////////////////////////
void AddPoint(Point *point, Triangulation *theTriangulation)// TODO ajouter les pointeurs sur les edges
{
	theTriangulation->trigGlobal =NULL;
	theTriangulation->edgeGlobal=NULL;

	int nEdge=theTriangulation->nEdge;
	int nElem=theTriangulation->nElem;
	
	PointLocate(point,theTriangulation,theTriangulation->theTree->theRoot); 
	
	if(theTriangulation->trigGlobal != NULL)
	{// point est inside trigGlobal
	 	 printf("on est dans un triangle \n ");
	 	 
	/* AJOUT DES 3 TRIANGLES */
	
	// garde une trace du triangle d'origine car on va le casser ..
	Point *pt0=theTriangulation->elem[theTriangulation->trigGlobal->indice].sommet0;
	Point *pt1=theTriangulation->elem[theTriangulation->trigGlobal->indice].sommet1;
	Point *pt2=theTriangulation->elem[theTriangulation->trigGlobal->indice].sommet2;
	int indiceGlobal = theTriangulation->trigGlobal->indice;
	Edge *edge0Global= theTriangulation->trigGlobal->edge0;
	Edge *edge1Global= theTriangulation->trigGlobal->edge1;
	Edge *edge2Global= theTriangulation->trigGlobal->edge2;
	///////////////////////////////////////////////////////////
	theTriangulation->elem[nElem].sommet0=point;
	theTriangulation->elem[nElem].sommet1=theTriangulation->trigGlobal->sommet0;
	theTriangulation->elem[nElem].sommet2=theTriangulation->trigGlobal->sommet1;
	theTriangulation->elem[nElem].indice=nElem;
	/////////////////////////////////////////////////////////////
	theTriangulation->elem[nElem+1].sommet0=point;
	theTriangulation->elem[nElem+1].sommet1=theTriangulation->trigGlobal->sommet1;
	theTriangulation->elem[nElem+1].sommet2=theTriangulation->trigGlobal->sommet2;
	theTriangulation->elem[nElem+1].indice=nElem+1;
	///////////////////////////////////////////////////////////
	theTriangulation->elem[theTriangulation->trigGlobal->indice].sommet0=point;
	theTriangulation->elem[theTriangulation->trigGlobal->indice].sommet1=pt2;
	theTriangulation->elem[theTriangulation->trigGlobal->indice].sommet2=pt0;
	theTriangulation->elem[theTriangulation->trigGlobal->indice].indice=theTriangulation->trigGlobal->indice;
	
	theTriangulation->nElem=nElem+2;
	
	/* AJOUT DES EDGES */
	theTriangulation->edges[nEdge].P0=point;
	theTriangulation->edges[nEdge].P1=pt0;
	theTriangulation->edges[nEdge].indice=nEdge;	
	theTriangulation->edges[nEdge].elem0=&theTriangulation->elem[nElem];
	theTriangulation->edges[nEdge].elem1=&theTriangulation->elem[indiceGlobal];
	//////////////////////////////////////////////////////
	theTriangulation->edges[nEdge+1].P0=point;
	theTriangulation->edges[nEdge+1].P1=pt1;
	theTriangulation->edges[nEdge+1].indice=nEdge+1;
	theTriangulation->edges[nEdge+1].elem0=&theTriangulation->elem[nElem+1];
	theTriangulation->edges[nEdge+1].elem1=&theTriangulation->elem[nElem];
	///////////////////////////////////////////////////////////////
	theTriangulation->edges[nEdge+2].P0=point;
	theTriangulation->edges[nEdge+2].P1=pt2;
	theTriangulation->edges[nEdge+2].indice=nEdge+2;
	theTriangulation->edges[nEdge+2].elem0=&theTriangulation->elem[indiceGlobal];
	theTriangulation->edges[nEdge+2].elem1=&theTriangulation->elem[nElem+1];
	
	theTriangulation->nEdge=nEdge+3;
	
	//// Pointeurs triangles->edges ///
	theTriangulation->elem[nElem].edge0= &theTriangulation->edges[nEdge]; 
	theTriangulation->elem[nElem].edge1= edge0Global;
	theTriangulation->elem[nElem].edge2= &theTriangulation->edges[nEdge+1];
	theTriangulation->elem[nElem+1].edge0= &theTriangulation->edges[nEdge+1];
	theTriangulation->elem[nElem+1].edge1= edge1Global;
	theTriangulation->elem[nElem+1].edge2= &theTriangulation->edges[nEdge+2];
	theTriangulation->elem[theTriangulation->trigGlobal->indice].edge0= &theTriangulation->edges[nEdge+2]; 
	theTriangulation->elem[theTriangulation->trigGlobal->indice].edge1= edge2Global;
	theTriangulation->elem[theTriangulation->trigGlobal->indice].edge2= &theTriangulation->edges[nEdge];
	
	LegalizeEdge(point, &theTriangulation->edges[nEdge], theTriangulation);
	LegalizeEdge(point, &theTriangulation->edges[nEdge+1], theTriangulation);
	LegalizeEdge(point, &theTriangulation->edges[nEdge+2], theTriangulation);
	/////////////////////////////////////////////////////////////////////
	}
	else if(theTriangulation->edgeGlobal != NULL)// TODO ajouter les pointeurs Triangle->Edge
	{// le point est sur une edge
	printf("on est sur une edge \n ");
	
	//////////////////////////////// 1) triangle0 ///////////////////
	Triangle *triangle0=theTriangulation->edgeGlobal->elem0;
	Edge *edgeA = triangle0->edge0;
	Edge *edgeB = triangle0->edge1;
	if (edgeA->indice == theTriangulation->edgeGlobal->indice)
	{
		edgeA = triangle0->edge2;// A et B sont ok 
	}
	else if (edgeB->indice == theTriangulation->edgeGlobal->indice)
	{
		edgeB = triangle0->edge2;// A et B sont ok 
	}
	if (edgeA->P0->indice != theTriangulation->edgeGlobal->P1->indice && edgeA->P1->indice != theTriangulation->edgeGlobal->P1->indice)
	{// switch si edgeA ne touche pas le P1 de l'edge centrale
		Edge *trash=edgeA;
		edgeA=edgeB;
		edgeB=trash;
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
	theTriangulation->elem[nElem].indice=nElem;
	theTriangulation->elem[nElem].sommet0=point;
	theTriangulation->elem[nElem].sommet1=theTriangulation->edgeGlobal->P0;	
	theTriangulation->elem[nElem].sommet2=point_exterieur0;	
	
	theTriangulation->elem[nElem].edge0= &theTriangulation->edges[nEdge+2];
	theTriangulation->elem[nElem].edge1= edgeB;
	theTriangulation->elem[nElem].edge2= &theTriangulation->edges[nEdge];  // TODO check
	
	//////////////////////////////////////////////////////////////
	theTriangulation->elem[triangle0->indice].indice=triangle0->indice;
	theTriangulation->elem[triangle0->indice].sommet0=point;
	theTriangulation->elem[triangle0->indice].sommet1=theTriangulation->edgeGlobal->P1;
	theTriangulation->elem[triangle0->indice].sommet2=point_exterieur0;  	
	
	theTriangulation->elem[triangle0->indice].edge0= &theTriangulation->edges[theTriangulation->edgeGlobal->indice];
	theTriangulation->elem[triangle0->indice].edge1= edgeA;
	theTriangulation->elem[triangle0->indice].edge2= &theTriangulation->edges[nEdge]; // TODO check
	
	/* AJOUT DE 1 EDGE */
	theTriangulation->edges[nEdge].indice=nEdge;
	theTriangulation->edges[nEdge].P0=point;
	theTriangulation->edges[nEdge].P1=point_exterieur0;
	theTriangulation->edges[nEdge].elem0=&theTriangulation->elem[triangle0->indice];
	theTriangulation->edges[nEdge].elem1=&theTriangulation->elem[nElem];
	
	//////////////////////////////// 2) triangle1 ///////////////////
	Triangle *triangle1=theTriangulation->edgeGlobal->elem1;
	Edge *edgeC=triangle1->edge0;
	Edge *edgeD=triangle1->edge1;
	if (edgeC->indice == theTriangulation->edgeGlobal->indice)
	{
		edgeC=triangle1->edge2;
	}
	else if (edgeD->indice == theTriangulation->edgeGlobal->indice)
	{
		edgeD=triangle1->edge2;
	}
	if (edgeC->indice != theTriangulation->edgeGlobal->P1->indice && edgeC->indice != theTriangulation->edgeGlobal->P1->indice)		
	{
	Edge *trashh = edgeC;
	edgeC = edgeD;
	edgeD = trashh;
	}

	/* Find point_exterieur1 */
	Point *point_exterieur1=NULL;
	//int ind_node0=0;
	if(  triangle1->sommet0->indice != theTriangulation->edgeGlobal->P0->indice
			 && triangle1->sommet0->indice != theTriangulation->edgeGlobal->P1->indice )
	{
		point_exterieur1=triangle1->sommet0;
		//ind_node0 = 0; 
	}
	else if(  triangle1->sommet1->indice != theTriangulation->edgeGlobal->P0->indice 
			&& triangle1->sommet1->indice != theTriangulation->edgeGlobal->P1->indice )
	{
		point_exterieur1=triangle1->sommet1;
		//ind_node0 = 1; 
	}
	else
	{
		point_exterieur1=triangle1->sommet2;
		//ind_node0 = 2;
	}
	
	/* AJOUT DE 2 TRIG */
	theTriangulation->elem[nElem+1].indice=nElem+1;
	theTriangulation->elem[nElem+1].sommet0=point;
	theTriangulation->elem[nElem+1].sommet1=theTriangulation->edgeGlobal->P0;
	theTriangulation->elem[nElem+1].sommet2=point_exterieur1;
		
	theTriangulation->elem[nElem+1].edge0= &theTriangulation->edges[nEdge+2];
	theTriangulation->elem[nElem+1].edge1= edgeD;
	theTriangulation->elem[nElem+1].edge2= &theTriangulation->edges[nEdge+1]; // TODO checker
		
	///////////////////////////////////////////////////////
	theTriangulation->elem[triangle1->indice].indice=triangle1->indice;
	theTriangulation->elem[triangle1->indice].sommet0=point;
	theTriangulation->elem[triangle1->indice].sommet1=theTriangulation->edgeGlobal->P1;
	theTriangulation->elem[triangle1->indice].sommet2=point_exterieur1;
		
	theTriangulation->elem[triangle1->indice].edge0= &theTriangulation->edges[theTriangulation->edgeGlobal->indice];
	theTriangulation->elem[triangle1->indice].edge1= edgeC;
	theTriangulation->elem[triangle1->indice].edge2= &theTriangulation->edges[nEdge+1]; // TODO checker
		
		
	/* AJOUT DE 1 EDGE */
	theTriangulation->edges[nEdge+1].indice=nEdge+1;
	theTriangulation->edges[nEdge+1].P0=point;
	theTriangulation->edges[nEdge+1].P1=point_exterieur1;
	theTriangulation->edges[nEdge+1].elem0=&theTriangulation->elem[nElem+1];
	theTriangulation->edges[nEdge+1].elem1=&theTriangulation->elem[triangle1->indice];
		
		
	// TODO vérifier que l'ordre des opérations pour les pointeurs est ok
		
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
	theTriangulation->edges[theTriangulation->edgeGlobal->indice].elem0=&theTriangulation->elem[triangle0->indice];
	theTriangulation->edges[theTriangulation->edgeGlobal->indice].elem1=&theTriangulation->elem[triangle1->indice];
		
	theTriangulation->nEdge=nEdge+3;
	theTriangulation->nElem=nElem+2;
	////////////////////////////////////////////////////////////////////
	LegalizeEdge(point,&theTriangulation->edges[theTriangulation->edgeGlobal->indice],theTriangulation);
	LegalizeEdge(point,&theTriangulation->edges[nEdge],theTriangulation);
	LegalizeEdge(point,&theTriangulation->edges[nEdge+1],theTriangulation);
	LegalizeEdge(point,&theTriangulation->edges[nEdge+2],theTriangulation); 
}
}

//////////////////////////////////////////////////

int IsLegal(Edge *edge, Triangulation *theTriangulation)
{

return 1; // true
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
/////////////////////////////////////////////////////////////////////////////////////////////

void PointLocate(Point *point,Triangulation *theTriangulation,myLeaf *leaves)
{ 
theTriangulation->trigGlobal=&theTriangulation->elem[0];
//trigGlobal = leaves->theTriangle;
theTriangulation->edgeGlobal = NULL;

/*if (leaves->theChildren != NULL) // not the real leaves: check child by child
{
	int i = 0;
	while (!(withinTriangle(point,leaves->theChildren[i].theTriangle)) && (i < leaves->nChildren))
	i++;
	PointLocate(point,theTriangulation,&(leaves->theChildren[i]));
}
else//theTriangulation->trigGlobal = leaves->theTriangle;
onSide(point,leaves->theTriangle,theTriangulation); //TODO: onSide*/
}

int withinTriangle(Point *point,Triangle *triangle)
{
return ( triArea(*(triangle->sommet0),*(triangle->sommet1),*(triangle->sommet2)) == ( triArea(*(triangle->sommet0),*(triangle->sommet1),*point) + triArea(*(triangle->sommet1),*(triangle->sommet2),*point) + triArea(*(triangle->sommet1),*(triangle->sommet2),*point) ) );
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
	return (crossProd(point0,point1,point2)/2);
}
/////////////////////////////////////////////////////////
void onSide(Point *point, Triangle *triangle,Triangulation *theTriangulation)
{
	if ( crossProd(*point,*(triangle->sommet0),*(triangle->sommet1) ) == 0 )
	{
		theTriangulation->trigGlobal = NULL;
		theTriangulation->edgeGlobal = NULL;// need edges related to the triangle :-(
	}
	else if( crossProd(*point,*(triangle->sommet1),*(triangle->sommet2) ) == 0 )
	{
		theTriangulation->trigGlobal = NULL;
		theTriangulation->edgeGlobal = NULL;// need edges related to the triangle :-(
	}
	else if( crossProd(*point,*(triangle->sommet2),*(triangle->sommet0) ) ==0 )
	{
		theTriangulation->trigGlobal = NULL;
		theTriangulation->edgeGlobal = NULL;// need edges related to the triangle :-(
	}
	else
	{
		theTriangulation->trigGlobal = triangle;
		theTriangulation->edgeGlobal = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
void LegalizeEdge(Point *point, Edge *edge, Triangulation *theTriangulation)
{// TODO
	if( IsLegal(edge,theTriangulation)==0 )// TODO attention, IsLegal a peut être besoin de point pour être efficace 
	{					//et ne pas tester trop de truc (on sait ce qu'on vient d'ajouter)
						// au pire on fait le IsLegal dans le AddPoint et on appelera Legalize que s'il
						// faut legaliser :-)
	
	
	// 1) on switch
	// 2) on refait des LegalizeEdge 
	//LegalizeEdge(point,   ,theTriangulation);
	//LegalizeEdge(point,   ,theTriangulation);// TODO à remplir :-)
	
	
	}

}
//////////////////////////////////// CREATE  ////////////////////////////////////////////////////////////
Triangulation *TriangulationCreate(char *FileName)
{
// lire le fichier data :-)

Triangulation *theTriangulation = malloc(sizeof(Triangulation));

    int i,trash;
    
    FILE* file = fopen(FileName,"r");
    //if (file == NULL) Error("No data file !",35,"something");

    trash = fscanf(file, "Number of nodes %d \n", &theTriangulation->nNode);
    int nNode=theTriangulation->nNode;
    
    theTriangulation->points = malloc(sizeof(Point)*(nNode+2));
    for (i = 0; i < nNode; ++i) {
	trash = fscanf(file,"%d : %le %le \n",&theTriangulation->points[i].indice,
					      &theTriangulation->points[i].x,
					      &theTriangulation->points[i].y);
         }
    theTriangulation->nElem = 1;    // valeur au départ
    
    findP0(theTriangulation);
        
    theTriangulation->elem  = malloc(sizeof(Triangle)*(2*nNode -2));//TODO ajuster ce nombre normalement ce sont des bornes sup 
    theTriangulation->edges = malloc(sizeof(Edge)*(3*nNode -3));// sur la taille possible des trucs (k=0 Thm9.1)
    								// je crois qu'on peut mettre k=3 et faire nNode+2 comme on met tout
    								// dans un grand triangle
    // TODO traiter p-1 et p-2 de manière symbolique 
	
	// init p-1 en (0.0 ,-1.0)
	theTriangulation->points[nNode].x= 0.0;
	theTriangulation->points[nNode].y=-1.0;
	theTriangulation->points[nNode].indice=nNode;
	
	// init p-2 en (-10.0 , 10.0 )
	theTriangulation->points[nNode+1].x=-10.0;
	theTriangulation->points[nNode+1].y= 10.0;
	theTriangulation->points[nNode+1].indice=nNode+1;
	
	// init le triangle[0]	
	theTriangulation->elem[0].indice= 0;
	theTriangulation->elem[0].sommet0= &theTriangulation->points[0];     // pointe vers p0;
	theTriangulation->elem[0].sommet1= &theTriangulation->points[nNode];
	theTriangulation->elem[0].sommet2= &theTriangulation->points[nNode+1];
	theTriangulation->elem[0].edge0 = &theTriangulation->edges[0];
	theTriangulation->elem[0].edge1 = &theTriangulation->edges[1];
	theTriangulation->elem[0].edge2 = &theTriangulation->edges[2]; 

    // initialiser les 3 first edges:     
    theTriangulation->edges[0].indice=0;//va de p0 à p-1
    theTriangulation->edges[0].elem0=&theTriangulation->elem[0];
    theTriangulation->edges[0].elem1=NULL;
    theTriangulation->edges[0].P0=&theTriangulation->points[0];
    theTriangulation->edges[0].P1=&theTriangulation->points[nNode];

    theTriangulation->edges[1].indice=1;// va de p-1 à p-2
    theTriangulation->edges[1].elem0=&theTriangulation->elem[0];
    theTriangulation->edges[1].elem1=NULL;
    theTriangulation->edges[1].P0=&theTriangulation->points[nNode];
    theTriangulation->edges[1].P1=&theTriangulation->points[nNode+1];


    theTriangulation->edges[2].indice=2;// va de p-2 à 0
    theTriangulation->edges[2].elem0=&theTriangulation->elem[0];
    theTriangulation->edges[2].elem1=NULL;
    theTriangulation->edges[2].P0=&theTriangulation->points[nNode+1];
    theTriangulation->edges[2].P1=&theTriangulation->points[0];
    
    
    theTriangulation->nEdge=3;
    
       // tree initialization
     myLeaf *triangleZero = malloc(sizeof(myLeaf*));
     triangleZero->theTriangle = theTriangulation->elem;
     triangleZero->theChildren = NULL;
     triangleZero->nChildren = 0;
     theTriangulation->theTree = malloc(sizeof(myTree*));
     theTriangulation->theTree->theRoot = triangleZero;
    
    
    fclose(file);

     
    /*theTriangulation->theTree->theRoot->theTriangle = theTriangulation->elem; A ENLEVER JE CROIS
    theTriangulation->theTree->theRoot->theChildren = NULL;*/ // [[[[[[[[[[[[[[[[[SEGMENTATION FAULT, il faut malloc ]]]]]]]]]]]]]]]]]
    
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



void TriangulationFree(Triangulation *theTriangulation)
{
	// TODO PA: je te laisse free ton Tree ;-)
    free(theTriangulation->edges);
    free(theTriangulation->points);
    free(theTriangulation->elem);
    free(theTriangulation);
}


void TriangulationWrite(const char *ResultName,Triangulation *theTriangulation)
{
    int i;
    
    FILE* file = fopen(ResultName,"w");
    
    fprintf(file, "Number of nodes %d \n", theTriangulation->nNode);
    for (i = 0; i < theTriangulation->nNode+2; i++) {
        fprintf(file,"%6d : %14.7e %14.7e \n",i,theTriangulation->points[i].x,theTriangulation->points[i].y); }
    
  	     fprintf(file, "Number of triangles %d \n", theTriangulation->nElem);  
        for (i = 0; i < theTriangulation->nElem; i++) {
            //elem = &(theMesh->elem[i*3]);
            fprintf(file,"%6d : %6d %6d %6d \n", i,theTriangulation->elem[i].sommet0->indice,
            				           theTriangulation->elem[i].sommet1->indice,
            				           theTriangulation->elem[i].sommet2->indice);}
     fprintf(file, "Nulber of edges %d \n", theTriangulation->nEdge);
     	for(i=0; i < theTriangulation->nEdge; i++){
     		fprintf(file,"%6d : %6d %6d \n",i,theTriangulation->edges[i].P0->indice,
     						  theTriangulation->edges[i].P1->indice);}
     	
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


