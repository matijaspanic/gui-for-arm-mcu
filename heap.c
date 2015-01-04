#include <math.h>

#define HEAP_SIZE 24576
#define HEAP_BLOCK_SIZE 16
#define HEAP_BLOCK_COUNT (HEAP_SIZE/HEAP_BLOCK_SIZE)

unsigned char heap_use[HEAP_BLOCK_COUNT/8];
char heap[HEAP_SIZE];

unsigned int used_bytes = 0;

void heap_init () {
  int i;
  for (i = 0; i < HEAP_BLOCK_COUNT/8; i++) {
    heap[i] = 0;
  }
}

unsigned int find_empty (unsigned int block_count) {
  unsigned int byte_index, bit_index, free_block_count, start_byte_indeks, start_bit_index;
  
  free_block_count = 0;
  
  for (byte_index = 0; byte_index < HEAP_BLOCK_COUNT/8; byte_index++) {
    if (heap_use[byte_index] == 0xff)
      continue;
      
    for (bit_index = 0; bit_index < 8; bit_index++) {
      if ((heap_use[byte_index] & (0x80>>bit_index))==0) {
        if (free_block_count==0) {
          start_byte_indeks = byte_index;
          start_bit_index = bit_index;
        }
        
        free_block_count++;
        
        if (free_block_count >= block_count) {
          return start_byte_indeks*8 + start_bit_index;
        }
      }
      else {
        free_block_count = 0;
      }
    }
  }
  
  return 0;
}

void set_use (unsigned int start_block_index, unsigned int block_count, unsigned char use) {
  int i;
  for (i = 0; i < block_count; i++) {
    unsigned int block_index = start_block_index + i;
    if (use > 0)
      heap_use[block_index/8] |= (0x80 >> (block_index%8));
    else
      heap_use[block_index/8] &= ~(0x80 >> (block_index%8));
  }
}

void *malloc(unsigned int size) {
  unsigned int block_count = ceil((float)size/(float)HEAP_BLOCK_SIZE);
  unsigned int start_block_index = find_empty(block_count);
  
  set_use(start_block_index, block_count, 1);
  
  used_bytes += size;

  return heap + start_block_index*HEAP_BLOCK_SIZE;
}

void free(void *pointer, unsigned int size) {
  int i;
  unsigned int block_count = ceil(size/HEAP_BLOCK_SIZE);
  unsigned int start_block_index = ((unsigned int)pointer-(unsigned int)heap)/HEAP_BLOCK_SIZE;

  used_bytes -= size;

  set_use(start_block_index, block_count, 0);
}

unsigned int heap_get_allocated_bytes() {
  unsigned int byte_index, bit_index;
  unsigned int used_blocks_count = 0;
  for (byte_index = 0; byte_index < HEAP_BLOCK_COUNT/8; byte_index++) {
    for (bit_index = 0; bit_index < 8; bit_index++) {
      if ((heap_use[byte_index] & (0x80>>bit_index))>0)
        used_blocks_count++;
    }
  }
  
  return used_blocks_count*HEAP_BLOCK_SIZE;
}

unsigned int heap_get_used_bytes() {
  return used_bytes;
}

