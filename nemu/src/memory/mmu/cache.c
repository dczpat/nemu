#include "memory/mmu/cache.h"
#include "memory/memory.h"
#include "cpu/cpu.h"
#include "memory.h"
#include <stdlib.h>
#include <stdio.h>


// init the cache
void init_cache()
{
	// implement me in PA 3-1
#ifdef CACHE_ENABLED
	for(int i=0;i<1024;++i)
	{
	    Cache[i].valid = false;
	    Cache[i].tag = 0;
	}
#else
    return;
#endif
}

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine* cache)
{
	// implement me in PA 3-1
	//m:26 c:10 q:7 s:3 b:6
	uint32_t tag = (paddr >> 13) & 0x7ffff;
	uint32_t group_id = (paddr >> 6) & 0x7f;
	uint32_t block_addr = paddr & 0x3f;
	
	memcpy(hw_mem+paddr, &data, len);
	//return;
	
	uint32_t cur;
	for(int i=0;i<8;++i)
	{
	    cur = (group_id * 8) + i;
	    if(cache[cur].tag == tag && cache[cur].valid)
	    {
	        if(block_addr + len <= 64)
	        {
	            //printf("\n------121212\n");
	            //very long time to find this bug
	            //DO NOT FORGET the offset
	            memcpy(cache[cur].block+block_addr, &data, len);
	        }
	        else
	        {
	            //printf("\n------343434\n");
	            size_t len1 = 64 - block_addr;
	            size_t len2 = len - len1;
	            cache_write(paddr, len1, data, cache);
	            cache_write(paddr+len1, len2, data>>(8*len1), cache);
	        }
	        return;
	    }
	}
	//printf("565656\n");
	//memcpy(hw_mem+paddr, &data, len);
}

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len, CacheLine* cache)
{
	// implement me in PA 3-1
	//m:26 c:10 q:7 s:3 b:6
	uint32_t tag = (paddr >> 13) & 0x7ffff;
	uint32_t group_id = (paddr >> 6) & 0x7f;
	uint32_t block_addr = paddr & 0x3f;
	
	uint32_t res = 0;//,tt=0;
	uint32_t cur;
	//memcpy(&tt, hw_mem+paddr, len);
	//printf("\n---------");
	//return tt;
	//memcpy(&res, hw_mem+paddr, len);
	//printf("0 res=%d, paddr=%x, len=%d, tag=%d, group_id=%d, block_addr=%x\n", res,(unsigned)paddr, len, tag, group_id, block_addr);
	//printf("zero:res=%d\n",res);
	//return res;
	
	for(int i=0;i<8;++i)
	{
	    cur = (group_id * 8) + i;
	    if(cache[cur].tag == tag && cache[cur].valid)
	    {
	        if(block_addr + len <= 64)
	        {
	            //uint32_t tt=0;
	            memcpy(&res, cache[cur].block+block_addr, len);
	            //if(tt!=res)
	            //printf("one:res=%d, tt=%d\n",res,tt);
	            return res;
	        }
	        else //cross lines
	        {
	            uint32_t res_l = 0, res_h = 0;
	            size_t len1 = 64 - block_addr;
	            size_t len2 = len - len1;
	            //res_l = cache_read(paddr, len1, cache);
	            memcpy(&res_l, cache[cur].block+block_addr, len1);
	            res_h = cache_read(paddr + len1, len2, cache);
	            res = (res_h << (8 * len1)) | res_l;
	            //printf("!!!\n");
	            return res;
	        }
	        //printf("111 res=%d, paddr=%x, len=%d, tag=%d, group_id=%d, block_addr=%x\n", res,(unsigned)paddr, len, tag, group_id, block_addr);
	        //return res;
	    }
	}
	//memcpy(&res, hw_mem + paddr, len);
	for(int i=0;i<8;++i)
	{
	    cur = (group_id * 8) + i;
	    if(!cache[cur].valid)
	    {
	        cache[cur].valid = true;
	        cache[cur].tag = tag;
	        memcpy(cache[cur].block, hw_mem + paddr - block_addr, 64);
	        res = cache_read(paddr, len, cache);
	        //printf("two:res=%d, tt=%d\n",res,tt);
	        return res;
	    }
	}
	int r = rand() % 8;
	cur = (group_id * 8) + r;
	cache[cur].valid = true;
	cache[cur].tag = tag;
	memcpy(cache[cur].block, hw_mem + paddr - block_addr, 64);
	res = cache_read(paddr, len, cache);
	//printf("333!");// paddr=%x, len=%d, tag=%d, group_id=%d, block_addr=%x\n", (unsigned)paddr, len, tag, group_id, block_addr);
	return res;
}


