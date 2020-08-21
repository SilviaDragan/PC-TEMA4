#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "include/project.h"

int *arrayProduct(int *array, int arrayLength)
{
    int i, n=arrayLength, p=1;
    //aloc memorie pentru un nou vector
    int *v=malloc(n*sizeof(int));
    //calculez in  p produsul tuturor elementelor din vectorul init
    for(i=0; i<n; i++)
    {
        v[i]=1;
        p=p*array[i];
    }
    //pentru fiecare element din noul vector, impart p la elementul
    //corespunzator din vectorul initial 
    for(i=0; i<n; i++)
    {
        if(array[i]!=0)
            v[i]=p/array[i];
    }

    return v;
}

int **rotateMatrix(int **matrix, int matrixSize)
{
    int nl=matrixSize;
    int i, j;
    //aloc memorie pt o noua matrice
    int **m=malloc(nl*sizeof(int*));
    for(i=0; i<nl; i++)
    {
        m[i]=calloc(nl, sizeof(int));
    }

    //rotesc matricea element cu element
    for(i=0; i<nl; i++)
        for(j=0; j<nl; j++)
        {
            m[i][j]=matrix[j][nl-i-1];
        }

    return m;
}

int *subMatrixesSums(int **matrix, int queriesNo, int *queries)
{
    int i, j, k;
    int x1, y1, x2, y2;
    //aloc memorie pentru un vector de queries
    int *v=calloc(queriesNo, sizeof(int));
    //repet instructiunile pentru fiecare set 
    for(k=0; k<queriesNo; k++)
    {
        x1=queries[4*k];
        y1=queries[4*k+1];
        x2=queries[4*k+2];
        y2=queries[4*k+3];

    //pe pozitia k in vectorul v se va afla suma din submatrice data de x12, y12
        for(i=x1; i<=x2; i++)
            for(j=y1; j<=y2; j++)
                {
                    v[k]=v[k]+matrix[i][j];
                }

    }

    return v;
}

TDriver *allocDriver(int ridesNo)
{
    //aloc memorie pentru un singur sofer de tip TDriver *
    TDriver *silvia=(TDriver *)calloc(1, sizeof(TDriver));
    //aloc memorie pentru vectorul de curse
    silvia->curse=(TRide *)calloc(ridesNo, sizeof(TRide));
    return silvia;
}

TDriver **allocDrivers(int driversNo, int *driversRidesNo)
{
    int i;
    //aloc memorie pentru un vector de soferi
    TDriver **vectordrivers=(TDriver **)malloc(driversNo*sizeof(TDriver *));
    //aloc memorie pentru fiecare sofer folosind fct precendenta
    for(i=0; i<driversNo; i++)
    {
        vectordrivers[i]=allocDriver(driversRidesNo[i]);
    }
    return vectordrivers;
}

TDriver **readDrivers(FILE *inputFile, int *driversNo)
{
    int numarsof;
    //citesc din fisier numarul da soferi
    fread(&numarsof, sizeof(int), 1, inputFile);
    *driversNo=numarsof;
    //aloc memorie si citesc din fisier numarul de curse pt fiecare sofer
    int *nrcurse=(int *)calloc(numarsof, sizeof(int));
    fread(nrcurse, sizeof(int), numarsof, inputFile);

    TDriver **vectordrivers=allocDrivers(numarsof, nrcurse);
    int i;
    int j;
    //fac citirile pentru fiecare sofer
    for(i=0; i<numarsof; i++)
    {
        //citesc numele, numarul si locatia
        fread(vectordrivers[i]->nume, sizeof(char), 20, inputFile);
        fread(vectordrivers[i]->platenr, sizeof(char), 8, inputFile);
        fread(&vectordrivers[i]->x, sizeof(double), 1, inputFile);
        fread(&vectordrivers[i]->y, sizeof(double), 1, inputFile);
        
        vectordrivers[i]->nrcurse=nrcurse[i];
        //citesc informatiile curselor soferului
        for(j=0; j<nrcurse[i]; j++)
        {
            fread(&vectordrivers[i]->curse[j].x, sizeof(double), 1, inputFile);
            fread(&vectordrivers[i]->curse[j].y, sizeof(double), 1, inputFile);
            fread(&vectordrivers[i]->curse[j].state,sizeof(char),1, inputFile);

        }

    }
    free(nrcurse);
    return vectordrivers;
}

void printDrivers(FILE *outputFile, TDriver **drivers, int driversNo)
{
    
    int i;
    unsigned int j;
    //afisez pentru fiecare sofer
    for(i=0; i<driversNo; i++)
    { 
        //afisez informatiile soferului
        fwrite(drivers[i]->nume, 20, 1, outputFile);
        fwrite(drivers[i]->platenr, 8, 1, outputFile);
        fwrite(&drivers[i]->x, sizeof(double), 1, outputFile);
        fwrite(&drivers[i]->y, sizeof(double), 1, outputFile);
        //afisez informatiile curselor fiecarui sofer
        for(j=0; j<drivers[i]->nrcurse; j++)
        {
            fwrite(&drivers[i]->curse[j].x, sizeof(double), 1, outputFile);
            fwrite(&drivers[i]->curse[j].y, sizeof(double), 1, outputFile);
            fputc(drivers[i]->curse[j].state, outputFile);
        }

    }
}

char *maxRatingDriverName(TDriver **drivers, int driversNo)
{
    int i, best_driver=0;
    double max_rating=0, current_rating=0, nrstele=0, nrc=0;
    unsigned int j;
    //calculez ratingul fiecarui sofer si verific daca acesta e maxim
    for(i=0; i<driversNo; i++)
    {
        nrc=drivers[i]->nrcurse;
        //calculez numarul de stele primite ale soferului
        for(j=0; j<drivers[i]->nrcurse; j++)
        {
            if(drivers[i]->curse[j].state>=0 && drivers[i]->curse[j].state<=5)
                nrstele=nrstele + (drivers[i]->curse[j].state);
            /*daca exista alta informatie inafara de numarul de stele 
            intre 0-5, atunci cursa este anulata sau in desfasurare, deci este 
            scazuta din numarul curselor valide*/
            else 
                {
                    nrc--;
                }
        }
            if(nrc!=0) 
            {
                //calculez ratingul si determin maximul si soferul 
                //cu rating maxim
                current_rating=nrstele/nrc;
                if(current_rating>max_rating) 
                { 
                    max_rating=current_rating;
                    best_driver=i;
                }
            }
            current_rating=0;
            nrc=0;
            nrstele=0;

    }
   return drivers[best_driver]->nume;
}

TDriver **getClosestDrivers(TDriver **drivers, int driversNo, double desiredX,
    double desiredY, int resultsNo)
{
    int i, j, aux;
    double auxd;
    //aloc memorie pentru vectorul de distante
    double *distances=(double *)calloc(driversNo, sizeof(double));
    //calculez distanta de la locatia curenta pana la locati dorita 
    //pentru fiecare sofer
    for(i=0; i<driversNo; i++)
    {
        distances[i]=sqrt( (desiredX-drivers[i]->x)*(desiredX-drivers[i]->x) + 
            (desiredY-drivers[i]->y)*(desiredY-drivers[i]->y) );
    }
    /*aloc memorie pentru un vector de indici  ce va fi folosit pentru a sorta
    vectorul de distante si il initializez*/
    int *indici=(int *)calloc(driversNo, sizeof(int));
    for(i=0; i<driversNo; i++)
    {
        indici[i]=i;
    }
    //fac sortarea crescatoare a distantelor folosind vectorul de indici
    for(i=0; i<driversNo-1; i++)
        for(j=i+1; j<driversNo; j++)
        {
            if(distances[i]>distances[j])
            {
                aux=indici[i];
                indici[i]=indici[j];
                indici[j]=aux;

                auxd=distances[i];
                distances[i]=distances[j];
                distances[j]=auxd;
            }
            else if(distances[i]==distances[j])
            {
                if (strcmp(drivers[indici[i]]->nume, drivers[indici[j]]->nume) >0 )
                {
                    aux=indici[i];
                    indici[i]=indici[j];
                    indici[j]=aux;
                }
            }
        }
    /*aloc memorie pentru un nou vector ce va contine numarul de soferi cerut
    in ordinea crescatoare a distantelor*/

    TDriver **closest_drivers=(TDriver **)calloc(resultsNo, sizeof(TDriver *));
    for(i=0; i<resultsNo; i++)
    {
        closest_drivers[i]=drivers[indici[i]];
    }
    free(distances);
    free(indici);
    return closest_drivers;
}

void freeDriver(TDriver *driver)
{
    free(driver->curse);
    free(driver);
    return;
}

void freeDrivers(TDriver **drivers, int driversNo)
{
    int i;
    for(i=0; i<driversNo; i++)
    {
        freeDriver(drivers[i]);
    }
    free(drivers);
    return;
}
