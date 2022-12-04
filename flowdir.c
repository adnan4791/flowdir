#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define sqrt2  1.41421
int x_off[9] = {-1,0,1,-1,0,1,-1,0,1};
int y_off[9] = {-1,-1,-1,0,0,0,1,1,1};

float **dem;
int **direction;
int map_size;
int ncols,cols;
int nrows; 

int allocate_model(int x_size, int y_size);

int bacafile_dem(char *fn) {
    FILE *fp;
    char *filename = fn;
    int buffersize = 4096*4;
    char *line = (char *) malloc(buffersize*sizeof(char));
    size_t lines=0;
    char *token;
    fp = fopen(filename,"r");
    if(!fp) {
        printf("file tidak dapat ditemukan\n");
        exit(EXIT_FAILURE);
    }
    fgets(line,256,fp);
    token = strtok(line," ");
    token = strtok(NULL," ");
    //printf("%s\n",token);
    ncols = atoi(token);
    printf("ncols : %d\n",ncols);
    fgets(line,256,fp);
    token = strtok(line," ");
    token = strtok(NULL," ");
    nrows = atoi(token);
    printf("nrow : %d\n",nrows);
    allocate_model(ncols,nrows);
    fgets(line,256,fp);
    printf("%s\n",line);
    fgets(line,256,fp);
    printf("%s\n",line);
    fgets(line,256,fp);
    printf("%s\n",line);
    fgets(line,256,fp);
    printf("%s\n",line);
    while(fgets(line,buffersize,fp)) {
        cols = 0;
        char *data_tok;
        data_tok = strtok(line," ");
        dem[lines][cols] = atof(data_tok);
        while(*data_tok != '\n') {
            dem[lines][cols] = atof(data_tok);
            data_tok = strtok(NULL," ");
            cols++;
        } 
        lines++;
    }
    fclose(fp);
    free(line);
    printf("ukuran data  : %d baris * %d kolom \n",lines,cols);
}

int allocate_model(int x_size, int y_size) {
    int status = 0;
    dem = (float **) malloc(y_size * sizeof(float *));
    if (!dem) {
        status = 1;
    }
        
    direction = (int **) malloc(y_size * sizeof(int *));
    if (!direction) {
        status++;
    }
    for(int i=0;i<y_size;i++) {
        dem[i] = (float *) malloc(x_size*sizeof(float));
        if (!dem[i]) {
            status++;
        }
        direction[i] = (int *) malloc(x_size*sizeof(int));
        if (!direction[i]) {status++;}
    }
    return status;
}

void deallocate_model(int x_size, int y_size) {
    for(int i=0;i<y_size;i++) {
        free(dem[i]);
        free(direction[i]);
    }
    free(dem);
    free(direction);
}

int flow_dir(float *d) {
    float max_value = d[0];
    int max_index = 4;
    for(int i=1;i<7;i++) {
        if(d[i] < 0) {
            continue;
        } else
        if (max_value < d[i]) {
            max_value = d[i];
            max_index = i;
        }
    }
    return max_index;
}

void d8_y(int x, int ny) {
     for(int y=0;y<ny;y++) {
        float slop[9] = {0.0,0.0,0.0,
                         0.0,0.0,0.0,
                         0.0,0.0,0.0};
        if ((x==0) && (y==0)) {
            slop[0] = -1;
            slop[1] = -1;
            slop[2] = -1;
            slop[3] = -1;
            //slop[4] = 0.0; //its me !        
            slop[5] = dem[x][y] - dem[x+x_off[5]][y+y_off[5]];
            slop[6] = -1;
            slop[7] = dem[x][y] - dem[x+x_off[7]][y+y_off[7]];
            slop[8] = (dem[x][y] - dem[x+x_off[8]][y+y_off[8]])/sqrt2;
            direction[x][y] = flow_dir(slop);
        } else if ((x==0) && (y==1666)) {
            slop[0] = -1;
            slop[1] = dem[x][y] - dem[x+x_off[1]][y+y_off[1]];
            slop[2] = (dem[x][y] - dem[x+x_off[2]][y+y_off[2]])/sqrt2;
            slop[3] = -1;
            slop[4] = -1;
            slop[5] = dem[x][y] - dem[x+x_off[5]][y+y_off[5]];
            slop[6] = -1;
            slop[7] = -1;
            slop[8] = -1;
            direction[x][y] = flow_dir(slop);
        } else if ((x==1666) && (y==0)) {
            slop[0] = -1;
            slop[1] = -1;
            slop[2] = -1;
            slop[3] = dem[x][y] - dem[x+x_off[3]][y+y_off[3]];
            slop[4] = -1;
            slop[5] = -1;
            slop[6] = dem[x][y] - dem[x+x_off[6]][y+y_off[6]];
            slop[7] = dem[x][y] - dem[x+x_off[7]][y+y_off[7]];
            slop[8] = -1;
            direction[x][y] = flow_dir(slop);  
        } else if ((x==1666) && (y==1666)) {
            slop[0] = (dem[x][y] - dem[x+x_off[0]][y+y_off[0]])/sqrt2;
            slop[1] = dem[x][y] - dem[x+x_off[1]][y+y_off[1]];
            slop[2] = -1;
            slop[3] = dem[x][y] - dem[x+x_off[3]][y+y_off[3]];
            slop[4] = -1;
            slop[5] = -1;
            slop[6] = -1;
            slop[7] = -1;
            slop[8] =-1;
            direction[x][y] = flow_dir(slop);
        } else if (x==0){
            slop[0] = -1;
            slop[1] = dem[x][y] - dem[x+x_off[1]][y+y_off[1]];
            slop[2] = (dem[x][y] - dem[x+x_off[2]][y+y_off[2]])/sqrt2;
            slop[3] = -1;
            slop[4] = -1;
            slop[5] = dem[x][y] - dem[x+x_off[5]][y+y_off[5]];       
            slop[6] = -1;
            slop[7] = dem[x][y] - dem[x+x_off[7]][y+y_off[7]];
            slop[8] = (dem[x][y] - dem[x+x_off[7]][y+y_off[7]])/sqrt2;
            direction[x][y] = flow_dir(slop);
        } else if(y==0) {
            slop[0] = -1;
            slop[1] = -1;
            slop[2] = -1;
            slop[3] = dem[x][y] - dem[x+x_off[3]][y+y_off[3]];
            slop[4] = -1;
            slop[5] = dem[x][y] - dem[x+x_off[5]][y+y_off[5]];
            slop[6] = (dem[x][y] - dem[x+x_off[6]][y+y_off[6]])/sqrt2;
            slop[7] = dem[x][y] - dem[x+x_off[7]][y+y_off[7]];
            slop[8] = (dem[x][y] - dem[x+x_off[8]][y+y_off[8]])/sqrt2;
            direction[x][y] = flow_dir(slop);
        } else if (x == 1666) { // assume y != 0, y != 1666
            slop[0] = (dem[x][y] - dem[x+x_off[0]][y+y_off[0]])/sqrt2; //assume y != 0 , y != 1666
            slop[1] = dem[x][y] - dem[x+x_off[1]][y+y_off[1]];
            slop[2] = -1;
            slop[3] = dem[x][y] - dem[x+x_off[3]][y+y_off[3]];
            slop[4] = -1;
            slop[5] = -1;
            slop[6] = dem[x][y] - dem[x+x_off[6]][y+y_off[6]];
            slop[7] = dem[x][y] - dem[x+x_off[7]][y+y_off[7]];
            slop[8] = -1;
            direction[x][y] = flow_dir(slop);
        } else if (y == 1666) {
            slop[0] = (dem[x][y] - dem[x+x_off[0]][y+y_off[0]])/sqrt2;
            slop[1] = dem[x][y] - dem[x+x_off[1]][y+y_off[1]];
            slop[2] = (dem[x][y] - dem[x+x_off[2]][y+y_off[2]])/sqrt2;
            slop[3] = dem[x][y] - dem[x+x_off[3]][y+y_off[3]];
            slop[4] = -1;
            slop[5] = dem[x][y] - dem[x+x_off[5]][y+y_off[5]];
            slop[6] = -1;
            slop[7] = -1;
            slop[8] = -1;
            direction[x][y] = flow_dir(slop);
        } else {
            for(int i=0;i<9;i++) {
                if (i==4) {
                    slop[i] = -1;
                }
                slop[i] = dem[x][y] - dem[x+x_off[i]][y+y_off[i]];
                if (i == 0 || i == 2 || i == 6 || i ==8) {
                   slop[i] = slop[i]/sqrt2;
                }
            }
            direction[x][y] = flow_dir(slop);
        }
     }    
}

void flowdir() {
    for(int x=0;x<ncols;x++) {
        for(int y=0;y<nrows;y++) {
            float slope[9];
            if (x == 0 && y == 0) {
               slope[0] = -1.0f;
               slope[1] = -1.0f;
               slope[2] = -1.0f;
               slope[3] = -1.0f;
               slope[4] = 0.0f;
               slope[5] = dem[x][y] - dem[x+x_off[5]][y+y_off[5]];
               slope[6] = -1.0f;
               slope[7] = dem[x][y] - dem[x+x_off[7]][y+y_off[7]];
               slope[8] = (dem[x][y] - dem[x+x_off[8]][y+y_off[8]])/sqrt2;
            } else if (x == 0 && y == 1666) {
               slope[0] = -1.0f;
               slope[1] = dem[x][y] - dem[x+x_off[1]][y+y_off[1]];
               slope[2] = (dem[x][y] - dem[x+x_off[2]][y+y_off[2]])/sqrt2;
               slope[3] = -1.0f;
               slope[4] = 0.0f;
               slope[5] = dem[x][y] - dem[x+x_off[5]][y+y_off[5]];
               slope[6] = -1.0f;
               slope[7] = -1.0;
               slope[8] = -1.0f; 
            } else if (x == 1666 && y == 0) {
               slope[0] = -1.0f;
               slope[1] = -1.0f;
               slope[2] = -1.0f;
               slope[3] = dem[x][y] - dem[x+x_off[3]][y+y_off[3]];
               slope[4] = 0.0f;
               slope[5] = -1.0f;
               slope[6] = (dem[x][y] - dem[x+x_off[6]][y+y_off[6]])/sqrt2;
               slope[7] = dem[x][y] - dem[x+x_off[7]][y+y_off[7]];
               slope[8] = -1.0f; 
            } else if (x== 1666 && y == 1666) {
               slope[0] = (dem[x][y] - dem[x+x_off[0]][y+y_off[0]])/sqrt2;
               slope[1] = dem[x][y] - dem[x+x_off[1]][y+y_off[1]];
               slope[2] = -1.0f;
               slope[3] = dem[x][y] - dem[x+x_off[3]][y+y_off[3]];
               slope[4] = 0.0f;
               slope[5] = -1.0f;
               slope[6] = -1.0f;
               slope[7] = -1.0f;
               slope[8] = -1.0f;  
            } else if (y == 0) {
               slope[0] = -1.0f;
               slope[1] = -1.0f;
               slope[2] = -1.0f;
               slope[3] = dem[x][y] - dem[x+x_off[3]][y+y_off[3]];
               slope[4] = 0.0f;
               slope[5] = dem[x][y] - dem[x+x_off[5]][y+y_off[5]];
               slope[6] = (dem[x][y] - dem[x+x_off[6]][y+y_off[6]])/sqrt2;
               slope[7] = dem[x][y] - dem[x+x_off[7]][y+y_off[7]];
               slope[8] = (dem[x][y] - dem[x+x_off[8]][y+y_off[8]])/sqrt2;  
            } else if (y == 1666) {
               slope[0] = (dem[x][y] - dem[x+x_off[0]][y+y_off[0]])/sqrt2;
               slope[1] = dem[x][y] - dem[x+x_off[1]][y+y_off[1]];
               slope[2] = (dem[x][y] - dem[x+x_off[2]][y+y_off[2]])/sqrt2;
               slope[3] = dem[x][y] - dem[x+x_off[3]][y+y_off[3]];
               slope[4] = 0.0f;
               slope[5] = dem[x][y] - dem[x+x_off[5]][y+y_off[5]];
               slope[6] = -1.0f;
               slope[7] = -1.0f;
               slope[8] = -1.0f;
            } else if (x == 0) {
               slope[0] = -1.0f;
               slope[1] = dem[x][y] - dem[x+x_off[1]][y+y_off[1]];
               slope[2] = (dem[x][y] - dem[x+x_off[2]][y+y_off[2]])/sqrt2;
               slope[3] = -1.0f;
               slope[4] = 0.0f;
               slope[5] = dem[x][y] - dem[x+x_off[5]][y+y_off[5]];
               slope[6] = -1.0f;
               slope[7] = dem[x][y] - dem[x+x_off[7]][y+y_off[7]];
               slope[8] = (dem[x][y] - dem[x+x_off[8]][y+y_off[8]])/sqrt2;
            } else if (x == 1666) {
               slope[0] = (dem[x][y] - dem[x+x_off[0]][y+y_off[0]])/sqrt2;
               slope[1] = dem[x][y] - dem[x+x_off[1]][y+y_off[1]];
               slope[2] = -1.0f;
               slope[3] = dem[x][y] - dem[x+x_off[3]][y+y_off[3]];
               slope[4] = 0.0f;
               slope[5] = -1.0f;
               slope[6] = (dem[x][y] - dem[x+x_off[6]][y+y_off[6]])/sqrt2;
               slope[7] = dem[x][y] - dem[x+x_off[7]][y+y_off[7]];
               slope[8] = -1.0f;
            } else {
               for(int i=0;i<9;i++) {
                  if (i == 4) {
                    slope[4] = 0.0f;
                  } else if ((i == 0) || (i == 2) || (i == 6) || (i == 8))
                    slope[i] = (dem[x][y] - dem[x+x_off[i]][y+y_off[i]])/sqrt2;
                  else
                    slope[i] = dem[x][y] - dem[x+x_off[i]][y+y_off[i]];
               }                 
            }
            direction[x][y] = flow_dir(slope);
        }
    }
}

void flowdirection(int nx, int ny) {
    for(int x=0;x<nx;x++)
       d8_y(x,ny);
}

void simpandata(char *fn) {
    FILE *fp;
    int buffersize = 5*4096;
    char *buffer = (char *) malloc(buffersize*sizeof(char));
    char *nama_file=fn;
    fp = fopen(nama_file,"w");
    for(int y=0;y<nrows;y++) {
        for(int x=0;x<ncols;x++) {
            sprintf(&(buffer[x*10]),"%d, ",direction[y][x]);
            fputs(buffer,fp);
        }
        //fputs(buffer,fp);
        //puts(buffer);
    }
 /*   
    char data[10];
    itoa(ncols,data,10);
    fputs(data,fp);
    itoa(nrows,data,10);
    fputs(data,fp);
    for(int y=0;y<nrows;y++) {

    }
    */
    free(buffer);
    fclose(fp);
}

int main(int argc, char *args[]) {
    char nama_file[50];
    char out_file[50];
    int status = 0;
    strcpy(nama_file,args[1]);
    printf("Nama file : %s\n",nama_file);
    //status = allocate_model(ncols,nrows);
    if (status > 0) {
      printf("ada kegagalan alokasi memory\n");
      return -1;
    }
    bacafile_dem(nama_file);
    flowdirection(nrows,ncols);
    //flowdir();
    strcpy(out_file,"direction.txt");
    simpandata(out_file);
    deallocate_model(ncols,nrows);
    return 0;
}