

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Maximum lengths for string fields
#define MAX_NAME_LENGTH 50
#define MAX_LASTNAME_LENGTH 50
#define MAX_BIRTHPLACE_LENGTH 20

// maximum block capacity (in number of records)
#define MAXTAB 10

typedef struct {
    int nread;
    int nwrite;
}t_nE_S;

// Improved record structure
typedef struct {
    long int id; 
    int del;                      // Unique identifier
    char firstName[MAX_NAME_LENGTH];   // First name
    char lastName[MAX_LASTNAME_LENGTH];// Last name
    char birthDate[20];                // Date in format DD/MM/YYYY
    char birthPlace[MAX_BIRTHPLACE_LENGTH]; // Birthplace
} t_rec;

// type of a data block (and therefore of buffer variables as well)
typedef struct blck {
    t_rec tab[ MAXTAB ];  // array of records inside a block;                    // logical erase indicators ('*' erased / ' ' not erased)
    int nb;                              // number of records inserted in the block
} t_block;

typedef struct header { // File header structure
    int nblk;   // Number of blocks
    int nbrec;  // Number of  total records
    int delrec ;//number  of  delted records
    int  nmissingdata;
    int loidingFactor;
     int TOF_fragmentation;
 
} header;
typedef struct t_TOF {
            FILE *f;    // C stream implementing the file
            header h;// the header in main memory
            t_nE_S o ;
        } t_TOF;
   
void open(t_TOF **F, char mode, const char *filename) {

      if(mode=='N' || mode=='n')
      {
     (*F)->f = fopen(filename, "wb+");
     if((*F)->f==NULL){
            perror( "TOF_open" );
        
     }
     (*F)->h.delrec=0;
      (*F)->h.loidingFactor=0;
      (*F)->h.nblk=0;
      (*F)->h.nbrec=0;
      (*F)->h.nmissingdata=0;
      (*F)->h.TOF_fragmentation=0;
      (*F)->o.nread=0;
      (*F)->o.nwrite=0;
   	fwrite( &((*F)->h), sizeof(header), 1, (*F)->f );


      }
      else{//the  mode  

   (*F)->f = fopen(filename, "rb+");
     if((*F)->f==NULL){
            perror( "TOF_open" );
        
     }
   
     	fread( &((*F)->h), sizeof(header), 1, (*F)->f );
   

      }
    }
          
         
        

    // Close file function
void TOF_close( t_TOF *F )
{
   // saving header part in secondary memory (at the begining of the stream F->f)
   fseek(F->f, 0, SEEK_SET);
   fwrite( &F->h, sizeof(header), 1, F->f );
   fclose(F->f);
   free( F );
}
void TOF_readBlock( t_TOF *F, long i, t_block *buf )
{
     F->o.nread++;
   fseek(F->f, (sizeof(header) +sizeof(t_nE_S)) +(i-2)*sizeof(t_block), SEEK_SET);
   fread( buf, sizeof(t_block), 1, F->f );
}
void TOF_writeBlock( t_TOF *F, long i, t_block *buf )
{
    F->o.nwrite++;
   fseek(F->f, sizeof(header) +sizeof(t_nE_S) + (i-2)*sizeof(t_block), SEEK_SET);
   fwrite( buf, sizeof(t_block), 1, F->f );
}

// Set header in file
void setheader(t_TOF F, int i, int v) {
    switch (i)
    {
    case 1:
        F.h.delrec=v;
        break;
     case 2:
        F.h.loidingFactor=v;
        break;
           case 3:
        F.h.nblk=v;
        break;
           case 4:
        F.h.nbrec=v;
        break;
            case 5:
        F.h.nmissingdata=v;
        break;
             case 6:
        F.h.TOF_fragmentation=v;
        break;
    
    default:
    printf("invalide inputs ");
        break;
    }
  
}
// Get header value from file
int getheader(t_TOF F, int i) {
      switch (i)
    {
    case 1:
        return F.h.delrec;
        break;
     case 2:
         return F.h.loidingFactor;
        break;
           case 3:
          return F.h.nblk;
        break;
           case 4:
           return F.h.nbrec;
        break;
            case 5:
          return F.h.nmissingdata;
        break;
    default:
    printf("invalide inputs ");
        break;
    }
  
   
}

