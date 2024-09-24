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
 double DB[10][10];			//data base
 char str[100];
 char str_read[100];
 char *K_sym[100];

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
   }
   A[0][b][2]=A[0][b][1];
  }
 }                      //make data sheets (scaning band# and values)


 fscanf(f1,"%d",&num_k);
 for(a=1;a<=num_k+1;a++) {
  fgets(str_read,100,f1);
  l=strlen(str_read);
  if (l > 0){
   char* k_sym = (char*)malloc(sizeof(char)*(l+1));
   strcpy(k_sym,str_read);
   K_sym[a] = k_sym;
  }
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
  f3=fopen("./down-band_siesta", "wt");
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
  fprintf(f4,"%d     \n",num_k);
  for (a=1;a<=num_k+1;a++) {
   fprintf(f4,"%s",K_sym[a]);
  }
 fclose(f4);

 printf("band program finished\n");

 del_3D_array(A,a1+10,a2+10,a3+10); //A[a1][a2][a3]

}

void gnuband(FILE *filename,char *material_name,double fermi_level,int bnd_num,int spin){

 FILE *f1;
 int a,b;
 int num_k;
 double k_sym[100];
 double i;
 char str_read[100];

  f1 = fopen("./kpnt-path", "r");
  fscanf(f1,"%d",&num_k);
  for(a=1;a<=num_k;a++) {
   fscanf(f1,"%lf",&i);
   fgets(str_read,100,f1);
   k_sym[a]=i;
  }
  fclose(f1);


  filename=fopen("./gnuband", "wt");
   fprintf(filename,"set term postscript eps color enhanced\n");
   fprintf(filename,"set output \"%s-band.eps\"\n",material_name);
   fprintf(filename,"set size 0.70,1.00\n");
   fprintf(filename,"set size ratio 1.0\n");
   fprintf(filename,"set multiplot layout 1,1\n");
   fprintf(filename,"set nokey\n");
   fprintf(filename,"set yr [-4:4]\n");
   fprintf(filename,"set xtic (\"{/Symbol G}\"0,");
   if (b>2){
    for(b=2;b<num_k;b++){
      fprintf(filename," \"{/Symbol G}\"%lf,",k_sym[b]);
     }
    }
   fprintf(filename," \"{/Symbol G}\"%lf)\n",k_sym[num_k]);
   fprintf(filename,"set mxtic 2\n");
   fprintf(filename,"set ytic 1\n");
   fprintf(filename,"set mytic 2\n");
   fprintf(filename,"set grid xtics lw 4\n");
   fprintf(filename,"set xzeroaxis lw 4\n");
   fprintf(filename,"set xtic font \"Arial,30\"\n");
   fprintf(filename,"set ytic font \"Arial,30\"\n");
   fprintf(filename,"set tics font \"Arial,30\"\n");
   fprintf(filename,"set tmargin 3\n");
   fprintf(filename,"set bmargin 6\n");
   fprintf(filename,"set lmargin 12\n");
   fprintf(filename,"set rmargin 6\n");
   fprintf(filename,"set xlabel 'wave vector' font \"Arial, 36\" offset 0,-1.5,0\n");
   fprintf(filename,"set ylabel 'E-E_F (eV)' font \"Arial, 36\" offset -2,0,0\n");
   fprintf(filename,"set origin -0.15,0.0\n");
   fprintf(filename,"plot    \"up-band_siesta\" using ($1):($2+%lf) w l  lw 4 lt 1 lc 0 ,\\\n",fermi_level*-1);
   for (a=3;a<bnd_num+1;a++) {
    fprintf(filename,"        \"up-band_siesta\" using ($1):($%d+%lf) w l  lw 4 lt 1 lc 0 ,\\\n",a,fermi_level*-1);
   }
   fprintf(filename,"        \"up-band_siesta\" using ($1):($%d+%lf) w l  lw 4 lt 1 lc 0 \n",bnd_num+1,fermi_level*-1);
   if(spin==2){
   fprintf(filename,"#plot    \"down-band_siesta\" using ($1):($2+%lf) w l  lw 4 lt 1 lc 1 ,\\\n",fermi_level);
   for (a=3;a<bnd_num+1;a++) {
    fprintf(filename,"        \"down-band_siesta\" using ($1):($%d+%lf) w l  lw 4 lt 1 lc 1 ,\\\n",a,fermi_level*-1);
   }
   fprintf(filename,"        \"down-band_siesta\" using ($1):($%d+%lf) w l  lw 4 lt 1 lc 1 \n",bnd_num+1,fermi_level*-1);
   }
  fclose(filename);

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

//spin moment check


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

 gnuband(fg1,name1,fermi_level,bnd_num,spin);
 
 system("gnuplot gnuband");

 printf("Finish!\n");

return 0;

}
