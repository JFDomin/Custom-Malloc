#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int main()
{
  clock_t start, end;
  printf("Running benchmark\n");
  start = clock();
  char * ptr = ( char * ) malloc ( 50000 );

  char * ptr_array[25000];

  int i;
  for ( i = 0; i < 25000; i++ )
  {
    if(i % 3 == 0)
    {
      ptr_array[i] = ( char * ) malloc ( 5000 ); 
    }
    else if ( i % 50 == 0)
    {
      ptr_array[i] = ( char * ) malloc ( 500 ); 
    }
    else if ( i % 79 == 0)
    {
      free(ptr_array[i]);
    }
    else
    {
      ptr_array[i] = ( char * ) malloc ( 1024 ); 
    }    
    ptr_array[i] = ptr_array[i];
  }

  free( ptr );

  for ( i = 0; i < 25000; i++ )
  {
    if( i % 2 == 0 )
    {
      free( ptr_array[i] );
      free(ptr_array[i-1]);
    }
  }

  ptr = ( char * ) malloc ( 65535 );
  free( ptr );

  char * ptr2_array[25000];
  for( i = 0; i < 25000; i++)
  {
    ptr2_array[i] = ( char * ) malloc ( 500 );
    ptr2_array[i] = ptr2_array[i];
  }

  for(i = 0; i < 25000; i++)
  {
    if(i % 17 == 0)
    {
      free(ptr2_array[i]);
    }
  }
  char * ptr3_arr[1000];
  ptr = (char *) malloc(276);
  for(i = 0; i < 1000; i++)
  {
    ptr3_arr[i] = (char *) malloc (355);
    ptr3_arr[i] = ptr3_arr[i];
  }
  free(ptr);
  end = clock();
  double seconds = (double)(end-start)/CLOCKS_PER_SEC;
  printf("ticks = %ld\n", (end-start)); 
  printf("seconds = %f\n", seconds);
  return 0;
}
