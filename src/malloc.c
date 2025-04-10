#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define ALIGN4(s)         (((((s) - 1) >> 2) << 2) + 4)
#define BLOCK_DATA(b)     ((b) + 1)
#define BLOCK_HEADER(ptr) ((struct _block *)(ptr) - 1)

static int atexit_registered = 0;
static int num_mallocs       = 0;
static int num_frees         = 0;
static int num_reuses        = 0;
static int num_grows         = 0;
static int num_splits        = 0;
static int num_coalesces     = 0;
static int num_blocks        = 0;
static int num_requested     = 0;
static int max_heap          = 0;



struct _block 
{
   size_t  size;         /* Size of the allocated _block of memory in bytes */
   struct _block *next;  /* Pointer to the next _block of allocated memory  */
   bool   free;          /* Is this _block free?                            */
   char   padding[3];    /* Padding: IENTRTMzMjAgU3jMDEED                   */
};


struct _block *heapList = NULL; /* Free list to track the _blocks available */
struct _block *lastAlloc = NULL; /*list to track the previously allocated block for next fit*/

/*
 *  \brief printStatistics
 *
 *  \param none
 *
 *  Prints the heap statistics upon process exit.  Registered
 *  via atexit()
 *
 *  \return none
 */
void printStatistics( void )
{
   //loop to find amount of blocks at end of program
   struct _block *blockNum = heapList;
   while(blockNum)
   {
      num_blocks++;
      blockNum = blockNum->next;
   }
  printf("\nheap management statistics\n");
  printf("mallocs:\t%d\n", num_mallocs );
  printf("frees:\t\t%d\n", num_frees );
  printf("reuses:\t\t%d\n", num_reuses );
  printf("grows:\t\t%d\n", num_grows );
  printf("splits:\t\t%d\n", num_splits );
  printf("coalesces:\t%d\n", num_coalesces );
  printf("blocks:\t\t%d\n", num_blocks );
  printf("requested:\t%d\n", num_requested );
  printf("max heap:\t%d\n", max_heap );
}
/*
 * \brief findFreeBlock
 *
 * \param last pointer to the linked list of free _blocks
 * \param size size of the _block needed in bytes 
 *
 * \return a _block that fits the request or NULL if no free _block matches
 *
 */
struct _block *findFreeBlock(struct _block **last, size_t size) 
{
   struct _block *curr = heapList;

#if defined FIT && FIT == 0
   /* First fit */
   //
   // While we haven't run off the end of the linked list and
   // while the current node we point to isn't free or isn't big enough
   // then continue to iterate over the list.  This loop ends either
   // with curr pointing to NULL, meaning we've run to the end of the list
   // without finding a node or it ends pointing to a free node that has enough
   // space for the request.
   // 
   while (curr && !(curr->free && curr->size >= size)) 
   {
      *last = curr;
      curr  = curr->next;
   }
#endif

#if defined BEST && BEST == 0
   long long best = INT_MAX;
   struct _block *bestfit = NULL;
   long long difference = 0;
   while(curr)
   {
      if(curr && curr->free && curr->size >= size)
      {
         difference = curr->size - size;
         if(best >= difference)
         {
            best = difference;
            bestfit = curr;
         }
      }
      *last = curr;
      curr = curr->next;
   }
   curr = bestfit;
#endif

#if defined WORST && WORST == 0
   long long largest = 0;
   struct _block *worst = NULL;
   while (curr) 
   {
      if(curr && curr->free && largest <= curr->size && curr->size >= size)
      {
         largest = curr->size;
         worst = curr;
      }
      *last = curr;
      curr  = curr->next;
   }
   curr = worst;
#endif

#if defined NEXT && NEXT == 0
   if(lastAlloc)
   {
      curr = lastAlloc;
   }
   while(curr && !(curr->free && curr->size >= size))
   {
      *last = curr;
      curr = curr->next;
   }
   //if there was a previous postion but curr is null because it did not find anything, retry list from the start by making curr=heapList;
   if(lastAlloc && !curr)
   {
      curr = heapList;
      while(curr && !(curr->free && curr->size >= size))
      {
         *last = curr;
         curr = curr->next;
      }
   }
   lastAlloc = curr;
#endif

   return curr;
}

/*
 * \brief growheap
 *
 * Given a requested size of memory, use sbrk() to dynamically 
 * increase the data segment of the calling process.  Updates
 * the free list with the newly allocated memory.
 *
 * \param last tail of the free _block list
 * \param size size in bytes to request from the OS
 *
 * \return returns the newly allocated _block of NULL if failed
 */
struct _block *growHeap(struct _block *last, size_t size) 
{
   /* Request more space from OS */
   struct _block *curr = (struct _block *)sbrk(0);
   struct _block *prev = (struct _block *)sbrk(sizeof(struct _block) + size);

   assert(curr == prev);

   /* OS allocation failed */
   if (curr == (struct _block *)-1) 
   {
      return NULL;
   }

   /* Update heapList if not set */
   if (heapList == NULL) 
   {
      heapList = curr;
   }

   /* Attach new _block to previous _block */
   if (last) 
   {
      last->next = curr;
   }
   num_grows++;
   /* Update _block metadata:
      Set the size of the new block and initialize the new block to "free".
      Set its next pointer to NULL since it's now the tail of the linked list.
   */
   curr->size = size;
   curr->next = NULL;
   curr->free = false;
   return curr;
}

/*
 * \brief malloc
 *
 * finds a free _block of heap memory for the calling process.
 * if there is no free _block that satisfies the request then grows the 
 * heap and returns a new _block
 *
 * \param size size of the requested memory in bytes
 *
 * \return returns the requested memory allocation to the calling process 
 * or NULL if failed
 */
void *malloc(size_t size) 
{
   num_mallocs++;
   if( atexit_registered == 0 )
   {
      atexit_registered = 1;
      atexit( printStatistics );
   }

   /* Align to multiple of 4 */
   size = ALIGN4(size);

   /* Handle 0 size */
   if (size == 0) 
   {
      return NULL;
   }
   num_requested += size;
   if(size >= max_heap)
   {
      max_heap = size;
   }
   /* Look for free _block.  If a free block isn't found then we need to grow our heap. */

   struct _block *last = heapList;
   struct _block *next = findFreeBlock(&last, size);

   /* Could not find free _block, so grow heap */
   if (next == NULL) 
   {
      next = growHeap(last, size);
   }
   else
   {
      num_reuses++;
   }
   /* Could not find free _block or grow heap, so just return NULL */
   if (next == NULL) 
   {
      return NULL;
   }
   /* If the block found by findFreeBlock is larger than we need then:
            If the leftover space in the new block is greater than the sizeof(_block)+4 then
            split the block.
            If the leftover space in the new block is less than the sizeof(_block)+4 then
            don't split the block.
   */
   if(next && next->size > size)
   {
      if(next->next && (next->size - size) > (sizeof(struct _block) + 4))
      {
         struct _block *p = next;
         long long oldsize = p->size;
         struct _block *oldnext = p->next;
         p->size = size;
         p->next = (struct _block*)((long long)p + sizeof(struct _block) + size);
         p->next->next = oldnext;
         p->next->free = true;
         p->next->size = oldsize - size - sizeof(struct _block);
         //add to split count
         num_splits++;
      }
   }
   /* Mark _block as in use */
   next->free = false;

   /* Return data address associated with _block to the user */
   return BLOCK_DATA(next);
}

/*
 * \brief free
 *
 * frees the memory _block pointed to by pointer. if the _block is adjacent
 * to another _block then coalesces (combines) them
 *
 * \param ptr the heap memory to free
 *
 * \return none
 */
void free(void *ptr) 
{
   if (ptr == NULL) 
   {
      return;
   }

   /* Make _block as free */
   struct _block *curr = BLOCK_HEADER(ptr);
   assert(curr->free == 0);
   curr->free = true;

   /* Coalesce free _blocks.  If the next block or previous block 
            are free then combine them with this block being freed.
   */
   struct _block *prev = heapList;
   while(prev && prev->next != curr)
   {
      prev = prev->next;
   }
   if(prev && curr && prev->free && curr->free)
   {
      prev->size = curr->size +sizeof(struct _block) + prev->size;
      prev->next = curr->next;
      num_coalesces++;
   }
   if(curr && curr->free && curr->next != NULL && curr->next->free)
   {
      curr->size = curr->next->size + sizeof(struct _block) + curr->size;
      curr->next = curr->next->next;
      num_coalesces++;
   }
   num_frees++;
}

void *calloc( size_t nmemb, size_t size )
{
   if(nmemb == 0 || size == 0)
   {
      return NULL;
   }
   else
   {
      struct _block *cal = malloc(nmemb * size);
      if(cal)
      {
         memset(cal, 0, nmemb*size);
      }
      return cal;
   }
}

void *realloc( void *ptr, size_t size )
{
   struct _block *new_ptr = malloc(size);
   if(new_ptr && ptr)
   {
      memcpy(new_ptr, ptr, size);
      free(ptr);
      return new_ptr;
   }
   //if size is equal to zero, and ptr is not NULL, then the call is equivalent to free(ptr)
   else if(ptr && size == 0)
   {
      free(ptr);
      return NULL;
   }
   else
   {
      return NULL;
   }
}




/* vim: IENTRTMzMjAgU3ByaW5nIDIwMjM= -----------------------------------------*/
/* vim: set expandtab sts=3 sw=3 ts=6 ft=cpp: --------------------------------*/