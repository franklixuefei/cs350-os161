/*
 * =====================================================================================
 *
 *       Filename:  swapOps.c
 *
 *    Description:  setup and operations of swap file
 *
 *        Version:  1.0
 *        Created:  03/27/2013 05:51:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <swapfile.h>
#include <machine/tlb.h>
#include <synch.h>
#include <types.h>
#include <pt.h>
#include <kern/errno.h>
#include <synch.h>
#include <thread.h>
#include <uio.h>
#include <vnode.h>
#include <coremap.h>
#include <kern/unistd.h>
#include <addrspace.h>
#include <lib.h>
#include <vfs.h>
#include <uw-vmstats.h>



#define SWAP_ENTRY_COUNT (2304)
#define SWAP_FILE_SIZE (PAGE_SIZE*SWAP_ENTRY_COUNT)
#define SWAP_FILE_NAME ("SWAPFILE")
#define SWAP_KERNEL_TEMP_LOC (0x80008000)

struct lock *swapFileLock = NULL;
struct vnode *swapFile = NULL;

int lastOffset = 0;



struct lock *swapFileLookupLock = NULL;
struct swapEntry *swapLookupTable = NULL;


int
initSwapOps ()
{
    if (!swapFileLock) swapFileLock = lock_create("swap lock");
    swapFileLookupLock = lock_create("swap look up lock");
    char *swap_file_name = kstrdup(SWAP_FILE_NAME);
    int returnErr;

    returnErr = vfs_open(swap_file_name, O_WRONLY|O_CREAT|O_TRUNC, &swapFile);


    kfree(swap_file_name);
    swapLookupTable = kmalloc(sizeof(struct swapEntry)*SWAP_ENTRY_COUNT);
    if (swapFileLock == NULL || returnErr != 0 ||swapFile == NULL || swapLookupTable == NULL) {
        return ENOMEM;
    }
    int i =0;
    for (i = 0; i < SWAP_ENTRY_COUNT; i++) {
        swapLookupTable[i].addr = 0;
        swapLookupTable[i].offset = 0;
        swapLookupTable[i].belongToAddrsapce = NULL;
    }
    return 0;
}



void
shutdownSwapOps ()
{
    kfree(swapLookupTable);
    vfs_close(swapFile); // TODO vfs_remove();
    lock_destroy(swapFileLookupLock);
    lock_destroy(swapFileLock);
}



//TODO: UPDATE CORE MAP AND ADDRESS SPACE
//int
//swapMemory (paddr_t fromMemoryLoc, paddr_t toSwapFileLoc){

//}


int
lookup (paddr_t address, int* offset)
{
    lock_acquire(swapFileLookupLock);
    int i =0;
    for (i = 0; i < SWAP_ENTRY_COUNT; i++) {
        if (swapLookupTable[i].addr == address) {
            *offset = i;
            lock_release(swapFileLookupLock);
            return 0;
        }
    }
    lock_release(swapFileLookupLock);
    return -1;
}




// swap from file to memory
int
swapIn (vaddr_t targetAddr, struct addrspace* targetAddrs)
{
    assert(targetAddrs != NULL);
    assert(targetAddr != 0);

    u_int32_t ehi, elo;
    struct uio copyUIO; 
    int inIns =0, result;
    /*
    struct addrspace * targetAddrs = addrSpace;
     switch (segType){
        case PT_DATA:
            inIns = 1;
            targetAddr = targetAddrs->pt_data[segOffset]->frameNum;
            break;
        case PT_STACK:
            targetAddr = targetAddrs->pt_stack[segOffset]->frameNum;
            break;
        case PT_CODE:
            targetAddr = targetAddrs->pt_code[segOffset]->frameNum;
            break;
        default:
            return EFAULT;
    }
     * */


    lock_acquire(swapFileLock);
    lock_acquire(swapFileLookupLock);
    int foundTatgetIndex = -1;
    int i = 0;
    for (i = 0; i < SWAP_ENTRY_COUNT ; i++) {
        if (swapLookupTable[i].belongToAddrsapce == targetAddrs && swapLookupTable[i].addr == targetAddr) {
            foundTatgetIndex = i;
            break;
        }
    }
    if (foundTatgetIndex < 0) {
        lock_release(swapFileLookupLock);
        lock_release(swapFileLock);
        //return EFAULT;
        panic("entry not found in SWAPFILE\n");
    }



    int segNum = -1;
    int errRes = calculate_segment(targetAddrs, targetAddr, &segNum);
    if (errRes) {
        return errRes;
    }
    if (segNum == PT_CODE) {
        inIns = 1;
    }
    
    int swap_res, swap_j;
    /* updating TLB */
    paddr_t swap_paddr;
    swap_res = TLB_Probe((u_int32_t)targetAddr, 0);
    assert(swap_res < 0);
    //if (swap_res < 0) {
        //TODO: may need to added status info 
        swap_j = tlb_get_rr_victim();
        lock_release(swapFileLookupLock);
        lock_release(swapFileLock);
        swap_paddr = getppages(1, targetAddr);
        lock_acquire(swapFileLock);
        lock_acquire(swapFileLookupLock);
        vmstats_inc(VMSTAT_TLB_FAULT);
         TLB_Read(&ehi, &elo, swap_j);
       
        if (elo & TLBLO_VALID) {
            vmstats_inc(VMSTAT_TLB_FAULT_REPLACE);
        } else {   
            vmstats_inc(VMSTAT_TLB_FAULT_FREE);
        }

        TLB_Write(targetAddr, swap_paddr | TLBLO_VALID | TLBLO_DIRTY, swap_j);
    //}



    mk_kuio(&copyUIO, (void*)targetAddr, PAGE_SIZE, foundTatgetIndex*PAGE_SIZE, UIO_READ);
    copyUIO.uio_space = targetAddrs;
    copyUIO.uio_segflg = (inIns) ? UIO_USERISPACE : UIO_USERSPACE;


    //kprintf ("[%s]\t%s\t:\t%d\tvalue: %d\n", __FILE__ , __PRETTY_FUNCTION__, __LINE__, 0);
   // kprintf ("%p\t%d\t%p\n", targetAddr, foundTatgetIndex, targetAddrs);
    result = VOP_READ(swapFile, &copyUIO);

    errRes = enableReadForPte(targetAddr, targetAddrs, swap_paddr);

    if (segNum == PT_CODE) {
        TLB_Write(targetAddr, swap_paddr | TLBLO_VALID, swap_j);
    }

    if (errRes) {
        lock_release(swapFileLookupLock);
        lock_release(swapFileLock);
        return errRes;
    }


    lock_release(swapFileLookupLock);
    lock_release(swapFileLock);

    coremap_insert(swap_paddr,targetAddr, targetAddrs);
  //  dumpCoreMap();
  //  dumpTLB();
    return 0;
}



// swap from memory to file
//swapOut (struct swapTransaction * swapWriteTarget)
int
swapOut (vaddr_t targetAddr, struct addrspace* addrSpace)
{
    vmstats_inc(VMSTAT_SWAP_FILE_WRITE);
    assert(addrSpace != NULL);
    assert(targetAddr != 0);
    struct uio copyUIO; 
    int inIns = 0, result;
//    vaddr_t targetAddr = 0;
    struct addrspace * targetAddrs = (struct addrspace *)addrSpace;
//    int segnmentType = segType;
/*
    switch (segType){
        case PT_DATA:
            inIns = 1;
            targetAddr = targetAddrs->pt_data[segOffset]->frameNum;
            break;
        case PT_STACK:
            targetAddr = targetAddrs->pt_stack[segOffset]->frameNum;
            break;
        case PT_CODE:
            targetAddr = targetAddrs->pt_code[segOffset]->frameNum;
            break;
        default:
            return EFAULT;
    }
 * */
    int segNum = -1;
    int errRes = calculate_segment(addrSpace, targetAddr, &segNum);
    if (errRes) {
        return errRes;
    }
    if (segNum == PT_CODE) {
        inIns = 1;
    }
    lock_acquire(swapFileLock);
    lock_acquire(swapFileLookupLock);


    int roataion = 0;
    int i = 0;
    for (i = 0; i < SWAP_ENTRY_COUNT; i++) {
        if (swapLookupTable[i].addr == targetAddr) {
            lastOffset = i;
        }else if (swapLookupTable[i].addr == 0) {
            lastOffset = i;
        }

    }
#if abc
    while (swapLookupTable[lastOffset].addr != 0){
        lastOffset ++ ;
        /*
        if ((lastOffset - SWAP_ENTRY_COUNT) > 0) {
            roataion ++;
            if (roataion > 1) {
                lock_release(swapFileLookupLock);
                lock_release(swapFileLock);
                return ENOMEM;
            }
        }
        */
        lastOffset = lastOffset % SWAP_ENTRY_COUNT;
    }
#endif
    swapLookupTable[lastOffset].addr = targetAddr;
    swapLookupTable[lastOffset].offset = lastOffset;
    swapLookupTable[lastOffset].belongToAddrsapce = addrSpace;


    mk_kuio(&copyUIO, (void*)targetAddr, PAGE_SIZE, lastOffset*PAGE_SIZE, UIO_WRITE);
    copyUIO.uio_space = targetAddrs;
    copyUIO.uio_segflg = (inIns) ? UIO_USERISPACE : UIO_USERSPACE;
    //kprintf ("[%s]\t%s\t:\t%d\tvalue: %d\n", __FILE__ , __PRETTY_FUNCTION__, __LINE__, 0);
    //kprintf ("%p\t%d\t%p\n", targetAddr, lastOffset, addrSpace);

    result = VOP_WRITE(swapFile, &copyUIO);

    lastOffset++;
    errRes = disableReadForPte (targetAddr, targetAddrs, 0);


    if (errRes) { 
        lock_release(swapFileLookupLock);
        lock_release(swapFileLock);
        return errRes;
    }
 
    lock_release(swapFileLookupLock);
    lock_release(swapFileLock);
//    dumpTLB();
    return 0;
}

void
swapTableFree(struct addrspace* addrs)
{
   int i = 0;
   for (i = 0; i < SWAP_ENTRY_COUNT; i++) {
       if (swapLookupTable[i].belongToAddrsapce == addrs) {
           swapLookupTable[i].addr = 0;
           swapLookupTable[i].offset = -1;
           swapLookupTable[i].belongToAddrsapce = NULL;
       }
   }
}

