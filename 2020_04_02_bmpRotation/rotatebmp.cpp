#include<iostream>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>

using std::cout;
using std::cin;
using std::endl;

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef long LONG;


//位图文件头定义
struct FileHeader
{
    DWORD bfSize;// 位图文件的大小，以字节为单位 
    WORD bfReserved1;//保留字1，位图文件保留字，必须为 0 
    WORD bfReserved2;//保留字2，位图文件保留字，必须为 0 
    DWORD bfOffBits;//位图数据的起始位置，以相对于位图文件头的偏移量表示，以字节为单位 
};

//位图信息头定义
struct InfoHeader
{
    DWORD biSize;//本结构所占用字节数,即信息头大小
    DWORD biWidth;//位图的宽度，以像素为单位
    DWORD biHeight;//位图的高度，以像素为单位
    WORD biPlanes;//目标设备的平面数不清，必须为 1 
    WORD biBitCount;//每个像素所需的位数，必须是 1(双色), 4(16 色)，8(256 色)或 24(真彩色)之一 
    DWORD biCompression;//位图压缩类型，必须是 0(不压缩),1(BI_RLE8 压缩类型)或 2(BI_RLE4压缩类型)之一 
    DWORD biSizeImage;//位图的大小，以字节为单位 
    DWORD biXPelsPerMeter;//位图水平分辨率，每米像素数 
    DWORD biYPelsPerMeter;//位图垂直分辨率，每米像素数 
    DWORD biClrUsed;//位图实际使用的颜色表中的颜色数 
    DWORD biClrImportant;//位图显示过程中重要的颜色数 
};

//像素信息,b,g,r
struct Data
{
    BYTE red;
    BYTE green;
    BYTE blue;
};

FileHeader srcHead;//原文件文件头
InfoHeader srcInfo;//原文件信息头
int h,w,size;//原文件的高，宽，尺寸(多少像素)

void get_head(FileHeader& head,FILE* input)
{
    fread(&head.bfSize,sizeof(DWORD),1,input);
    fread(&head.bfReserved1,sizeof(WORD),1,input);
    fread(&head.bfReserved2,sizeof(WORD),1,input);
    fread(&head.bfOffBits,sizeof(DWORD),1,input);
}

void out_head(FileHeader& head,FILE* output){
    WORD bfType=0x4d42;
    fwrite(&bfType,sizeof(WORD),1,output);
    fwrite(&head.bfSize,sizeof(DWORD),1,output);
    fwrite(&head.bfReserved1,sizeof(WORD),1,output);
    fwrite(&head.bfReserved2,sizeof(WORD),1,output);
    fwrite(&head.bfOffBits,sizeof(DWORD),1,output);
}

void get_info(InfoHeader& info,FILE* input){
    fread(&info.biSize,sizeof(DWORD),1,input);
    fread(&info.biWidth,sizeof(DWORD),1,input);
    fread(&info.biHeight,sizeof(DWORD),1,input);
    fread(&info.biPlanes,sizeof(WORD),1,input);
    fread(&info.biBitCount,sizeof(WORD),1,input);
    fread(&info.biCompression,sizeof(DWORD),1,input);
    fread(&info.biSizeImage,sizeof(DWORD),1,input);
    fread(&info.biXPelsPerMeter,sizeof(DWORD),1,input);
    fread(&info.biYPelsPerMeter,sizeof(DWORD),1,input);
    fread(&info.biClrUsed,sizeof(DWORD),1,input);
    fread(&info.biClrImportant,sizeof(DWORD),1,input);
}

void out_info(InfoHeader& info,FILE* output){
    fwrite(&info.biSize,sizeof(DWORD),1,output);
    fwrite(&info.biWidth,sizeof(DWORD),1,output);
    fwrite(&info.biHeight,sizeof(DWORD),1,output);
    fwrite(&info.biPlanes,sizeof(WORD),1,output);
    fwrite(&info.biBitCount,sizeof(WORD),1,output);
    fwrite(&info.biCompression,sizeof(DWORD),1,output);
    fwrite(&info.biSizeImage,sizeof(DWORD),1,output);
    fwrite(&info.biXPelsPerMeter,sizeof(DWORD),1,output);
    fwrite(&info.biYPelsPerMeter,sizeof(DWORD),1,output);
    fwrite(&info.biClrUsed,sizeof(DWORD),1,output);
    fwrite(&info.biClrImportant,sizeof(DWORD),1,output);
}

int main()
{
    char s;
    char d[100]="dest1.bmp",p[10]="1.bmp";
    cout<<"Please input the picture number you want to rotate and press enter to end the input"<<endl;
    cout<<"Among them:\n 1 is Picchu\n 2 is XP classic desktop\n 3 is world map,\n 4 is a certain icon"<<endl;
    cout<<"Please input the number:";
    while(cin>>s)
    {
    FILE* input;
    FILE* output;
    p[0]=s;
    input=fopen(p,"rb");

    WORD bfType;
    fread(&bfType,sizeof(WORD),1,input);

    FileHeader head;
    InfoHeader info;

    get_head(head,input);
    get_info(info,input);

    int perline=(info.biWidth*3+3)/4*4;
    info.biSizeImage=perline*info.biHeight;
    head.bfSize=info.biSizeImage+head.bfOffBits;

    BYTE* MAP;
    MAP=new BYTE[info.biSizeImage+10];
    for(int i=0;i<info.biSizeImage;i++)
    {
        fread(&MAP[i],sizeof(BYTE),1,input);
    }

    Data* Image;
    Image=new Data[info.biHeight*info.biWidth+10];
    for (int i=0;i<info.biHeight;i++){
        for (int j=0;j<info.biWidth;j++){
            Image[i*info.biWidth+j].red=MAP[i*perline+j*3];
            Image[i*info.biWidth+j].green=MAP[i*perline+j*3+1];
            Image[i*info.biWidth+j].blue=MAP[i*perline+j*3+2];
        }
    }

    Data* newImage;
    newImage=new Data[info.biHeight*info.biWidth+10];
    for (int i=0;i<info.biWidth;i++){
        for (int j=0;j<info.biHeight;j++){
            newImage[i*info.biHeight+j]=
            Image[(info.biWidth-1-i)+j*info.biWidth];
        }
    }

    int newPerLine=(info.biHeight*3+3)/4*4;
    int newSize=newPerLine*info.biWidth;
    BYTE* newMAP;
    newMAP=new BYTE[newSize+10];
    for (int i=0;i<info.biWidth;i++){
        for (int j=0;j<info.biHeight;j++){
            newMAP[i*newPerLine+j*3]=newImage[i*info.biHeight+j].red;
            newMAP[i*newPerLine+j*3+1]=newImage[i*info.biHeight+j].green;
            newMAP[i*newPerLine+j*3+2]=newImage[i*info.biHeight+j].blue;
        }
    }

    info.biSizeImage=newSize;
    head.bfSize=info.biSizeImage+head.bfOffBits;
    DWORD tmp;
    tmp=info.biHeight;
    info.biHeight=info.biWidth;
    info.biWidth=tmp;


    d[4]=s;
    output=fopen(d,"wb");
    
    out_head(head,output);
    out_info(info,output);
    for (int i=0;i<info.biSizeImage;i++){
        fwrite(&newMAP[i],sizeof(BYTE),1,output);
    }
    cout<<endl;
    cout<<"Rotation is OK, please check the generated image in the folder where the original image is located, and the name is  "<<d<<endl;
    cout<<"If you want to rotate other picture,please continue to enter Number;"<<endl;
    cout<<"Or you can enter Ctrl+Z to stop rotation."<<endl<<endl;
    cout<<"Please input your decision: ";


    }//while
    
    return 0;
}