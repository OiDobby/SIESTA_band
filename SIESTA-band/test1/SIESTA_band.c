#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
double*** make_3D_array(double ***array_3D,double a,double b,double c){
 int x,y;
  array_3D = (double ***)calloc(a, sizeof(double **));
   for(x=0;x<a;x++) {
    array_3D[x] = (double **)calloc(b, sizeof(double *));
    for(y=0;y<b;y++) {
     array_3D[x][y] = (double *)calloc(c, sizeof(double));
    }
   }
  return array_3D;
}

double** make_2D_array(double **array_2D,double a,double b){
 int x;
  array_2D = (double **)calloc(a, sizeof(double *));
    for(x=0;x<a;x++) {
      array_2D[x] = (double *)calloc(b, sizeof(double));
      }
  return array_2D;
}

void del_3D_array(double ***array_3D,double a,double b,double c){
 int x,y,z;
 for(x=0;x<a;x++){
  for(y=0;y<b;y++){
   for(z=0;z<c;z++){
    array_3D[x][y][z]=0;
   }
  }
 }
 for(x=0;x<a;x++){
  for (y=0;y<b;y++){
   free(array_3D[x][y]);
  }
 free(array_3D[x]);
 }
 free(array_3D);
}

void del_2D_array(double **array_2D,double a,double b){
 int x,y;
 for(x=0;x<a;x++){
  for(y=0;y<b;y++){
   array_2D[x][y]=0;
  }
 }
 for(x=0;x<a;x++){
   free(array_2D[x]);
  }
 free(array_2D);
}

void band(double fermi_level){
 FILE *f1;				//band_siesta 
 FILE *f2;				//up-band_siesta
 FILE *f3;				//down-band_siesta
 FILE *f4;				//kpoint-path
 char s1,s2;
 int a,b,c,d,e,g,h,x,y,z;
 double i,j,k,l,m,n,o;
 double c1;				//I don't know what is this
 double a1,a2,a3;			//data sheet X Y Z
 double b1;				//data sheet new X
 int num_k;				//k point integration, delta k
 double DB[10][10],K_sym[20];		//data base
 char str[100];

 f1 = fopen("./band_siesta", "r");
  if (f1==NULL) {
   printf("Fail! Can't find the '**.bands' file in this directory\n");
  } 			//consider spin in calculation?

 for (a=1;a<=1;a++) {
  fscanf(f1,"%lf",&i);
  DB[1][a]=i;      //fermi level
 }
 for (a=1;a<=2;a++) {
  fscanf(f1,"%lf",&i);
  DB[2][a]=i;      //minimum and maximum lengthof the path in k-space
 }
 for (a=1;a<=2;a++) {
  fscanf(f1,"%lf",&i);
  DB[3][a]=i;      //minimum and maximum eigenvalues
 }
 for (a=1;a<=3;a++) {
  fscanf(f1,"%lf",&i);
  DB[4][a]=i;      //bnd_num, spin_polarization, kpoint_num
 }

 a1=DB[4][1]; 		//band number = X axis (first number in array)
 a2=DB[4][3];		//kpoint number = Y axis
 a3=DB[4][2]; 		//spin polarization = Z axis

 b1 = a3*a1;
 
 double ***A;
  A = make_3D_array(A,a1+10,a2+10,a3+10); //A[a1][a2][a3]


 if (a3==1){
  for (b=1;b<=a2;b++) {
   for (a=0;a<=a1;a++) {
    fscanf(f1,"%lf",&n);
    A[a][b][1]=n;	
   }
  }
 }			//make data sheets (scaning band# and values)

 if (a3==2){
  for (b=1;b<=a2;b++) {
   for (a=0;a<=a1;a++) {
    fscanf(f1,"%lf",&n);
    A[a][b][1]=n;
   }
   for (a=1;a<=a1;a++){
    fscanf(f1,"%lf",&n);
    A[a][b][2]=n;
    A[0][b][2]=A[0][b][1];
   }
  }
 }                      //make data sheets (scaning band# and values)


 fscanf(f1,"%d",&num_k);
 for(a=1;a<=num_k;a++) {
  fscanf(f1,"%lf",&m);
  K_sym[a]=m;
 }

 fclose(f1);
 
 f2=fopen("./up-band_siesta", "wt");
  for (b=1;b<=a2;b++) {
   for (a=0;a<=a1;a++) {
     i=A[a][b][1];
     fprintf(f2,"%lf    ",i);
   }
  fprintf(f2,"\n");
  }			//writing file
 fclose(f2);

 if (a3==2) {
  f2=fopen("./down-band_siesta", "wt");
  for (b=1;b<=a2;b++) {
   for (a=0;a<=a1;a++) {
     i=A[a][b][2];
     fprintf(f3,"%lf    ",i);
   }
  fprintf(f3,"\n");
  }                     //writing file
  fclose(f3);
 }

 f4=fopen("./kpnt-path", "wt");
  for (a=1;a<=num_k;a++) {
   i=K_sym[a];
   fprintf(f4,"%lf     \n",i);
  }
 fclose(f4);

 printf("band program finished\n");

 del_3D_array(A,a1+10,a2+10,a3+10); //A[a1][a2][a3]

}

int main(void){
 FILE *f1;		//band_siesta
 FILE *f3,*f4;		//up-band_siesta, down-band_siesta
 FILE *fg1;             //gnuband
 char s1;
 char *name1,*name2;    //eps file names
 int a,b,c,d,e,g,h,x,y,z;
 double i,j,k,l,m,n,o;
 int b1,b2,b3;          //checking calculation option
 double c1,c2;		//option variable
 double max_band,min_band,m_band,val_band;		//metalic checking
 int metal;
 double VBM,CBM;	//valance band maximun, conduction band minimum
 int bnd_num,kpoint_num,spin;
 double fermi_level,re_fermi;
 double DB_band[10][10]; 


 system("cp **.bands ./band_siesta");
 system("sed -i \"s/'/ /g\" band_siesta");

 name1 = (char *)calloc(10, sizeof(char));

 printf("We will make gnuband file. Please enter eps file's name.\n");
 scanf("%s",name1);

 f1 = fopen("./band_siesta", "r");
 for (a=1;a<=1;a++) {
  fscanf(f1,"%lf",&i);
  DB_band[1][a]=i;	//fermi level
 }
 for (a=1;a<=2;a++) {
  fscanf(f1,"%lf",&i);
  DB_band[2][a]=i;	//minimum and maximum lengthof the path in k-space
 }
 for (a=1;a<=2;a++) {
  fscanf(f1,"%lf",&i);
  DB_band[3][a]=i;	//minimum and maximum eigenvalues
 }
 for (a=1;a<=3;a++) {
  fscanf(f1,"%lf",&i);
  DB_band[4][a]=i;	//bnd_num, spin_polarization, kpoint_num
 }
 fseek(f1,0L,SEEK_SET);
 fclose(f1);

 fermi_level = DB_band[1][1];
 bnd_num = DB_band[4][1];
 spin = DB_band[4][2];
 kpoint_num = DB_band[4][3];


 printf("band program start\n");
 band(fermi_level);

 printf("checking properties\n");

 double ***eigenval, ***band_gap;
 eigenval = make_3D_array(eigenval,bnd_num+10,kpoint_num+10,5);
 band_gap = make_3D_array(band_gap,bnd_num+10,5,5);


 f3=fopen("./up-band_siesta", "r"); 
 for (b=1;b<=kpoint_num;b++) {
  for (a=0;a<=bnd_num;a++) {
   fscanf(f3,"%lf",&i);
   eigenval[a][b][1]=i;
  }
 }
 fclose(f3);

 if(spin==2){
  f4=fopen("./down-band_siesta", "r");        
  for (b=1;b<=kpoint_num;b++) {
   for (a=0;a<=bnd_num;a++) {
    fscanf(f4,"%lf",&i);
    eigenval[a][b][2]=i;
   }
  }
  fclose(f4);
 }

//file reading

 
 if(spin==2){
  printf("***	spin effect : O		***\n");
 }
 else{
  printf("***	spin effect : X		***\n");
 }

//spin effecj check


 for(c=1;c<=spin;c++){
  for(a=1;a<=bnd_num;a++){
   m_band = eigenval[a][1][c];
   for(b=2;b<=kpoint_num;b++){
    val_band = eigenval[a][b][c];
    if(val_band>=m_band){
     m_band = val_band;
    }
    else{
     m_band = m_band;
    }
   }
   max_band = m_band;
   m_band = eigenval[a][1][c];
   for(b=2;b<=kpoint_num;b++){
    val_band = eigenval[a][b][c];
    if(val_band<=m_band){
     m_band = val_band;
    }
    else{
     m_band = m_band;
    }
   }
   min_band = m_band;
   
   band_gap[a][2][c] = max_band;
   band_gap[a][1][c] = min_band;

// printf("%lf,%lf,%lf\n",min_band,max_band,fermi_level);
  }
 }


 for(c=1;c<=spin;c++){
  for(a=1;a<=bnd_num;a++){
   if(band_gap[a][1][c]<fermi_level){
    if(band_gap[a][2][c]<fermi_level){
     metal = 0;
    }
    if(band_gap[a][2][c]>=fermi_level){
     metal = 1;
    }
   }
   if(band_gap[a][2][c]<=fermi_level){
    metal = 1;
   }
   else{
    metal = 0;
   }
  }
 }

 if(metal==1){
  printf("***	band gap : X		***\n");
 }
 else{
  printf("***	band gap : O		***\n");
 }

  for(c=1;c<=spin;c++){
   for(a=1;a<=bnd_num;a++){
    if(eigenval[a][1][c]-fermi_level<=0){
     VBM = band_gap[a][2][c];
     if(band_gap[a][2][c]-VBM>=0){
      VBM = band_gap[a][2][c];
      x=a;
     }
     else{
      VBM = VBM;
     }
    } 
    if(eigenval[a][1][c]-fermi_level>=0){
     CBM = band_gap[x+1][1][c];
     if(band_gap[a][1][c]-CBM<=0){
      CBM = band_gap[a][1][c];
     }
     else{
      CBM = CBM;
     }
    }
   }
  }

 printf("***	VBM : %lf		***\n",VBM);
 printf("***	CBM : %lf		***\n",CBM);

 re_fermi = (VBM+CBM)/2;

 if(metal==0){
  fermi_level = re_fermi;
 }
 if(metal==1){
  fermi_level = fermi_level;
 } 

 printf("***	fermi : %lf	***\n",fermi_level);
 printf("write gnu files\n");

 fg1=fopen("./gnuband", "wt");
  fprintf(fg1,"set term postscript eps color enhanced\n");
  fprintf(fg1,"set output \"%s-band.eps\"\n",name1);
  fprintf(fg1,"set nokey\n");
  fprintf(fg1,"set yr [-4:4]\n");
  fprintf(fg1,"set xtic (\"{/Symbol G}\"0, \"K\"0.745356, \"M\"1.118034, \"{/Symbol G}\"1.618034)\n");
  fprintf(fg1,"set mytic 2\n");
  fprintf(fg1,"set ytic 1\n");
  fprintf(fg1,"set grid xtics lw 4\n");
  fprintf(fg1,"set xzeroaxis lw 4\n");
  fprintf(fg1,"set xtic font \"Times Roman,30\"\n");
  fprintf(fg1,"set ytic font \"Times Roman,30\"\n");
  fprintf(fg1,"set tics font \"Times Roman,30\"\n");
  fprintf(fg1,"set tmargin 3\n");
  fprintf(fg1,"set bmargin 6\n");
  fprintf(fg1,"set lmargin 10\n");
  fprintf(fg1,"set rmargin 2\n");
  fprintf(fg1,"set xlabel 'Wave Vector' font \"Times Roman, 36\" offset 0,-1.5,0\n");
  fprintf(fg1,"set ylabel 'E-E_F (eV)' font \"Times Roman, 36\" offset -2,0,0\n");
  fprintf(fg1,"plot    \"up-band_siesta\" using ($1):($2+%lf) w l  lw 4 lt 1 lc 0 ,\\\n",fermi_level*-1);
  for (a=3;a<bnd_num+1;a++) {
   fprintf(fg1,"        \"up-band_siesta\" using ($1):($%d+%lf) w l  lw 4 lt 1 lc 0 ,\\\n",a,fermi_level*-1);
  }
  fprintf(fg1,"        \"up-band_siesta\" using ($1):($%d+%lf) w l  lw 4 lt 1 lc 0 \n",bnd_num+1,fermi_level*-1);
  if(spin==2){
  fprintf(fg1,"#plot    \"down-band_siesta\" using ($1):($2+%lf) w l  lw 4 lt 1 lc 1 ,\\\n",fermi_level);
  for (a=3;a<bnd_num+1;a++) {
   fprintf(fg1,"        \"down-band_siesta\" using ($1):($%d+%lf) w l  lw 4 lt 1 lc 1 ,\\\n",a,fermi_level*-1);
  }
  fprintf(fg1,"        \"down-band_siesta\" using ($1):($%d+%lf) w l  lw 4 lt 1 lc 1 \n",bnd_num+1,fermi_level*-1);
  }

  fclose(fg1);


 system("gnuplot gnuband");

 printf("Finish!\n");

return 0;

}
