#

#include <stdio.h>
#include <string.h>
#include <string>
#include <capstone/capstone.h>
#include "loader.h"


int dont_print_opcode=0;
int disasm(Binary *bin);

int main(int argc,char* argv[])
{
	Binary bin;
	std::string fname;

	if(argc <2)
	{
		printf("usage: %s <binaryfileaddr>\n",argv[0]);
		exit(1);
	}
	if(argc==3 && strcmp(argv[2],"-nopc")==0)
		dont_print_opcode=1;
	fname.assign(argv[1]);
	if(load_binary(fname,&bin,Binary::BIN_TYPE_AUTO)<0)
	{
		return 1;
	}
	if(disasm(&bin)<0)
	{
		return 1;
	}
	unload_binary(&bin);
	return 0;

}

int disasm(Binary* bin)
{
	csh dis;
	cs_insn* insns;
	Section* text;
	size_t	n;

	text=bin->get_text_section();
	if(!text)
	{
		fprintf(stderr,"nothing in text section!\n");
		return 0;
	}
	if(cs_open(CS_ARCH_X86,CS_MODE_64,&dis)!=CS_ERR_OK)
	{
		fprintf(stderr,"Faild to init Capstone!\n");
		return -1;
	}
	n=cs_disasm(dis,text->bytes,text->size,text->vma,0,&insns);
	if(n<=0)
	{
		fprintf(stderr,"Disassembly erorr:%s",cs_strerror(cs_errno(dis)));
		return -1;
	}
	if(dont_print_opcode==0)
	for(size_t i=0;i<n;i++)
	{
		printf("0x%08jx: ",insns[i].address);
		for(size_t j=0;j<16;j++)
		{
			if(j<insns[i].size)
				printf("%02x ",insns[i].bytes[j]);
			else
				printf("  ");
		}
		printf("%-12s %s\n",insns[i].mnemonic,insns[i].op_str);
	}
	else
	for(size_t i=0;i<n;i++)
        {
                printf("0x%08jx:  ",insns[i].address);
                printf("%-12s %s\n",insns[i].mnemonic,insns[i].op_str);
        }

	cs_free(insns,n);
	cs_close(&dis);
}
