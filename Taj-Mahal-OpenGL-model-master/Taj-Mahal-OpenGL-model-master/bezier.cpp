#include <iostream>
#include <GL/glut.h> 

using namespace std;


/**Defines a 2D point
*/
struct point
{
	float x;
	float y;
};

int n;					/*<Number of control points*/
///Degree is n-1
point points[200];		/*<Array for control points*/
int draggedTo;			/*<New location of a dragged point*/
bool dragging = false;	/*<Indicates whether dragging is going on*/


void plot(int x, int y, int color){
	if(color==1){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(4.5);
	}
	else {
		glColor3f(1.0, 1.0, 0.0);
		glPointSize(2);
	}
	glBegin(GL_POINTS);
	glVertex2i(x, y);
    glEnd();
}


void clearScreen(){
	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}


point lerp(float t, point a, point b){
	point p;
	p.x = (1-t)*a.x + t*b.x;
	p.y = (1-t)*a.y + t*b.y;
	return p;
}


point deCasteljau(float t, point p[], int n){
	point q[n];
	for(int i=0; i<n-1; i++){
		q[i]=lerp(t, p[i], p[i+1]);
	}
	
	if(n==2){
		return q[0];
	}
	return deCasteljau(t, q, n-1);
}


void draw(){
	clearScreen();
	
	///Plotting the control points
	for(int i=0; i<n; i++){
		plot(points[i].x, points[i].y, 1);
	}
	
	///Drawing the curve
	if(n<2){
		glFlush();
		return;
	}
	for (float t=0; t<=1; t+=0.0005){
		point p = deCasteljau(t, points, n);
		plot(p.x, p.y, 2);
	}
	glFlush();
}


void display(){
	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	draw();
}


int nearTo(int x, int y){
	for(int i=0; i<n; i++){
		if(abs(x-points[i].x)<=2 && abs(y-points[i].y)<=2){
			return i;
		}
	}
	return -1;
}


void keyboard(unsigned char key, int x, int y){
	switch(key)
	{
		case 'z':	cout<<"UNDO last point"<<endl;
					if (n > 0)
					{
						n--;
					}
					draw();	
					break;
					
		case 'c':   cout<<"Clearing screen"<<endl;
					n = 0;
					clearScreen();
					glFlush();
					glutSetCursor(GLUT_CURSOR_CROSSHAIR);
					break; 	
	}
}


void mouse(int button, int state, int x, int y){
	if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
		int index = nearTo(x,y);
		if(index==-1){
			point p;
			p.x=x;
			p.y=y;
			points[n++]=p;
			cout<<"PLotting ("<<x<<","<<y<<")\n";
		} 
		else {
			cout<<"Dragging\n";
			draggedTo = index;
			dragging = true;
		}
	}
	else if(button==GLUT_LEFT_BUTTON && state==GLUT_UP){
		if(dragging){
			points[draggedTo].x = x;
			points[draggedTo].y = y;
			dragging = false;
			cout<<"Dragged to ("<<x<<","<<y<<")\n";
		}
	}
	else if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN){
		int index = nearTo(x,y);
		if(index!=-1){
			cout<<"Deleting ("<<points[index].x<<","<<points[index].y<<")\n";
			for(int i=index; i<n-1; i++){
				points[i].x = points[i+1].x;
				points[i].y = points[i+1].y;
			}
			n--;
		}
	}
}


void passiveMotion(int x, int y){
	int index = nearTo(x,y);
	if(index==-1)
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	else
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
}


void init() {

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);	///Display Mode
	glutInitWindowPosition(0, 0);					///Initialises the start position on the laptop screen
	glutInitWindowSize(700, 500);					///Initialises window size
	glutCreateWindow("Bezier Curve");
	gluOrtho2D(0, 700, 500, 0);						///This is 2D Orthographic view
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);			
	clearScreen();
}


int main(int argc, char** argv) {
	cout<<"Press 'c' to clear the screen and 'z' to undo.\n";
	
	glutInit(&argc, argv);            			///Initialize GLUT
	init();	
	
	///Registering the event handler callbacks
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(passiveMotion);
	clearScreen();
	glutMainLoop();			///Control goes into the event processing loop
}
