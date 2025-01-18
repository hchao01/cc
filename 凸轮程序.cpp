#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define Pi 3.141592653

double r0=25,e=8,rt=8,h=20,t1=60,t2=210,t3=270,t4=360,a1=30,a2=75,P=3.6,N=120;

void main()
{
	int i,j;
	double zz,t,m,n,a,b,pa,s0,ds,dx,dy,d2x,d2y,d2s,T1,T2;
	double a1max=0,a2max=0,t1max,t2max,pamin=1000,tamin,x[4][121],s[121];
	
	zz=Pi;
	T1=((t2-t1)*zz)/180;
	T2=((t4-t3)*zz)/180;
flag:
	s0=sqrt(r0*r0-e*e);
	for(i=0;i<=N;i++)
	{
		t=(2*zz*i)/N;
		if(i>=0&&i<=((t1*N)/360))	/*近休阶段*/
		{
			s[i]=0;
			ds=0;
			d2s=0;
		}
		else if(i>((t1*N)/360)&&i<=((t2*N)/360))	/*推程阶段*/
		{
			t=t-(t1*zz)/180;
			s[i]= h*(t/T1-sin((2*zz*t)/T1)/(2*zz));
			ds= h/T1-(h*cos((2*zz*t)/T1))/T1;
			d2s=(2*zz*h*sin((2*zz*t)/T1))/(pow(T1,2));

			b=(ds-e)/(sqrt(r0*r0-e*e)+s[i]);	/*计算压力角*/
			a=fabs(atan(b));
			if(fabs(a)>=((a1*zz)/180))	/*判断压力角*/
			{
				r0=r0+e;
				goto flag;
			}
			else if(a1max<a)
			{
				a1max=a;
				t1max=t+(t1*zz)/180;
			}
		}
		else if(i>((t2*N)/360)&&i<=((t3*N)/360))	/*远休阶段*/
		{
			s[i]=h;
			ds=0;
			d2s=0;
		}                                                                                      
		else if(i>((t3*N)/360)&&i<=((t4*N)/360))	/*回程阶段*/
		{
			t=t-(t3*zz)/180;
			s[i]= (h*(1+cos((zz*t)/(T2))))/2 ;
			ds=- (h*zz*sin((zz*t)/(T2)))/(2*(T2));
			d2s=-(h*zz*zz*cos((zz*t)/(T2)))/(2*pow((T2),2));

			b=(ds-e)/(sqrt(r0*r0-e*e)+s[i]);	/*计算压力角*/
			a=fabs(atan(b));
			if(fabs(a)>=((a2*zz)/180))	/*判断压力角*/
			{
				r0=r0+e;
				goto flag;
			}
			else if(a2max<a)
			{
				a2max=a;
				t2max=t+(t3*zz)/180;
			}
		}
		else 
		{
			printf("It is wrong!\n");
			exit(0);
		}
		t=(2*zz*i)/N;
		x[0][i]=(s0+s[i])*sin(t)+e*cos(t);
		x[1][i]=(s0+s[i])*cos(t)-e*sin(t);
		dx=(ds-e)*sin(t)+(s0+s[i])*cos(t);
		dy=(ds-e)*cos(t)-(s0+s[i])*sin(t);
		m=dx/sqrt(dx*dx+dy*dy);
		n=-dy/sqrt(dx*dx+dy*dy);
		x[2][i]=x[0][i]-rt*n;
		x[3][i]=x[1][i]-rt*m;
		d2x=(d2s-(s0+s[i]))*sin(t)+(2*ds-e)*cos(t);
		d2y=(d2s-(s0+s[i]))*cos(t)-(2*ds-e)*sin(t);
		pa=sqrt(pow((dx*dx+dy*dy),3))/(dx*d2y-dy*d2x);
		if(pa>0)
		{
			if(P<=(fabs(pa)-rt))
			{
				if(pa<pamin)
				{
					pamin=pa;
					tamin=t;
				}
			}
			else
			{
				s0=s0+e;
				goto flag;
			}
		}
		else continue;
	}
	a1max=(a1max*180)/zz;	/*弧度转化角度*/
	a2max=(a2max*180)/zz;
	t1max=(t1max*180)/zz;
	t2max=(t2max*180)/zz;
	tamin=(tamin*180)/zz;
	
	printf("运算结果如下:\n   δ      s       x       y      x1      y1\n");
	for(i=0;i<=N;i++)
	{
		printf("%4d",3*i);
		printf("%8.3lf",s[i]);
		for(j=0;j<4;j++)
			printf("%8.3lf",x[j][i]);
		printf("\n");
	}
	printf("ρamin=%-8.3lf,δamin=%-8.3lf\nα1max=%-8.3lf,δ1max=%-8.3lf\n",pamin,tamin,a1max,t1max);
	printf("α2max=%-8.3lf,δ2max=%-8.3lf\nr0=%.2lf\n",a2max,t2max,r0);
}
