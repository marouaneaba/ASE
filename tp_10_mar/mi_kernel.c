#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hardware.h"
#include "mi_syscall.h"

/*
	Marouane Abakarim
	Mar.abakarim@gmail.com
*/

unsigned int current_process = 0;
struct tlb_entry_s tlb;

static int ppage_of_vaddr(int process,unsigned vaddr)
{
  unsigned int vpage;
    if(vaddr < ((int)virtual_memory) || vaddr > ((int)virtual_memory+VM_SIZE - 1) ) return -1;

    vpage = (vaddr>>12)&0xFFF;

    if(vpage > N/2) return -1;

    return (process * N/2 + 1)+vpage;
}

static void mmuhandler()
{
    unsigned ppage , vaddr;

    vaddr = _in(MMU_FAULT_ADDR);
    ppage = ppage_of_vaddr(current_process,vaddr);

    if(ppage == -1 )
    {
      printf("erreur de segmentation");
      exit(EXIT_FAILURE);
    }else
    {
      tlb.vpage = (vaddr>>12)&0xFFF;
      tlb.ppage = ppage;
      tlb.access_x = 1;
      tlb.access_w = 1;
      tlb.access_r = 1;
      tlb.is_active = 1;
      _out(TLB_ADD_ENTRY,*((int *)&tlb));
    }
}

static void switch_to_process0(void)
{
  current_process = 0;
  _out(MMU_CMD,MMU_RESET);
}

static void switch_to_process1(void)
{
  current_process = 1;
  _out(MMU_CMD,MMU_RESET);
}

void irq_empty(){
  return;
}

void init()
{
  int i = 0;
  if(init_hardware("hardware.ini") == 0)
  {
    printf("erreur initialisation hardware\n");
    exit(EXIT_FAILURE);
  }

  for(i=0;i<16;i++)
  {
    IRQVECTOR[i] = irq_empty;
  }
  IRQVECTOR[MMU_IRQ] = mmuhandler;
  IRQVECTOR[16] = switch_to_process0;
  IRQVECTOR[17] = switch_to_process1;

}
