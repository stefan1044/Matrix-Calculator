#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <graphics.h>
#include <winbgim.h>
#include <math.h>
#include <stdio.h>;
 
#define font_curent COMPLEX_FONT
#define bgcolor 1
 
using namespace std;
ifstream fin("matrice.in");
 
struct matrice{
    bool nula=1,inversa=0;
    float el[11][11];
    int L,C;
    char nume[20]="";
};
matrice m1,m2,m3;
struct area{
    int x1,x2,y1,y2;
    matrice *m;
    bool nula=1;
};
 
textsettingstype *prev_settings;
matrice matrice_salvate[20],m_nula;
 
int nr_matrice=1;
 
void menu();
void optiuni_matrice(int zona,matrice& m,matrice m_de_afisat);
void citire_matrice_tastatura(matrice &m, matrice matrice_de_afisat,int zona);
void citire_matrice_fisier();
void operatii_matrice();
void grafica_operatii_matrice(matrice m1,matrice m2);
void adunare(matrice m1,matrice m2,matrice &m3);
void diferenta(matrice m1,matrice m2,matrice(&m3));
void inmultire(matrice m1,matrice m2,matrice &m3,int &ok);
void ridicareputere(matrice &m,int nr);
void selectie_memorie(int zona_selectie,matrice m_de_afisat,int zona_matrice, matrice& m);
void initarea(area &zona,int x1,int x2,int y1,int y2);
void assign_matrice_to_zone(area zone[],int prima);
void scroll(area zone[],int &prima,int zona,matrice m,int zona_matrice,bool directie);
void blinking_cursor(area zona);
void filled_rectangle(area zona);
void afisare2(matrice m,int zona,int x_culoare=-1,int y_culoare=-1, int culoare=-1);
void optiuni_m3(matrice m3);
void transpusa(int zona,matrice m,int zona1,matrice &m1);
int determinant(float m[][11],int n);
int produssumaDP(matrice m,int ok,int ok1);
int produssumaDS(matrice m,int ok,int ok1);
bool inzona(int x,int y, area zona);
 
void menu(){
    cleardevice();
    setbkcolor(bgcolor);
    settextstyle(font_curent,HORIZ_DIR,8);
 
    int menuh=textheight("MENIU"),menuw=textwidth("MENIU"),iesireh=textheight("IESIRE"),iesirew=textwidth("IESIRE");
    memset(m1.el,0,sizeof m1.el); m1.nula=1; strcpy(m1.nume,"Matrice1");
    memset(m2.el,0,sizeof m2.el); m2.nula=1; strcpy(m2.nume,"Matrice2");
    memset(m3.el,0,sizeof m3.el); m3.nula=1;
    while(true)    {
        cleardevice();
 
        outtextxy(840,200,"MENIU");
        outtextxy(90,900,"IESIRE");
 
        rectangle(840,210+menuh,840+menuw,450);
        settextstyle(font_curent,HORIZ_DIR,2);
        outtextxy(850,380,"OPERATII CU MATRICE");
 
        while(true){
            if (ismouseclick(WM_LBUTTONDOWN)){
                int x,y;
                getmouseclick(WM_LBUTTONDOWN,x,y);
                if (x>840 && x<840+menuw && y>210+menuh && y<((800-(menuh-320))/2-50))
                    operatii_matrice();
                if (x>90 && x<90+iesirew && y>900 && y<900+iesireh)
                    exit(0);
                Sleep(100);
            }
        }
    }
    getch();
}
 
void operatii_matrice(){
 
    int x,y,ok_inmultire=1;
 
    grafica_operatii_matrice(m1,m2);
    settextstyle(font_curent,HORIZ_DIR,4);
 
    while(true){
 
        if (ismouseclick(WM_LBUTTONDOWN)){
            getmouseclick(WM_LBUTTONDOWN,x,y);
 
            ///click pe numele primei matrice
            if (x>0 && x <640 && y>0 && y<50){
                m1.nula=1;
                memset(m1.el,0,sizeof m1.el);
                for(int i=0;i<=20;i++) m1.nume[i]=NULL;
                grafica_operatii_matrice(m1,m2);
                optiuni_matrice(1,m1,m2);
                grafica_operatii_matrice(m1,m2);
            }
            ///click pe numele celei de-a doua matrice
            if (x>640 && x <1280 && y>0 && y<50){
                m2.nula=1;
                memset(m2.el,0,sizeof m2.el);
                for(int i=0;i<=20;i++) m2.nume[i]=NULL;
                grafica_operatii_matrice(m1,m2);
                optiuni_matrice(2,m2,m1);
                grafica_operatii_matrice(m1,m2);
            }
            ///Click pe plus
            if(x>0 && x<=320 && y>650 && y<800 && m1.nula==0 && m2.nula==0){
                adunare(m1,m2,m3);
                optiuni_m3(m3);
 
            }
            ///Click pe minus
            if(x>320 && x<=640 && y>650 && y<800 && m1.nula==0 && m2.nula==0){
                line(1280,540,1920,540);
                line(1600,540,1600,650);
                settextstyle(font_curent,HORIZ_DIR,4);
                outtextxy(1340,580,"M1-M2");
                outtextxy(1660,580,"M2-M1");
                int x1,y1;
                while(true){
                    if (ismouseclick(WM_LBUTTONDOWN)){
                        getmouseclick(WM_LBUTTONDOWN,x1,y1);
                        if(x1>1280&&x1<1600&&y1>540&&y1<650){
                            grafica_operatii_matrice(m1,m2);
                            diferenta(m1,m2,m3);x1=0;y1=0;
                            x=0;
                            y=0;
                            optiuni_m3(m3);
                        }
                        if(x1>1600&&x1<1920&&y1>540&&y1<650){
                            grafica_operatii_matrice(m1,m2);
                            diferenta(m2,m1,m3);x1=0;y1=0;
                            x=0;
                            y=0;
                            optiuni_m3(m3);
                        }
                    }
                }
            }
            ///Inmultire
            if (x>640 && x<=960&& y>650 &&y<800 && m1.nula==0 && m2.nula==0){
                line(1280,540,1920,540);
                line(1600,540,1600,650);
                settextstyle(font_curent,HORIZ_DIR,4);
                outtextxy(1340,580,"M1*M2");
                outtextxy(1660,580,"M2*M1");
                int x1,y1;
                while(true){
                    if (ismouseclick(WM_LBUTTONDOWN)){
                        getmouseclick(WM_LBUTTONDOWN,x1,y1);
                        if(x1>1280&&x1<1600&&y1>540&&y1<650){
                            grafica_operatii_matrice(m1,m2);
                            inmultire(m1,m2,m3,ok_inmultire);
                            if (ok_inmultire)
                                optiuni_m3(m3);
                            operatii_matrice();
                        }
                        x=0;
                        y=0;
                        if(x1>1600&&x1<1920&&y1>540&&y1<650){
                            grafica_operatii_matrice(m1,m2);
                            inmultire(m2,m1,m3,ok_inmultire);
                            if (ok_inmultire)
                                optiuni_m3(m3);
                            operatii_matrice();
                        }
                        x=0;
                        y=0;
                        optiuni_m3(m3);
                        }
                    }
                }
            ///Ridicare la putere
            if(x>960 && x<=1280 && y>650 && y<800 && (m1.nula==0 || m2.nula==0)){
                line(1280,540,1920,540);
                line(1600,540,1600,650);
                settextstyle(font_curent,HORIZ_DIR,4);
                outtextxy(1340,580,"Matricea 1");
                outtextxy(1660,580,"Matricea 2");
                int x1,y1,nr;
                while(true){
                    if (ismouseclick(WM_LBUTTONDOWN)){
                        getmouseclick(WM_LBUTTONDOWN,x1,y1);
                        if(x1>1280&&x1<1600&&y1>540&&y1<650 && m1.nula==0){
                            nr=getch();
                            while(nr<49 || nr>57){
                                nr=getch();
                            }
                            nr-='0';
                            grafica_operatii_matrice(m1,m2);
                            ridicareputere(m1,nr);x1=0;y1=0;x=0;
                            y=0;
                            settextstyle(font_curent,HORIZ_DIR,4);
                            optiuni_m3(m3);
                            int x2,y2;
                        }
                        if(x1>1600&&x1<1920&&y1>540&&y1<650 && m2.nula==0){
                            nr=getch();
                            while(nr<49 || nr>57){
                                nr=getch();
                            }
                            nr-='0';
                            memset(m1.el,0,sizeof m1.el); m1.nula=1; strcpy(m1.nume,"Matrice1");
                            grafica_operatii_matrice(m1,m2);
                            ridicareputere(m2,nr);x1=0;y1=0;x=0;
                            y=0;
                            settextstyle(font_curent,HORIZ_DIR,4);
                            optiuni_m3(m3);
                        }
                    }
                }
            }
            ///DP
            if (x>1280 && x<=1600 && y>650 && y<800 &&(m1.nula==0 || m2.nula==0))
            {
                char nr[100],nr1[100];
                settextstyle(font_curent,HORIZ_DIR,4);
                outtextxy(1380,600,"Matrice1");
                outtextxy(1550+textwidth("Matrice1"),600,"Matrice2");
                int x3,y3;
                while(true)
                {
                    if(ismouseclick(WM_LBUTTONDOWN))
                    {
                        getmouseclick(WM_LBUTTONDOWN,x3,y3);
                        if(x3>1380&&x3<1380+textwidth("Matrice1")&&y3>600&&y3<600+textheight("Matrice1") && m1.nula==0)
                        {
                            grafica_operatii_matrice(m1,m2);
                            settextstyle(COMPLEX_FONT,HORIZ_DIR,4);
                            outtextxy(1380,600,"Sub");
                            outtextxy(1480+textwidth("Sub"),600,"Deasupra");
                            outtextxy(1580+textwidth("Sub")+textwidth("Deasupra"),600,"Pe");
                            int x1,y1;
                            while(true)
                            {
                                if (ismouseclick(WM_LBUTTONDOWN))
                                {
                                    getmouseclick(WM_LBUTTONDOWN,x1,y1);
                                    if(x1>1380&&x1<1380+textwidth("Sub")&&y1>600&&y1<600+textheight("Sub"))///sub DP M1
                                    {
                                        grafica_operatii_matrice(m1,m2);
 
                                        settextstyle(COMPLEX_FONT,HORIZ_DIR,4);
                                        outtextxy(1380,600,"Suma");
                                        outtextxy(1650+textwidth("Suma"),600,"Produs");
                                        int x2,y2;
                                        while(true)
                                        {
                                            if (ismouseclick(WM_LBUTTONDOWN))
                                            {
                                                getmouseclick(WM_LBUTTONDOWN,x2,y2);
                                                if(x2>1380&&x2<1380+textwidth("Suma")&&y2>600&&y2<600+textheight("Suma"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
 
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Suma elementelor de sub DP ");
                                                    outtextxy(1350,10+textheight("Suma elementelor de sub DP ")," a primei matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDP(m1,0,1),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    x3=0;
                                                    y3=0;
                                                    optiuni_m3(m3);
                                                }
                                                if(x2>1650&&x2<1650+textwidth("Produs")+textwidth("Suma")&&y2>600&&y2<600+textheight("Produs"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
 
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Produsul elementelor de sub DP ");
                                                    outtextxy(1350,10+textheight("Produsul elementelor de sub DP ")," a primei matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDP(m1,1,0),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    optiuni_m3(m3);
                                                }
                                                if (x2>50 && x2<200 && y2>980 && y2<1080)
                                                    menu();
                                            }
                                            Sleep(100);
                                        }
                                    }
                                    if(x1>1480+textwidth("Sub")&&x1<1480+textwidth("Deasupra")&&y1>600&&y1<600+textheight("Deasupra")) ///deasupra DP M1
                                    {
                                        grafica_operatii_matrice(m1,m2);
                                        settextstyle(COMPLEX_FONT,HORIZ_DIR,4);
                                        outtextxy(1380,600,"Suma");
                                        outtextxy(1650+textwidth("Suma"),600,"Produs");
                                        int x2,y2;
                                        while(true)
                                        {
                                            if (ismouseclick(WM_LBUTTONDOWN))
                                            {
                                                getmouseclick(WM_LBUTTONDOWN,x2,y2);
                                                if(x2>1380&&x2<1380+textwidth("Suma")&&y2>600&&y2<600+textheight("Suma"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Suma elementelor de deasupra DP ");
                                                    outtextxy(1350,10+textheight("Suma elementelor de deasupra DP ")," a primei matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDP(m1,0,3),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    x3=0;
                                                    y3=0;
                                                    optiuni_m3(m3);
                                                }
                                                if(x2>1650&&x2<1650+textwidth("Produs")&&y2>600&&y2<600+textheight("Produs"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Produsul elementelor de deasupra DP ");
                                                    outtextxy(1350,10+textheight("Produsul elementelor de deasupra DP ")," a primei matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDP(m1,3,0),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    optiuni_m3(m3);
                                                }
                                                if (x2>50 && x2<200 && y2>980 && y2<1080)
                                                    menu();
                                            }
                                            Sleep(100);
                                        }
 
                                    }
                                    if(x1>1580+textwidth("Sub")+textwidth("Deasupra")&&x1<1580+textwidth("Sub")+textwidth("Deasupra")+textwidth("Pe")&&y1>600&&y1<600+textheight("Pe")) ///pe DP M1
                                    {
                                        grafica_operatii_matrice(m1,m2);
 
                                        settextstyle(COMPLEX_FONT,HORIZ_DIR,4);
                                        outtextxy(1380,600,"Suma");
                                        outtextxy(1650+textwidth("Suma"),600,"Produs");
                                        int x2,y2;
                                        while(true)
                                        {
                                            if (ismouseclick(WM_LBUTTONDOWN))
                                            {
                                                getmouseclick(WM_LBUTTONDOWN,x2,y2);
                                                if(x2>1380&&x2<1380+textwidth("Suma")&&y2>600&&y2<600+textheight("Suma"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
 
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Suma elementelor de pe DP ");
                                                    outtextxy(1350,10+textheight("Suma elementelor de [e DP ")," a primei matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDP(m1,0,2),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    x3=0;
                                                    y3=0;
                                                    optiuni_m3(m3);
                                                }
                                                if(x2>1650+textwidth("Suma")&&x2<1650+textwidth("Suma")+textwidth("Produs")&&y2>600&&y2<600+textheight("Produs"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Produsul elementelor de pe DP ");
                                                    outtextxy(1350,10+textheight("Produsul elementelor de pe DP ")," a primei matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDP(m1,2,0),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    optiuni_m3(m3);
                                                }
                                                if (x2>50 && x2<200 && y2>980 && y2<1080)
                                                    menu();
                                            }
                                            Sleep(100);
                                        }
                                    }
                                    if (x1>50 && x1<200 && y1>980 && y1<1080)///back
                                        menu();
                                }
                                Sleep(100);
                            }
                            x3=0;
                            y3=0;
                        }
                        if(x3>1550+textwidth("Matrice1")&&x3<1550+textwidth("Matrice1")+textwidth("Matrice2")&&y3>600&&y3<600+textheight("Matrice2") && m2.nula==0)  ///Pentru matricea 2 DP
                        {
                            grafica_operatii_matrice(m1,m2);
                            settextstyle(font_curent,HORIZ_DIR,4);
                            outtextxy(1380,600,"Sub");
                            outtextxy(1480+textwidth("Sub"),600,"Deasupra");
                            outtextxy(1580+textwidth("Sub")+textwidth("Deasupra"),600,"Pe");
                            int x1,y1;
                            while(true)
                            {
                                if (ismouseclick(WM_LBUTTONDOWN))
                                {
                                    getmouseclick(WM_LBUTTONDOWN,x1,y1);
                                    if(x1>1380&&x1<1380+textwidth("Sub")&&y1>600&&y1<600+textheight("Sub"))///sub DP M1
                                    {
                                        grafica_operatii_matrice(m1,m2);
                                        settextstyle(COMPLEX_FONT,HORIZ_DIR,4);
                                        outtextxy(1380,600,"Suma");
                                        outtextxy(1650+textwidth("Suma"),600,"Produs");
                                        int x2,y2;
                                        while(true)
                                        {
                                            if (ismouseclick(WM_LBUTTONDOWN))
                                            {
                                                getmouseclick(WM_LBUTTONDOWN,x2,y2);
                                                if(x2>1380&&x2<1380+textwidth("Suma")&&y2>600&&y2<600+textheight("Suma"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Suma elementelor de sub DP ");
                                                    outtextxy(1350,10+textheight("Suma elementelor de sub DP ")," din a doua matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDP(m2,0,1),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    optiuni_m3(m3);
                                                }
                                                if(x2>1650+textwidth("Suma")&&x2<1650+textwidth("Suma")+textwidth("Produs")&&y2>600&&y2<600+textheight("Produs"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Produsul elementelor de sub DP ");
                                                    outtextxy(1350,10+textheight("Produsul elementelor de sub DP ")," din a doua matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDP(m2,1,0),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    optiuni_m3(m3);
                                                }
                                                if (x2>50 && x2<200 && y2>980 && y2<1080)
                                                    menu();
                                            }
                                            Sleep(100);
                                        }
                                    }
                                    if(x1>1480&&x1<1480+textwidth("Deasupra")&&y1>600&&y1<600+textheight("Deasupra")) ///deasupra DP M2
                                    {
                                        grafica_operatii_matrice(m1,m2);
                                        settextstyle(COMPLEX_FONT,HORIZ_DIR,4);
                                        outtextxy(1380,600,"Suma");
                                        outtextxy(1650+textwidth("Suma"),600,"Produs");
                                        int x2,y2;
                                        while(true)
                                        {
                                            if (ismouseclick(WM_LBUTTONDOWN))
                                            {
                                                getmouseclick(WM_LBUTTONDOWN,x2,y2);
                                                if(x2>1380&&x2<1380+textwidth("Suma")&&y2>600&&y2<600+textheight("Suma"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Suma elementelor de deasupra DP ");
                                                    outtextxy(1350,10+textheight("Suma elementelor de deasupra DP ")," din a doua matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDP(m2,0,3),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    optiuni_m3(m3);
                                                    x3=0;
                                                    y3=0;
                                                }
                                                if(x2>1650+textwidth("Suma")&&x2<1650+textwidth("Suma")+textwidth("Produs")&&y2>600&&y2<600+textheight("Produs"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Produsul elementelor de deasupra DP ");
                                                    outtextxy(1350,10+textheight("Produsul elementelor de deasupra DP ")," din a doua matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDP(m2,3,0),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    optiuni_m3(m3);
 
                                                }
                                                if (x2>50 && x2<200 && y2>980 && y2<1080)
                                                    menu();
                                            }
                                            Sleep(100);
                                        }
                                    }
                                    if(x1>1580+textwidth("Sub")+textwidth("Deasupra")&&x1<1580+textwidth("Sub")+textwidth("Deasupra")+textwidth("Pe")&&y1>600&&y1<600+textheight("Pe")) ///pe DP M2
                                    {
                                        grafica_operatii_matrice(m1,m2);
                                        settextstyle(COMPLEX_FONT,HORIZ_DIR,4);
                                        outtextxy(1380,600,"Suma");
                                        outtextxy(1650+textwidth("Suma"),600,"Produs");
                                        int x2,y2;
                                        while(true)
                                        {
                                            if (ismouseclick(WM_LBUTTONDOWN))
                                            {
                                                getmouseclick(WM_LBUTTONDOWN,x2,y2);
                                                if(x2>1380&&x2<1380+textwidth("Suma")&&y2>600&&y2<600+textheight("Suma"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Suma elementelor de pe DP ");
                                                    outtextxy(1350,10+textheight("Suma elementelor de [e DP ")," din a doua matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDP(m2,0,2),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    x3=0;
                                                    y3=0;
                                                    optiuni_m3(m3);
                                                }
                                                if(x2>1650+textwidth("Suma")&&x2<1650+textwidth("Suma")+textwidth("Produs")&&y2>600&&y2<600+textheight("Produs"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Produsul elementelor de pe DP ");
                                                    outtextxy(1350,10+textheight("Produsul elementelor de pe DP ")," din a doua matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDP(m2,2,0),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    optiuni_m3(m3);
                                                }
                                                if (x2>50 && x2<200 && y2>980 && y2<1080)
                                                    menu();
                                            }
                                            Sleep(100);
                                        }
                                    }
                                    if (x1>50 && x1<200 && y1>980 && y1<1080)///back
                                        menu();
                                }
                                Sleep(100);
                            }
                            x3=0;
                            y3=0;
                        }
                        if (x3>50 && x3<200 && y3>980 && y3<1080)///back
                            menu();
 
                    }
                    Sleep(100);
                }
            }
            ///DS
            if (x>1600 && x<=1920 && y>650 && y<800 &&(m1.nula==0 || m2.nula==0))
            {
                char nr[100],nr1[100];
                settextstyle(font_curent,HORIZ_DIR,4);
                outtextxy(1380,600,"Matrice1");
                outtextxy(1550+textwidth("Matrice1"),600,"Matrice2");
                int x3,y3;
                while(true)
                {
                    if(ismouseclick(WM_LBUTTONDOWN))
                    {
                        getmouseclick(WM_LBUTTONDOWN,x3,y3);
                        if(x3>1380&&x3<1380+textwidth("Matrice1")&&y3>600&&y3<600+textheight("Matrice1") && m1.nula==0)
                        {
                            grafica_operatii_matrice(m1,m2);
                            settextstyle(COMPLEX_FONT,HORIZ_DIR,4);
                            outtextxy(1380,600,"Sub");
                            outtextxy(1480+textwidth("Sub"),600,"Deasupra");
                            outtextxy(1580+textwidth("Sub")+textwidth("Deasupra"),600,"Pe");
                            int x1,y1;
                            while(true)
                            {
                                if (ismouseclick(WM_LBUTTONDOWN))
                                {
                                    getmouseclick(WM_LBUTTONDOWN,x1,y1);
                                    if(x1>1380&&x1<1380+textwidth("Sub")&&y1>600&&y1<600+textheight("Sub"))///sub DS M1
                                    {
                                        grafica_operatii_matrice(m1,m2);
 
                                        settextstyle(COMPLEX_FONT,HORIZ_DIR,4);
                                        outtextxy(1380,600,"Suma");
                                        outtextxy(1650+textwidth("Suma"),600,"Produs");
                                        int x2,y2;
                                        while(true)
                                        {
                                            if (ismouseclick(WM_LBUTTONDOWN))
                                            {
                                                getmouseclick(WM_LBUTTONDOWN,x2,y2);
                                                if(x2>1380&&x2<1380+textwidth("Suma")&&y2>600&&y2<600+textheight("Suma"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Suma elementelor de sub DP ");
                                                    outtextxy(1350,10+textheight("Suma elementelor de sub DP ")," a primei matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDS(m1,0,1),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    optiuni_m3(m3);
                                                    x3=0;
                                                    y3=0;
                                                }
                                                if(x2>1650&&x2<1650+textwidth("Produs")+textwidth("Suma")&&y2>600&&y2<600+textheight("Produs"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
 
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Produsul elementelor de sub DP ");
                                                    outtextxy(1350,10+textheight("Produsul elementelor de sub DP ")," a primei matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDS(m1,1,0),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    optiuni_m3(m3);
                                                }
                                                if (x2>50 && x2<200 && y2>980 && y2<1080)
                                                    menu();
                                            }
                                            Sleep(100);
                                        }
                                    }
                                    if(x1>1480+textwidth("Sub")&&x1<1480+textwidth("Deasupra")&&y1>600&&y1<600+textheight("Deasupra")) ///deasupra DS M1
                                    {
                                        grafica_operatii_matrice(m1,m2);
                                        settextstyle(COMPLEX_FONT,HORIZ_DIR,4);
                                        outtextxy(1380,600,"Suma");
                                        outtextxy(1650+textwidth("Suma"),600,"Produs");
                                        int x2,y2;
                                        while(true)
                                        {
                                            if (ismouseclick(WM_LBUTTONDOWN))
                                            {
                                                getmouseclick(WM_LBUTTONDOWN,x2,y2);
                                                if(x2>1380&&x2<1380+textwidth("Suma")&&y2>600&&y2<600+textheight("Suma"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Suma elementelor de deasupra DP ");
                                                    outtextxy(1350,10+textheight("Suma elementelor de deasupra DP ")," a primei matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDS(m1,0,3),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    optiuni_m3(m3);
                                                    x3=0;
                                                    y3=0;
                                                }
                                                if(x2>1650&&x2<1650+textwidth("Produs")&&y2>600&&y2<600+textheight("Produs"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Produsul elementelor de deasupra DP ");
                                                    outtextxy(1350,10+textheight("Produsul elementelor de deasupra DP ")," a primei matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDS(m1,3,0),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    optiuni_m3(m3);
                                                }
                                                if (x2>50 && x2<200 && y2>980 && y2<1080)
                                                    menu();
                                            }
                                            Sleep(100);
                                        }
 
                                    }
                                    if(x1>1580+textwidth("Sub")+textwidth("Deasupra")&&x1<1580+textwidth("Sub")+textwidth("Deasupra")+textwidth("Pe")&&y1>600&&y1<600+textheight("Pe")) ///pe DS M1
                                    {
                                        grafica_operatii_matrice(m1,m2);
                                        settextstyle(COMPLEX_FONT,HORIZ_DIR,4);
                                        outtextxy(1380,600,"Suma");
                                        outtextxy(1650+textwidth("Suma"),600,"Produs");
                                        int x2,y2;
                                        while(true)
                                        {
                                            if (ismouseclick(WM_LBUTTONDOWN))
                                            {
                                                getmouseclick(WM_LBUTTONDOWN,x2,y2);
                                                if(x2>1380&&x2<1380+textwidth("Suma")&&y2>600&&y2<600+textheight("Suma"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
 
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Suma elementelor de pe DP ");
                                                    outtextxy(1350,10+textheight("Suma elementelor de [e DP ")," a primei matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDS(m1,0,2),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    optiuni_m3(m3);
                                                    x3=0;
                                                    y3=0;
                                                }
                                                if(x2>1650+textwidth("Suma")&&x2<1650+textwidth("Suma")+textwidth("Produs")&&y2>600&&y2<600+textheight("Produs"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Produsul elementelor de pe DP ");
                                                    outtextxy(1350,10+textheight("Produsul elementelor de pe DP ")," a primei matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDS(m1,2,0),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    optiuni_m3(m3);
                                                }
                                                if (x2>50 && x2<200 && y2>980 && y2<1080)
                                                    menu();
                                            }
                                            Sleep(100);
                                        }
                                    }
                                    if (x1>50 && x1<200 && y1>980 && y1<1080)///back
                                        menu();
                                }
                                Sleep(100);
                            }
                            x3=0;
                            y3=0;
                        }
                        if(x3>1550+textwidth("Matrice1")&&x3<1550+textwidth("Matrice1")+textwidth("Matrice2")&&y3>600&&y3<600+textheight("Matrice2") && m2.nula==0)  ///Pentru matricea 2 DS
                        {
                            grafica_operatii_matrice(m1,m2);
                            outtextxy(1380,600,"Sub");
                            outtextxy(1480+textwidth("Sub"),600,"Deasupra");
                            outtextxy(1580+textwidth("Sub")+textwidth("Deasupra"),600,"Pe");
                            int x1,y1;
                            while(true)
                            {
                                if (ismouseclick(WM_LBUTTONDOWN))
                                {
                                    getmouseclick(WM_LBUTTONDOWN,x1,y1);
                                    if(x1>1380&&x1<1380+textwidth("Sub")&&y1>600&&y1<600+textheight("Sub"))///sub DS M1
                                    {
                                        grafica_operatii_matrice(m1,m2);
                                        settextstyle(COMPLEX_FONT,HORIZ_DIR,4);
                                        outtextxy(1380,600,"Suma");
                                        outtextxy(1650+textwidth("Suma"),600,"Produs");
                                        int x2,y2;
                                        while(true)
                                        {
                                            if (ismouseclick(WM_LBUTTONDOWN))
                                            {
                                                getmouseclick(WM_LBUTTONDOWN,x2,y2);
                                                if(x2>1380&&x2<1380+textwidth("Suma")&&y2>600&&y2<600+textheight("Suma"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Suma elementelor de sub DP ");
                                                    outtextxy(1350,10+textheight("Suma elementelor de sub DP ")," din a doua matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDS(m2,0,1),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    optiuni_m3(m3);
                                                }
                                                if(x2>1650+textwidth("Suma")&&x2<1650+textwidth("Suma")+textwidth("Produs")&&y2>600&&y2<600+textheight("Produs"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Produsul elementelor de sub DP ");
                                                    outtextxy(1350,10+textheight("Produsul elementelor de sub DP ")," din a doua matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDS(m2,1,0),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    optiuni_m3(m3);
                                                }
                                                if (x2>50 && x2<200 && y2>980 && y2<1080)
                                                    menu();
                                            }
                                            Sleep(100);
                                        }
                                    }
                                    if(x1>1480&&x1<1480+textwidth("Deasupra")&&y1>600&&y1<600+textheight("Deasupra")) ///deasupra DS M1
                                    {
 
                                        grafica_operatii_matrice(m1,m2);
                                        settextstyle(COMPLEX_FONT,HORIZ_DIR,4);
                                        outtextxy(1380,600,"Suma");
                                        outtextxy(1650+textwidth("Suma"),600,"Produs");
                                        int x2,y2;
                                        while(true)
                                        {
                                            if (ismouseclick(WM_LBUTTONDOWN))
                                            {
                                                getmouseclick(WM_LBUTTONDOWN,x2,y2);
                                                if(x2>1380&&x2<1380+textwidth("Suma")&&y2>600&&y2<600+textheight("Suma"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Suma elementelor de deasupra DP ");
                                                    outtextxy(1350,10+textheight("Suma elementelor de deasupra DP ")," din a doua matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDS(m2,0,3),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    optiuni_m3(m3);
                                                    x3=0;
                                                    y3=0;
                                                }
                                                if(x2>1650+textwidth("Suma")&&x2<1650+textwidth("Suma")+textwidth("Produs")&&y2>600&&y2<600+textheight("Produs"))
                                                {
                                                    grafica_operatii_matrice(m1,m2);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Produsul elementelor de deasupra DP ");
                                                    outtextxy(1350,10+textheight("Produsul elementelor de deasupra DP ")," din a doua matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDS(m2,3,0),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    optiuni_m3(m3);
                                                }
                                                if (x2>50 && x2<200 && y2>980 && y2<1080)
                                                    menu();
                                            }
                                            Sleep(100);
                                        }
                                    }
                                    if(x1>1580&&x1<1580+textwidth("Pe")&&y1>600&&y1<600+textheight("Pe")) ///pe DS M1
                                    {
                                        grafica_operatii_matrice(m1,m2);
                                        settextstyle(COMPLEX_FONT,HORIZ_DIR,4);
                                        int x2,y2;
                                        outtextxy(1380,600,"Suma");
                                        outtextxy(1650+textwidth("Suma"),600,"Produs");
                                            while(true){
                                                if (ismouseclick(WM_LBUTTONDOWN)){
                                                getmouseclick(WM_LBUTTONDOWN,x2,y2);
                                                    if(x2>1380&&x2<1380+textwidth("Suma")&&y2>600&&y2<600+textheight("Suma"))
                                                    {
                                                        grafica_operatii_matrice(m1,m2);
                                                        settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                        outtextxy(1330,10,"Suma elementelor de pe DP ");
                                                        outtextxy(1350,10+textheight("Suma elementelor de [e DP ")," din a doua matrice este ");
                                                        x2=0;
                                                        y2=0;
                                                        itoa(produssumaDP(m1,0,2),nr,10);
                                                        settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                        outtextxy(1580,300,nr);
                                                        optiuni_m3(m3);
                                                        x3=0;
                                                        y3=0;
                                                    }
                                                    if(x2>1650+textwidth("Suma")&&x2<1650+textwidth("Suma")+textwidth("Produs")&&y2>600&&y2<600+textheight("Produs"))
                                                    {
                                                    grafica_operatii_matrice(m1,m2);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,3);
                                                    outtextxy(1330,10,"Produsul elementelor de pe DP ");
                                                    outtextxy(1350,10+textheight("Produsul elementelor de pe DP ")," din a doua matrice este ");
                                                    x2=0;
                                                    y2=0;
                                                    itoa(produssumaDP(m1,2,0),nr,10);
                                                    settextstyle(COMPLEX_FONT,HORIZ_DIR,6);
                                                    outtextxy(1580,300,nr);
                                                    optiuni_m3(m3);
                                                    }
                                                    if (x2>50 && x2<200 && y2>980 && y2<1080) menu();
                                                }
                                            Sleep(100);
                                            }
                                        }
                                        if (x1>50 && x1<200 && y1>980 && y1<1080)menu();
                                    }
                                    Sleep(100);
                                }
                                x3=0;
                                y3=0;
                            }
                            if (x3>50 && x3<200 && y3>980 && y3<1080)menu();
 
                        }
                        Sleep(100);
                    }
                }
            /// DETERMINANT
            if(x>0 && x<640 && y>800 && y<950){
                line(1280,540,1920,540);
                line(1600,540,1600,650);
                settextstyle(font_curent,HORIZ_DIR,4);
                outtextxy(1340,580,"Matricea 1");
                outtextxy(1660,580,"Matricea 2");
                int x1,y1,nr;
                while(true){
                    if (ismouseclick(WM_LBUTTONDOWN)){
                        getmouseclick(WM_LBUTTONDOWN,x1,y1);
                        if(x1>1280&&x1<1600&&y1>540&&y1<650){
                            char nr[100];
                            itoa(determinant(m1.el,m1.L),nr,10);
                            settextstyle(font_curent,HORIZ_DIR,6);
                            outtextxy(1600,265,nr);x1=0;y1=0;
                            optiuni_m3(m3);
                        }
                        if(x1>1600&&x<1920&&y1>540&&y1<650){
                            char nr[100];
                            itoa(determinant(m2.el,m2.L),nr,10);
                            settextstyle(font_curent,HORIZ_DIR,6);
                            outtextxy(1600,265,nr);x1=0;y1=0;
                            optiuni_m3(m3);
                        }
                        if (x1>50 && x1<200 && y1>980 && y1<1080)
                            menu();
                }
                Sleep(100);
            }
        }
        if (x>920 && x<1920 && y>800 && y<950)///transpusa
        {
                        int x1,y1,nr;
                        if(m1.nula==0 && m2.nula==1){
                            grafica_operatii_matrice(m1,m2);
                                    transpusa(1,m1,3,m3);
                                    x1=0;
                                    y1=0;
                        }
                        else if (m1.nula==1 && m2.nula==0){
                            grafica_operatii_matrice(m1,m2);
                                    transpusa(2,m2,3,m3);
                                    x1=0;
                                    y1=0;
                        }
                        else if (m1.nula==0 && m2.nula==0){
                            settextstyle(font_curent,HORIZ_DIR,4);
                            outtextxy(1380,600,"Matricea 1");
                            outtextxy(1700,600,"Matricea 2");
                            while(true){
                                if (ismouseclick(WM_LBUTTONDOWN))
                                {
                                    getmouseclick(WM_LBUTTONDOWN,x1,y1);
                                    if(x1>1380&&x1<1380+textwidth("Matricea 1")&&y1>600&&y1<600+textheight("Matricea 1"))
                                    {
                                        grafica_operatii_matrice(m1,m2);
                                        transpusa(1,m1,3,m3);
                                        x1=0;
                                        y1=0;
                                    }
                                    if(x1>1700&&x<1700+textwidth("Matricea 2")&&y1>600&&y1<600+textheight("Matricea 2"))
                                    {
                                        grafica_operatii_matrice(m1,m2);
                                        transpusa(2,m2,3,m3);
                                        x1=0;
                                        y1=0;
                                    }
                                    if (x1>50 && x1<200 && y1>980 && y1<1080)
                                        menu();
                                }
                                Sleep(100);
                            }
                        }
            }
 
 
 
 
        if (x>50 && x <200 && y>980 && y<1080)
            menu();
    }
    Sleep(10);
}
}
 
 
void grafica_operatii_matrice(matrice m1,matrice m2){
    int x=0,y=0;
    cleardevice();
 
    settextstyle(font_curent,HORIZ_DIR,4);
    outtextxy(10,10,m1.nume);
    outtextxy(650,10,m2.nume);
 
    settextstyle(font_curent,HORIZ_DIR,4);
    if(!m1.nula)
        afisare2(m1,1);
 
    settextstyle(font_curent,HORIZ_DIR,4);
    if(!m2.nula)
        afisare2(m2,2);
 
    //unde se termina matricele
    line(0,650,1920,650);
    x=640;
    for(int i=1; i<=2; i++){
        line(x,0,x,650);
        x+=640;
    }
    //linia mijloc semne
    line(0,800,1920,800);
    //ultima linie de jos
    line(0,950,1920,950);
 
    x=320;
    for(int i=1; i<=6; i++){
        line(x,650,x,800);
        x+=320;
    }
    x=640;
 
    //Liniile de sub numele matricelors
    line(0,50,640,50);
    line(640,50,1280,50);
 
    settextstyle(font_curent,HORIZ_DIR,7);
    outtextxy(120,700,"+");
    outtextxy(450,700,"-");
    outtextxy(790,700,"*");
    outtextxy(1100,700,"^");
    outtextxy(1400,700,"DP");
    outtextxy(1700,700,"DS");
 
    //determinant transpusa
    line(960,800,960,950);
    outtextxy(80,850,"Determinant");
    outtextxy(1040,850,"Transpusa");
 
    outtextxy(50,980,"MENIU");
 
}
 
void optiuni_matrice(int zona,matrice &m,matrice m_de_afisat){
    int x1,y1=50,x2,y2=650,x,y;
    if (zona==1){
        x1=0;
        x2=640;
    }
    else{
        x1=640;
        x2=1280;
    }
 
    settextstyle(font_curent,HORIZ_DIR,5);
    outtextxy(x1+150,y1+35,"citire matrice");
    outtextxy(x1+150,y1+textheight("citire matrice")+235,"din memorie");
 
    while(true){
        if(ismouseclick(WM_LBUTTONDOWN)){
            getmouseclick(WM_LBUTTONDOWN,x,y);
            if(x>(x1+150) && x<(x1+150+textwidth("citire matrice")) && y>(y1+35) && y<(y1+35+textheight("citire matrice"))){
                citire_matrice_tastatura(m,m_de_afisat,zona);
                break;
            }
            if(x>(x1+150) && x<(x1+150+textwidth("din memorie")) && y>(y1+270) && y<(y1+textheight("din memorie")) + 270 && nr_matrice>1){
                selectie_memorie(zona,m_de_afisat,3-zona,m);
                break;
            }
        }
    }
}
 
void selectie_memorie(int zona_selectie,matrice m_de_afisat,int zona_matrice,matrice& m){
bool click=0;
area zone[12];
int x,y=65;
matrice m3,m4;m3.nula=1;m4.nula=1;
 
///Pentru ce zona am apelat functia
if (zona_selectie==1) x=0;
else x=640;
 
for(int i=1;i<=11;i++){
    initarea(zone[i],x+15,x+590,y,y+50);
    y+=50;
}
 
///Ce afisez in cealalta zona, o matrice sau nimic
if (m_de_afisat.nula==0){
    if(zona_matrice==1)
    grafica_operatii_matrice(m_de_afisat,m3);
    else
    grafica_operatii_matrice(m3,m_de_afisat);
}
else grafica_operatii_matrice(m3,m4);
 
///textheight == 20 la dimensiunea asta
settextstyle(font_curent,HORIZ_DIR,2);
 
int x1,y1;
 
if(nr_matrice<=11){
    for(int i=1;i<=nr_matrice;i++){
        zone[i].m = &matrice_salvate[i];
        outtextxy(zone[i].x1,zone[i].y1,zone[i].m->nume);
    }
 
    while(!click){
        if(ismouseclick(WM_LBUTTONDOWN)){
            getmouseclick(WM_LBUTTONDOWN,x1,y1);
 
        for(int i=1;i<=nr_matrice;i++){
            if(inzona(x1,y1,zone[i])){
                m=*zone[i].m;
                click=1;
                break;
            }
        }
        }
    }
}
else{
    int prima=1;
///bara
    line(x+590,50,x+590,650);
///triunghi cu varf in sus
    line(x+590,100,x+640,100);
    line(x+615,50,x+590,100);
    line(x+615,50,x+640,100);
///triunghi cu varf in jos
    line(x+590,600,x+640,600);
    line(x+615,650,x+590,600);
    line(x+615,650,x+640,600);
 
    area sageata_sus,sageata_jos;
    initarea(sageata_sus,x+590,x+640,50,100);
    initarea(sageata_jos,x+590,x+640,600,650);
 
    assign_matrice_to_zone(zone,prima);
 
    for(int i=1;i<=11;i++)
        outtextxy(zone[i].x1,zone[i].y1,zone[i].m->nume);
    outtextxy(x+15,615,"...");
 
    while(!click){
        if(ismouseclick(WM_LBUTTONDOWN)){
            getmouseclick(WM_LBUTTONDOWN,x1,y1);
        }
        if (inzona(x1,y1,sageata_sus)){
            scroll(zone,prima,zona_selectie,m_de_afisat,zona_matrice,0);x1=0;y1=0;}
        if (inzona(x1,y1,sageata_jos)){
            scroll(zone,prima,zona_selectie,m_de_afisat,zona_matrice,1);x1=0;y1=0;}
 
        for(int i=1;i<=11;i++){
            if(inzona(x1,y1,zone[i])){
                m=*zone[i].m;
                click=1;
                break;
            }
        }
        Sleep(50);
    }
}
}
 
void scroll(area zone[],int &prima,int zona,matrice m,int zona_matrice,bool directie){
    int x;
    matrice m3,m4;m3.nula=1;m4.nula=1;
 
    if (zona==1)
        x=0;
    else x=640;
 
///Daca apas sageata sus cand afisez de la primul element sau sageata jos cand afisez deja pana la ultimul
    if ((directie && ((prima+11)>nr_matrice)) || (!directie && (prima-1==0)))
        return;
 
///Ce afisez in cealalta zona, o matrice sau nimic
    if (m.nula==0){
        if(zona_matrice==1)
        grafica_operatii_matrice(m,m3);
        else
        grafica_operatii_matrice(m3,m);
    }
    else grafica_operatii_matrice(m3,m4);
 
///bara
    line(x+590,50,x+590,650);
///triunghi cu varf in sus
    line(x+590,100,x+640,100);
    line(x+615,50,x+590,100);
    line(x+615,50,x+640,100);
///triunghi cu varf in jos
    line(x+590,600,x+640,600);
    line(x+615,650,x+590,600);
    line(x+615,650,x+640,600);
 
    if(directie)
            prima++;
    else
        prima--;
    settextstyle(font_curent,HORIZ_DIR,2);
 
    assign_matrice_to_zone(zone,prima);
    for(int i=1;i<=11;i++)
        outtextxy(zone[i].x1,zone[i].y1,zone[i].m->nume);
 
    if(prima+11<=nr_matrice)
        outtextxy(x+15,615,"...");
}
 
void assign_matrice_to_zone(area zone[],int prima){
    int x=prima;
    for(int i=1;i<=11;i++){
        zone[i].m= &matrice_salvate[x];
        x++;
    }
}
 
void initarea(area &zona,int x1,int x2,int y1,int y2){
    zona.x1=x1;
    zona.x2=x2;
    zona.y1=y1;
    zona.y2=y2;
}
 
bool inzona(int x,int y,area zona){
    if(x>zona.x1 && x<zona.x2 && y>zona.y1 && y<zona.y2)
        return true;
    return false;
}
 
void adunare(matrice m1,matrice m2,matrice &m3){
    int x,y,culoare=4;
    int minim;
    m3.C=max(m1.C,m2.C);
    m3.L=max(m1.L,m2.L);
    minim=min(m3.C,m3.L); minim--; minim*=50;
    for(int i=1;i<=m3.L;i++){
        for(int j=1;j<=m3.C;j++){
            m3.el[i][j]= m1.el[i][j]+m2.el[i][j];
            afisare2(m1,1,i,j,culoare);
            afisare2(m2,2,i,j,culoare);Sleep(500-minim);
            afisare2(m3,3,i,j,culoare);
            Sleep(1000-minim);
        }
    }
}
 
void diferenta(matrice m1,matrice m2,matrice &m3){
    int x,y,culoare=4;
    int minim;
    if(m1.L>=m2.L){
        m3.L=m1.L;
        if(m1.C>m2.C){
            m3.C=m1.C;
        }
        else if(m1.C<m2.C){
            m3.C=m2.C;
        }
        else m3.C=m1.C;
    }
    else{
        m3.L=m2.L;
        if(m1.C<m2.C){
            m3.C=m2.C;
        }
        else if(m1.C>m2.C){
            m3.C=m1.C;
        }
        else m3.C=m1.C;
    }
    minim=min(m3.L,m3.C); minim--; minim*=50;
    for(int i=1; i<=m3.L; i++)
        for(int j=1; j<=m3.C; j++){
            m3.el[i][j] = m1.el[i][j] - m2.el[i][j];
            afisare2(m1,1,i,j,culoare);
            afisare2(m2,2,i,j,culoare);Sleep(500-minim);
            afisare2(m3,3,i,j,culoare);
            Sleep(1000-minim);
        }
    m3.nula=0;
}
 
void inmultire(matrice m1,matrice m2,matrice &m3,int &ok){
    int culoare=14;
    if(m1.C==m2.L){
        m3.L=m1.L;
        m3.C=m2.C;
        int minim=m3.L;
        for(int i=1; i<=m3.L; i++)
            for(int j=1; j<=m3.C; j++)
                m3.el[i][j]=0;
        for(int i=1; i<=m1.L; i++)
            for(int j=1; j<=m2.C; j++)
                for(int k=1; k<=m1.C; k++){
                    m3.el[i][j]+=m1.el[i][k]*m2.el[k][j];
                    afisare2(m1,1,i,k,culoare);
                    Sleep(250/minim);
                    afisare2(m2,2,k,j,culoare);
                    Sleep(250/minim);
                    afisare2(m3,3,i,j,culoare);
                    Sleep(250/minim);
                }
    }
    else{
            ok=0;
            settextstyle(TRIPLEX_FONT,HORIZ_DIR,4);
            outtextxy(1320,215,"NU ESTE POSIBILA");
            outtextxy(1350,215+textheight("NU ESTE POSIBILA"),"INMULTIREA!");
            Sleep(3500);
    }
}
 
void ridicareputere(matrice &m,int nr){
    int ok;
    if(m.L!=m.C){
        settextstyle(TRIPLEX_FONT,HORIZ_DIR,4);
        outtextxy(1320,215,"NU ESTE POSIBILA");
        outtextxy(1330,215+textheight("NU ESTE POSIBILA"),"RIDICAREA LA PUTERE");
        Sleep(3500);
    }
    else{
        matrice m3,m2,m4;
        m3=m;
        m2=m;
        for(int i=1; i<nr; i++){
            inmultire(m2,m3,m4,ok);
            m2=m4;
            m3=m;
        }
    }
}
 
int determinant( float m[][11],int n){
    int det = 0;
    float submatrix[11][11];
    if (n == 2)
    return ((m[1][1] * m[2][2]) - (m[2][1] * m[1][2]));
    else {
        for (int x = 1; x <= n; x++) {
            int subi = 1;
            for (int i = 2; i <= n; i++) {
                int subj = 1;
                for (int j = 1; j <= n; j++) {
                    if (j == x) continue;
                    submatrix[subi][subj] = m[i][j];
                    subj++;
                }
                subi++;
            }
            det = det + (pow(-1, x-1) * m[1][x] * determinant( submatrix, n - 1 ));
        }
   }
   return det;
}
 
 
void blinking_cursor(area zona){
    int i;
    rectangle(zona.x1,zona.y1,zona.x2,zona.y2);
    //int maxcolor = getmaxcolor(); NO
 
    setfillstyle(SOLID_FILL, 15);
    while(!kbhit()){
        setcolor(15);
        filled_rectangle(zona);
        i=1;
        while(!kbhit() && i<=200){
            i++;
            Sleep(1);
        }
 
        setcolor(bgcolor);
        filled_rectangle(zona);
        i=1;
        while(!kbhit() && i<=200){
            i++;
            Sleep(1);
        }
    }
    setcolor(bgcolor);
    filled_rectangle(zona);
    setcolor(15);
}
 
void filled_rectangle(area zona){
    int x1=zona.x1,x2=zona.x2,y1=zona.y1,y2=zona.y2;
    line(x1+(x2-x1)/2,y1,x1+(x2-x1)/2,y2);
 
    rectangle(x1,y1,x2,y2);
    x1++;y1++;x2--;y2--;
 
    while(x1!=x2 && x1<x2){
        rectangle(x1,y1,x2,y2);
        x1++;y1++;x2--;y2--;
    }
}
 
void citire_matrice_tastatura(matrice &m, matrice matrice_de_afisat,int zona){
    area cursor,zone[10][10];
    char c,*p=&c,*aux,cr[10];
    int x1,caracter,nrlitere=0,x1_aux,ratiex,ratiey,y1;
 
    if(zona==1){
        x1=10;
        m1.nula=1;
        grafica_operatii_matrice(m,matrice_de_afisat);
    }
    else {
        x1=650;
        m2.nula=1;
        grafica_operatii_matrice(matrice_de_afisat,m);
    }
 
    settextstyle(font_curent,HORIZ_DIR,4);
    initarea(cursor,x1,x1+textwidth("n"),10,10+textheight("n"));
 
    ///NUMELE
    while(kbhit())getch();
    while(caracter!=13){
        blinking_cursor(cursor);
        caracter=getch();
        c=caracter;
 
        m.nume[nrlitere++]=c;
 
        if(zona==1)
            grafica_operatii_matrice(m,matrice_de_afisat);
        else
            grafica_operatii_matrice(matrice_de_afisat,m);
 
        settextstyle(font_curent,HORIZ_DIR,4);
        cursor.x1+=textwidth("n");
        cursor.x2+=textwidth("n");
    }
    outtextxy(x1+80,80,"L:");
    outtextxy(x1+80,80+textheight("L")+10,"C:");
 
    ///LINII
    caracter=0;
    initarea(cursor,x1+80+textwidth("L:"),x1+80+textwidth("L: "),80,80+textheight("n"));
    blinking_cursor(cursor);
    caracter=getch();
    while(caracter<49 || caracter>57){
        caracter=getch();
    }
    c=caracter;
    m.L=atoi(p);
    cr[0]=c;
    cr[1]=0;
    outtextxy(x1+80+textwidth("L:"),80,cr);
    Sleep(500);
 
    ///COLOANE
    caracter=0;
    initarea(cursor,x1+80+textwidth("L:"),x1+80+textwidth("L: "),80+textheight("n"),80+2*textheight("n"));
    blinking_cursor(cursor);
    caracter=getch();
    while(caracter<49 || caracter>57){
        caracter=getch();
    }
    c=caracter;
    m.C=atoi(p);
    cr[0]=c;
    cr[1]=0;
    outtextxy(x1+80+textwidth("L:"),80+textheight("N")+10,cr);
    Sleep(500);
 
    ///ELEMENTE
    caracter=0;
    if (zona==1)
        grafica_operatii_matrice(m,matrice_de_afisat);
    else grafica_operatii_matrice(matrice_de_afisat,m);
 
    int font_determinator=max(m.C,m.L);
    ratiex=600/m.C;
    ratiey=600/m.L;
    if (zona==1)x1=30;
    else if (zona==2)x1=670;
    else if (zona==3)x1=1310;
 
    char element[3];
    settextstyle(font_curent,HORIZ_DIR,10-font_determinator);
    //settextstyle(font_curent,HORIZ_DIR,1);
    y1=70;
    for(int i=1;i<=m.L;i++){
        x1_aux=x1;
        for(int j=1;j<=m.C;j++){
            initarea(cursor,x1_aux,x1_aux+textwidth("n"),y1,y1+textheight("n"));
            caracter=0;
            while(caracter!=13){
                blinking_cursor(cursor);
                caracter=getch();
                if(caracter<48 || caracter>57)
                    continue;
                c=' '+caracter-' ';
 
                m.el[i][j]=m.el[i][j]*10+atoi(p);
                itoa(m.el[i][j],element,10);
 
                initarea(cursor,x1_aux+textwidth(element),x1_aux+textwidth(element)+textwidth("n"),y1,y1+textheight("n"));
 
                outtextxy(x1_aux,y1,element);
            }
            x1_aux+=ratiex;
        }
        y1+=ratiey;
    }
 
    m.nula=0;
    if (zona==1)
        grafica_operatii_matrice(m,matrice_de_afisat);
    else grafica_operatii_matrice(matrice_de_afisat,m);
}
 
void afisare2(matrice m,int zona,int x_culoare,int y_culoare,int culoare){
    int x1,y1=80,ratiex,ratiey,x1_aux;
    area zona_colorata;
    ratiex=600/m.C;
    ratiey=600/m.L;
    if (zona==1)x1=30;
    else if (zona==2)x1=670;
    else if (zona==3)x1=1310;
 
    char element[3];
    settextstyle(font_curent,HORIZ_DIR,10-max(m.C,m.L));
    for(int i=1;i<=m.L;i++){
        x1_aux=x1;
        for(int j=1;j<=m.C;j++){
            if(x_culoare==i && y_culoare==j){
                setcolor(culoare);
                itoa(m.el[i][j],element,10);
                outtextxy(x1_aux,y1,element);
                x1_aux+=ratiex;
                setcolor(15);
            }
            else{
                itoa(m.el[i][j],element,10);
                outtextxy(x1_aux,y1,element);
                x1_aux+=ratiex;
            }
        }
    cout<<'\n';
    y1+=ratiey;
    }
}
 
void citire_matrice_fisier(){
    while(fin>>matrice_salvate[nr_matrice].nume){
        fin>>matrice_salvate[nr_matrice].L>>matrice_salvate[nr_matrice].C;
        for(int i=1;i<=matrice_salvate[nr_matrice].L;i++)
            for(int j=1;j<=matrice_salvate[nr_matrice].C;j++)
                fin>>matrice_salvate[nr_matrice].el[i][j];
        matrice_salvate[nr_matrice].nula=0;
        nr_matrice++;
    }
}
 
void optiuni_m3(matrice m3){
    settextstyle(font_curent,HORIZ_DIR,4);
    outtextxy(1800,1000,"Nou");
    int x2,y2;
    memset(::m3.el,0,sizeof ::m3.el); ::m3.nula=1;
    while(true){
        if (ismouseclick(WM_LBUTTONDOWN)){
            getmouseclick(WM_LBUTTONDOWN,x2,y2);
            if(x2>1800&&x2<1800+textwidth("Nou")&&y2>1000&&y2<1000+textheight("Nou"))
                operatii_matrice();
            if (x2>50 && x2<200 && y2>980 && y2<1080)
                menu();
    }
    Sleep(100);
    }
 
}
 
void transpusa(int zona,matrice m,int zona1,matrice &m1){
    int culoare=5;
    int minim;
    m1.C=m.L;
    m1.L=m.C;
    minim=min(m1.C,m1.L);
    minim=1000/minim;
 
    for(int i=1; i<= m.L; i++){
        for(int j=1; j<=m.C; j++){
            m1.el[i][j]=(int)m.el[j][i];
            afisare2(m,zona,i,j,culoare);
            m1.nula=0;
            afisare2(m1,3,i,j,culoare);
            Sleep(minim);
        }
    }
    optiuni_m3(m3);
}
 
int produssumaDS(matrice m,int ok,int ok1)///ok=1 pentru sub,ok=2 de pe ds,ok=3 deasupra
{
            int psub=1,pdeas=1,p=1,s=0,ssub=0,sdeas=0,n;
            n=m.L;
            for(int i=1; i<=n; i++)
                for(int j=1; j<=n; j++)
                {
                    if(i+j>n+1)
                    {
                        psub*=m.el[i][j];
                        ssub+=m.el[i][j];
                    }
                    else if(i+j==n+1)
                    {
                        p*=m.el[i][j];
                        s+=m.el[i][j];
                    }
                    else
                    {
                        sdeas+=m.el[i][j];
                        pdeas*=m.el[i][j];
                    }
                }
            if(ok==1) return psub;
            else if(ok==2)return p;
            else if(ok==3)return pdeas;
            else if(ok1==1) return ssub;
            else if(ok1==2) return s;
            return sdeas;
}
 
int produssumaDP(matrice m,int ok,int ok1)///ok=1 pentru sub,ok=2 de pe dp,ok=3 deasupra
{
            int psub=1,pdeas=1,p=1,s=0,ssub=0,sdeas=0;
            for(int i=1; i<=m.L; i++)
                for(int j=1; j<=m.C; j++)
                {
                    if(i>j)
                    {
                        psub*=m.el[i][j];
                        ssub+=m.el[i][j];
                    }
                    else if(i==j)
                    {
                        p*=m.el[i][j];
                        s+=m.el[i][j];
                    }
                    else
                    {
                        sdeas+=m.el[i][j];
                        pdeas*=m.el[i][j];
                    }
                }
            if(ok==1) return psub;
            else if(ok==2)return p;
            else if(ok==3)return pdeas;
            else if(ok1==1) return ssub;
            else if(ok1==2) return s;
            return sdeas;
}