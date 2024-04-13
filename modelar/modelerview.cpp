#include "modelerview.h"
#include "camera.h"

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.h>
#include <FL/gl.h>
#include <GL/glu.h>
#include <cstdio>

static const int	kMouseRotationButton			= FL_LEFT_MOUSE;
static const int	kMouseTranslationButton			= FL_MIDDLE_MOUSE;
static const int	kMouseZoomButton				= FL_RIGHT_MOUSE;

ModelerView::ModelerView(int x, int y, int w, int h, char *label)
: Fl_Gl_Window(x,y,w,h,label)
{
    m_camera = new Camera();
}

ModelerView::~ModelerView()
{
	delete m_camera;
}
int ModelerView::handle(int event)
{
    unsigned eventCoordX = Fl::event_x();
	unsigned eventCoordY = Fl::event_y();
	unsigned eventButton = Fl::event_button();
	unsigned eventState  = Fl::event_state();

	switch(event)	 
	{
	case FL_PUSH:
		{
			switch(eventButton)
			{
			case kMouseRotationButton:
				m_camera->clickMouse(kActionRotate, eventCoordX, eventCoordY );
				break;
			case kMouseTranslationButton:
				m_camera->clickMouse(kActionTranslate, eventCoordX, eventCoordY );
				break;
			case kMouseZoomButton:
				m_camera->clickMouse(kActionZoom, eventCoordX, eventCoordY );
				break;
			}
           // printf("push %d %d\n", eventCoordX, eventCoordY);
		}
		break;
	case FL_DRAG:
		{
			m_camera->dragMouse(eventCoordX, eventCoordY);
            //printf("drag %d %d\n", eventCoordX, eventCoordY);
		}
		break;
	case FL_RELEASE:
		{
			switch(eventButton)
			{
			case kMouseRotationButton:
			case kMouseTranslationButton:
			case kMouseZoomButton:
				m_camera->releaseMouse(eventCoordX, eventCoordY );
				break;
			}
          //  printf("release %d %d\n", eventCoordX, eventCoordY);
		}
		break;
	default:
		return 0;
	}
	
	redraw();

	return 1;
}

static GLfloat lightPosition0[] = { 4, 2, -4, 0 };
static GLfloat lightDiffuse0[]  = { 1,1,1,1 };
static GLfloat lightPosition1[] = { -2, 1, 5, 0 };
static GLfloat lightDiffuse1[]  = { 1, 1, 1, 1 };

void ModelerView::draw()
{
	if (!valid()) {
		glShadeModel( GL_SMOOTH );
		glEnable( GL_DEPTH_TEST );
		glEnable( GL_LIGHTING );
		glEnable( GL_LIGHT0 );
		glEnable( GL_LIGHT1 );
		glEnable( GL_NORMALIZE );


		glEnable(GL_LIGHT2);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
		lightPosition0[0] = 2.8f;
		lightPosition0[1] = 2.8f;
		lightPosition0[2] = 3.0f;
		lightPosition0[3] = 1.0f;
		lightPosition1[0] = -2.8f;
		lightPosition1[1] = -2.8f;
		lightPosition1[2] = 1.0f;
		lightPosition1[3] = 1.0f;
		GLfloat lightPosition2[] = { -2.8f, -23.3f, -39.1f, 1.f };
		GLfloat lightIntensity0[] = { 0.8f, 0.5f, 0.5f, 0.2f };
		GLfloat lightIntensity1[] = { 0.8f, 0.5f, 0.5f, 0.2f };
		GLfloat lightIntensity2[] = { 9.f, 1.f, 1.f, 1.f };

		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
		glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
		glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity0);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, lightIntensity1);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, lightIntensity2);
	}

	glViewport( 0, 0, w(), h() );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, float(w())/float(h()), 1.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_camera->applyViewingTransform();

	glLightfv( GL_LIGHT0, GL_POSITION, lightPosition0 );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, lightDiffuse0 );
	glLightfv( GL_LIGHT1, GL_POSITION, lightPosition1 );
	glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse1 );
}