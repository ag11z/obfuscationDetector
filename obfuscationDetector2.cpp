#include<stdio.h>
#include<Windows.h>
#include<iostream>
#include <sstream> 
#include <ctype.h>
using namespace std;


int main(int argc,  char * argv[])
{
    IMAGE_NT_HEADERS peHead;
    IMAGE_DOS_HEADER dosMZ;
    IMAGE_SECTION_HEADER *secHead;
    unsigned long d;
    //create a handle to the file
    HANDLE host;
    int i=0; int unpackit=0, asked=0;
    int check =1;
    if (argc<2)
    {
        printf("\nInvalid Usage: Name of file expected in first command line parameter.\nUseage : obfuscationDetector2.exe filename.exe optional_packer_name \n"); 
        return 0;          
}

int upx =1;

string optionaldection="";
//if is a 
    if (argc>=3)
    {optionaldection=argv[2];
                 for (int i=0; i<optionaldection.length(); i++)
  optionaldection[i]=toupper(optionaldection[i]);
//cout<<optionaldection;
check =0; 
    } 
    string file=argv[1];
    Label:
// set it to the file loaded in memory 
    if((host=CreateFileA(file.c_str(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL))==INVALID_HANDLE_VALUE)
    {
        printf("\nFile OPEN Error");
        return 0;
    }
    //try reading the magic valur 
    if(!ReadFile(host,(void*)&dosMZ,sizeof(dosMZ),&d,NULL))
    {
        printf("\nRead Fail");
        return 0;
    }
    // If the file magic value is not a PE stop
    if(!(dosMZ.e_magic==IMAGE_DOS_SIGNATURE))
    {
        printf("\nNot a Valid PE");
        return 0;
    }
    
    
// move the file potiner to the begining of the file
    SetFilePointer(host,dosMZ.e_lfanew,NULL,FILE_BEGIN);
    
    if(!ReadFile(host,(void*)&peHead,sizeof(peHead),&d,NULL))
    {
        printf("\nRead Fail");
        return 0;
    }
    if(!(peHead.Signature==IMAGE_NT_SIGNATURE))
    {
        printf("\nNot Valid PE");
        return 0;
    }
//set the section header to 
    secHead=(IMAGE_SECTION_HEADER*)GlobalAlloc(GMEM_FIXED,sizeof(IMAGE_SECTION_HEADER)*peHead.FileHeader.NumberOfSections);
    ReadFile(host,(void*)secHead,sizeof(IMAGE_SECTION_HEADER)*peHead.FileHeader.NumberOfSections,&d,NULL);
    
  char section_name[101]; string temp; int nopacking=1; 
    for(i=0;i<peHead.FileHeader.NumberOfSections;i++)
    {
                                                     
   sprintf( section_name,"%100s",secHead[i].Name); 
   
  for (int i=0; i<100; i++)
  section_name[i]=toupper(section_name[i]);
 
  temp=section_name;
   size_t pos= temp.find("PETITE");
  
              if (pos!=std::string::npos)
        {
        printf("\nPetite Packing Detected!\n");
        nopacking=0;
        }
        pos= temp.find("UPX1");
        if (pos!=std::string::npos)
        {
         printf("\nUPX Packing Detected!\n");
         nopacking=0;string answer;
        cout<<"Do you want to unpack it? yes or no\n";
        cin>>answer;
        if (answer=="yes")
        unpackit=1;
        else
        if (answer!="no")
        cout<<"Assumed you ment no.\n";
        
        
         if (upx ==1 && unpackit==1)
         { upx=0; 
         
          int length; char Buffer[100];
          GetCurrentDirectory(length ,Buffer);
          
          //cout<<"gvbjh"<<Buffer;
          string newfile;
          //system();
          string copyname=file;
          size_t  pos2;
          do{
           pos2=copyname.find_first_of("/");
           if(pos2!=std::string::npos)
          copyname[pos2]='\\';
          }
          while(pos2!=std::string::npos);
          
          string name =Buffer;
          name+='\\';
          name+=copyname;
          cout<<"Copying "<<name<<" into ";
          newfile=Buffer;
          newfile+="\\unpacked.exe";
        file ="unpacked.exe";
          cout<<newfile<<"\n";
          string cmd="COPY ";
          cmd+=name;
          cmd+=" ";
          cmd+=newfile;
          system(cmd.c_str());
          cout<<"Trying to unpack it !\n";
          system("upx -d unpacked.exe");
          cout<<"\n Next iteration:";
         goto Label; 
           
           
        }
          
           
          
        }
             pos= temp.find("MPRESS1");
        if (pos!=std::string::npos)
        {
        printf("\nMpress Packing Detected!\n");
          nopacking=0;
        }
             pos= temp.find("ASPACK");
        if (pos!=std::string::npos)
        {
        printf("\nAspack Packing Detected!\n");
          nopacking=0;
        }
        if(check ==0)
            {  pos= temp.find(optionaldection);
        if (pos!=std::string::npos)
        {
                           
                             
        cout<<"\n"<<argv[2]<<" packing Detected!\n";
          nopacking=0;
        }}
        /*
         printf("\nAspack Packing Detected!");
        if (strstr((string)secHead[i].Name.str(), "UPX") != NULL) 
        {
            printf("\nUPX Packing Detected!");
            break;
        }
        else if (strstr((string)secHead[i].Name.str(), "MPRESS") != NULL) 
        {
            printf("\nMpress Packing Detected!");
            break;
        }
        else if (strstr(secHead[i].Name, "ASPACK") != NULL) 
        {
            printf("\nAspack Packing Detected!");
            break;
        }
        else if (strstr(secHead[i].Name, "PETITE") != NULL) 
        {
            printf("\nPetite Packing Detected!");
            break;
        } */
    }
    if (nopacking==1)
    {
        printf("\nNo packing was Detected!\n");             
    }
  
    CloseHandle(host);
    system("PAUSE");
    return 0;
}
