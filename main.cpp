#include<iostream>
#include<vector>
#include<cmath>
using namespace std;
#define f2(i,x) for((i)=0; (i)<(x); (i)++)
struct cube{
    double x,y,z,lx,ly,lz,col_r,col_g,col_b; // x/y/z 위치, x/y/z 크기, 색상 코드
    double rendered_x[2][3],rendered_y[2][3],rendered_z[2][3]; // x/y/z방향 색상 코드 - 양쪽 면 각각, *0이 -에서 오는 방향*
}; vector<cube> cb; // 각 cube 정보 저장
int aa=1,h=768,w=1024,bgc[3]; // aa는 anti-aliasing, h는 높이, w는 너비
double cam_x,cam_y,cam_z,cam_h,cam_v; // 카메라 직교 좌표 위치, 극좌표 방향 저장
double intensity[3],light[2]; // intensity 에는 x, y, z 방향의 밝기를, light 에는 빛의 극좌표 방향을 저장
double camera[1920*2][1280*2][3]; // 출력할 bmp 파일 저장
// functions
void init(){ // 초기화 : 큐브 1개 생성 및 위치 지정, 카메라 위치 및 방향, 빛의 방향 설정
    cout<<"Initializing..."<<endl;
    cb.clear(); cb.push_back({-1,-1,-1,2,2,2,255,255,255});
    cam_x=cam_y=cam_z=10;
    cam_h=3.93; cam_v=-0.615;
    light[0]=4; light[1]=-1;
    intensity[0]=cos(light[0])*cos(light[1]); intensity[1]=sin(light[0])*cos(light[1]); intensity[2]=sin(light[1]);
    {
        cout<<"Running Burenda 1.0"<<endl;
        cout<<"============================================================================================"<<endl;
        cout<<"+                                                                                          +"<<endl;
        cout<<"+                                       < Burenda >                                        +"<<endl;
        cout<<"+                                         v. -1.0                                          +"<<endl;
        cout<<"+                                                                                          +"<<endl;
        cout<<"+                                     Computer Science                                     +"<<endl;
        cout<<"+                                 Performance Test Project                                 +"<<endl;
        cout<<"+                                                                                          +"<<endl;
        cout<<"+                                   2022. 11. / gs22059                                    +"<<endl;
        cout<<"+                                                                                          +"<<endl;
        cout<<"=============================================================================titlescreen===="<<endl;
    } // title screen
}
void add(){ // 큐브 추가
    string s; cin>>s;
    if(s=="cube"){
        double x,y,z,lx,ly,lz,col_r,col_g,col_b;
        cin>>x>>y>>z>>lx>>ly>>lz>>col_r>>col_g>>col_b;
        cb.push_back({x,y,z,lx,ly,lz,col_r,col_g,col_b});
        cout<<"add shape done!"<<endl;
    }
    else cout<<"error: unknown shape"<<endl;
}
void relight(){ // 각 큐브의 면별로 렌더링한 사진에 나올 색 계산
    int n=(int)cb.size(),i;
    cout<<"relighting..."<<endl;
    f2(i,n){
        if(intensity[0]>0) {
            cb[i].rendered_x[0][0] = cb[i].col_r * intensity[0];
            cb[i].rendered_x[0][1] = cb[i].col_g * intensity[0];
            cb[i].rendered_x[0][2] = cb[i].col_b * intensity[0];
            cb[i].rendered_x[1][0] = cb[i].col_r * intensity[0]*0.2;
            cb[i].rendered_x[1][1] = cb[i].col_g * intensity[0]*0.2;
            cb[i].rendered_x[1][2] = cb[i].col_b * intensity[0]*0.2;
        }
        else{
            cb[i].rendered_x[0][0] = cb[i].col_r * -intensity[0]*0.2;
            cb[i].rendered_x[0][1] = cb[i].col_g * -intensity[0]*0.2;
            cb[i].rendered_x[0][2] = cb[i].col_b * -intensity[0]*0.2;
            cb[i].rendered_x[1][0] = cb[i].col_r * -intensity[0];
            cb[i].rendered_x[1][1] = cb[i].col_g * -intensity[0];
            cb[i].rendered_x[1][2] = cb[i].col_b * -intensity[0];
        }
        if(intensity[1]>0) {
            cb[i].rendered_y[0][0] = cb[i].col_r * intensity[1];
            cb[i].rendered_y[0][1] = cb[i].col_g * intensity[1];
            cb[i].rendered_y[0][2] = cb[i].col_b * intensity[1];
            cb[i].rendered_y[1][0] = cb[i].col_r * intensity[1]*0.2;
            cb[i].rendered_y[1][1] = cb[i].col_g * intensity[1]*0.2;
            cb[i].rendered_y[1][2] = cb[i].col_b * intensity[1]*0.2;
        }
        else{
            cb[i].rendered_y[0][0] = cb[i].col_r * -intensity[1]*0.2;
            cb[i].rendered_y[0][1] = cb[i].col_g * -intensity[1]*0.2;
            cb[i].rendered_y[0][2] = cb[i].col_b * -intensity[1]*0.2;
            cb[i].rendered_y[1][0] = cb[i].col_r * -intensity[1];
            cb[i].rendered_y[1][1] = cb[i].col_g * -intensity[1];
            cb[i].rendered_y[1][2] = cb[i].col_b * -intensity[1];
        }
        if(intensity[2]>0) {
            cb[i].rendered_z[0][0] = cb[i].col_r * intensity[2];
            cb[i].rendered_z[0][1] = cb[i].col_g * intensity[2];
            cb[i].rendered_z[0][2] = cb[i].col_b * intensity[2];
            cb[i].rendered_z[1][0] = cb[i].col_r * intensity[2]*0.2;
            cb[i].rendered_z[1][1] = cb[i].col_g * intensity[2]*0.2;
            cb[i].rendered_z[1][2] = cb[i].col_b * intensity[2]*0.2;
        }
        else{
            cb[i].rendered_z[0][0] = cb[i].col_r * -intensity[2]*0.2;
            cb[i].rendered_z[0][1] = cb[i].col_g * -intensity[2]*0.2;
            cb[i].rendered_z[0][2] = cb[i].col_b * -intensity[2]*0.2;
            cb[i].rendered_z[1][0] = cb[i].col_r * -intensity[2];
            cb[i].rendered_z[1][1] = cb[i].col_g * -intensity[2];
            cb[i].rendered_z[1][2] = cb[i].col_b * -intensity[2];
        }
    }
    cout<<"relighting done!"<<endl;
}
void cam(){ // 카메라 재설정
    cin>>cam_x>>cam_y>>cam_z>>cam_h>>cam_v;
    cout<<"camera set!"<<endl;
}
void light_edit(){ // 광원 재설정
    cin>>light[0]>>light[1];
    intensity[0]=cos(light[0])*cos(light[1]); intensity[1]=sin(light[0])*cos(light[1]); intensity[2]=sin(light[1]);
    cout<<"light set!"<<endl;
}
void renderer(){ // 렌더링
    int i,j,k,l,m;
    double local_x=cos(cam_h)*cos(cam_v);
    double local_y=sin(cam_h)*cos(cam_v);
    double local_z=sin(cam_v);
    cout<<"rendering..."<<endl;
    f2(i,w)f2(j,h){
        double r=0,g=0,b=0;
        f2(k,aa)f2(l,aa){
            double dh=((i*aa+k)-(double)w/2*aa)*1.0/w/aa, dv=((j*aa+l)-(double)h/2*aa)*1.0/w/aa;
            double local_dx=dh*sin(cam_h)-dv*cos(cam_h)*sin(cam_v)+local_x;
            double local_dy=-dh*cos(cam_h)-dv*sin(cam_h)*sin(cam_v)+local_y;
            double local_dz=dv*cos(cam_v)+local_z;
            double len=sqrt(local_dx*local_dx+local_dy*local_dy+local_dz*local_dz);
            // 솔브닥 기하 레이팅 플레가 될 테야!
            local_dx/=len; local_dy/=len; local_dz/=len;
            double min_d=987654321.0,col[3]={(double)bgc[0],(double)bgc[1],(double)bgc[2]};
            f2(m,cb.size()){
                if(cam_x<cb[m].x&&local_dx>0){
                    double d=(cb[m].x-cam_x)/local_dx;
                    double y=cam_y+local_dy*d;
                    double z=cam_z+local_dz*d;
                    if(d>0&&d<min_d&&y>=cb[m].y&&y<=cb[m].y+cb[m].ly&&z>=cb[m].z&&z<=cb[m].z+cb[m].lz){
                        min_d=d;
                        col[0]=cb[m].rendered_x[0][0],col[1]=cb[m].rendered_x[0][1],col[2]=cb[m].rendered_x[0][2];
                    }
                }
                else if(cam_x>cb[m].x+cb[m].lx&&local_dx<0){
                    double d=(cb[m].x+cb[m].lx-cam_x)/local_dx;
                    double y=cam_y+local_dy*d;
                    double z=cam_z+local_dz*d;
                    if(d>0&&d<min_d&&y>=cb[m].y&&y<=cb[m].y+cb[m].ly&&z>=cb[m].z&&z<=cb[m].z+cb[m].lz){
                        min_d=d;
                        col[0]=cb[m].rendered_x[1][0],col[1]=cb[m].rendered_x[1][1],col[2]=cb[m].rendered_x[1][2];
                    }
                }
                if(cam_y<cb[m].y&&local_dy>0){
                    double d=(cb[m].y-cam_y)/local_dy;
                    double x=cam_x+local_dx*d;
                    double z=cam_z+local_dz*d;
                    if(d>0&&d<min_d&&x>=cb[m].x&&x<=cb[m].x+cb[m].lx&&z>=cb[m].z&&z<=cb[m].z+cb[m].lz){
                        min_d=d;
                        col[0]=cb[m].rendered_y[0][0],col[1]=cb[m].rendered_y[0][1],col[2]=cb[m].rendered_y[0][2];
                    }
                }
                else if(cam_y>cb[m].y+cb[m].ly&&local_dy<0){
                    double d=(cb[m].y+cb[m].ly-cam_y)/local_dy;
                    double x=cam_x+local_dx*d;
                    double z=cam_z+local_dz*d;
                    if(d>0&&d<min_d&&x>=cb[m].x&&x<=cb[m].x+cb[m].lx&&z>=cb[m].z&&z<=cb[m].z+cb[m].lz){
                        min_d=d;
                        col[0]=cb[m].rendered_y[1][0],col[1]=cb[m].rendered_y[1][1],col[2]=cb[m].rendered_y[1][2];
                    }
                }
                if(cam_z<cb[m].z&&local_dz>0){
                    double d=(cb[m].z-cam_z)/local_dz;
                    double x=cam_x+local_dx*d;
                    double y=cam_y+local_dy*d;
                    if(d>0&&d<min_d&&x>=cb[m].x&&x<=cb[m].x+cb[m].lx&&y>=cb[m].y&&y<=cb[m].y+cb[m].ly){
                        min_d=d;
                        col[0]=cb[m].rendered_z[0][0],col[1]=cb[m].rendered_z[0][1],col[2]=cb[m].rendered_z[0][2];
                    }
                }
                else if(cam_z>cb[m].z+cb[m].lz&&local_dz<0){
                    double d=(cb[m].z+cb[m].lz-cam_z)/local_dz;
                    double x=cam_x+local_dx*d;
                    double y=cam_y+local_dy*d;
                    if(d>0&&d<min_d&&x>=cb[m].x&&x<=cb[m].x+cb[m].lx&&y>=cb[m].y&&y<=cb[m].y+cb[m].ly){
                        min_d=d;
                        col[0]=cb[m].rendered_z[1][0],col[1]=cb[m].rendered_z[1][1],col[2]=cb[m].rendered_z[1][2];
                    }
                }
            }
            r+=col[0];
            g+=col[1];
            b+=col[2];
        }
        camera[i][j][0]=r/aa/aa;
        camera[i][j][1]=g/aa/aa;
        camera[i][j][2]=b/aa/aa;
    }
    cout<<"rendering done!"<<endl;
}
void advs(){
    cout<<"current settings"<<endl<<"- anti-aliasing (command: aa) "<<aa<<"x"<<endl<<"- background color (command: bg) "<<bgc[0]<<" "<<bgc[1]<<" "<<bgc[2]<<endl;
    string s; cin>>s;
    if(s=="aa") cin>>aa;
    if(s=="bgc") cin>>bgc[0]>>bgc[1]>>bgc[2];
}
int main(){
    init();
    while(true){
        string d; cin>>d;
        if(d=="exit") break;
        if(d=="add") add();
        if(d=="delete") {cb.pop_back(); cout<<"last cube was deleted, you have "<<cb.size()<<" cubes left"<<endl;}
        if(d=="camera") cam();
        if(d=="light") light_edit();
        if(d=="adv") advs();
        if(d=="render"){
            string s;
            cout<<"save name:"; cin>>s; s+=".bmp";
            cout<<"rendering and saving to "<<s<<endl;
            relight();
            renderer();
            FILE *fp1=fopen("from.bmp","rb");
            FILE *fp2=fopen(s.c_str(),"wb");
            int i,j;
            f2(i,54) putc(getc(fp1),fp2);
            f2(j,h) f2(i,w){
                int c[3]={(int)camera[i][j][2],(int)camera[i][j][1],(int)camera[i][j][0]};
                if(c[0]<0)c[0]=0; if(c[0]>255)c[0]=255;
                if(c[1]<0)c[1]=0; if(c[1]>255)c[1]=255;
                if(c[2]<0)c[2]=0; if(c[2]>255)c[2]=255;
                putc(c[0],fp2);
                putc(c[1],fp2);
                putc(c[2],fp2);
            }
            fclose(fp1); fclose(fp2);
        }
        if(d=="save"){
            int i;
            string s;
            cout<<"name of the file:"; cin>>s; s+=".txt";
            FILE *fp=fopen(s.c_str(),"w");
            fprintf(fp,"%lf %lf %lf %lf %lf\n",cam_x,cam_y,cam_z,cam_h,cam_v);
            fprintf(fp,"%lf %lf %lf %lf %lf\n",light[0],light[1],intensity[0],intensity[1],intensity[2]);
            fprintf(fp,"%lld\n",cb.size());
            f2(i,cb.size()){
                fprintf(fp,"%lf %lf %lf %lf %lf %lf %lf %lf %lf\n",cb[i].x,cb[i].y,cb[i].z,cb[i].lx,cb[i].ly,cb[i].lz,cb[i].col_r,cb[i].col_g,cb[i].col_b);
            }
            fclose(fp);
            cout<<"save complete!"<<endl;
        }
        if(d=="load"){
            int i;
            string s;
            cout<<"name of the file:"; cin>>s; s+=".txt";
            FILE *fp=fopen(s.c_str(),"r");
            fscanf(fp,"%lf %lf %lf %lf %lf\n",&cam_x,&cam_y,&cam_z,&cam_h,&cam_v);
            fscanf(fp,"%lf %lf %lf %lf %lf\n",&light[0],&light[1],&intensity[0],&intensity[1],&intensity[2]);
            cb.clear();
            long long n; fscanf(fp,"%lld\n",&n);
            f2(i,n){
                cube c{};
                fscanf(fp,"%lf %lf %lf %lf %lf %lf %lf %lf %lf\n",&c.x,&c.y,&c.z,&c.lx,&c.ly,&c.lz,&c.col_r,&c.col_g,&c.col_b);
                cb.push_back(c);
            }
            fclose(fp);
            cout<<"load complete!"<<endl;
        }
    }
    return 0;
}
