#ifndef _SWAPOPS_H_
#define _SWAPOPS_H_

/*
 * =====================================================================================
 *
 *       Filename:  swapOps.h
 *
 *    Description:  header file for swap file
 *
 *        Version:  1.0
 *        Created:  03/27/2013 05:53:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <types.h>
#include <addrspace.h>

struct swapEntry {
    paddr_t addr;
    int offset;
    struct addrspace* belongToAddrsapce;
};

/*
struct swapTransaction{
    int segmentType;
    int segmentOffset;
    struct addrspace * targetAddressSpace;
};

*/
#endif