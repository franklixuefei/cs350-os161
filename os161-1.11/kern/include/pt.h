/*
 * =====================================================================================
 *
 *       Filename:  pt.h
 *
 *    Description:  page table header
 *    
 *        Version:  1.0
 *        Created:  03/24/2013 03:37:50 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *   Organization:  
 *
 * =====================================================================================
 */

#define PTE_RDONLY      0
#define PTE_WRONLY      1

#define PT_CODE      0
#define PT_DATA      1
#define PT_STACK      2


struct Pte {
    int32_t flag;
    int32_t frameNum;
};






