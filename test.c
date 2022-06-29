#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Box
{
	size_t offset;
	size_t len_key;
	size_t offset_next;
};
int main()
{
	char name[128];
	scanf("%s", name);
	FILE *file=fopen(name,"rb+");
	if (!file)
	{
		file=fopen(name,"wb+");
		if (!file) 
		{
			return 1;
		}
		size_t offset=0;
		struct Box box;
		char info[128];
		scanf("%s",info);
		box.len_key=strlen(info)+1;
		fwrite(&offset,sizeof(size_t),1,file);
		box.offset=ftell(file);
		fwrite(info,sizeof(char),box.len_key,file);
		box.offset_next=ftell(file);
		fwrite(&box,sizeof(struct Box),1,file);

		scanf("%s",info);
		box.len_key=strlen(info)+1;
		box.offset=ftell(file);
		fwrite(info,sizeof(char),box.len_key,file);
		offset=ftell(file);
		fwrite(&box,sizeof(struct Box),1,file);
		fseek(file,0,SEEK_SET);
		fwrite(&offset,sizeof(size_t),1,file);

		
	}
	else
	{
		char scan_info[128];
		struct Box len,len1;
		size_t offset;
		fread(&offset,sizeof(size_t),1,file);
		fseek(file,offset,SEEK_SET);
		fread(&len,sizeof(struct Box),1,file);
		fseek(file,len.offset_next,SEEK_SET);
		fread(&len1,sizeof(struct Box),1,file);
		fseek(file,len1.offset,SEEK_SET);
		fread(scan_info,sizeof(char),len1.len_key,file);
		printf("%s\n",scan_info);
		fseek(file,len.offset,SEEK_SET);
		fread(scan_info,sizeof(char),len.len_key,file);
		printf("%s\n",scan_info);
	}
	fclose(file);
	return 0;
}
