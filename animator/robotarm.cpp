// The sample robotarm model.  You should build a file
// very similar to this for when you make your model.
#pragma warning (disable : 4305)
#pragma warning (disable : 4244)
#pragma warning(disable : 4786)

#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "particleSystem.h"

#include "stb_image.h"

#include "vec.h"
#include "mat.h"
#include <FL/gl.h>
#include <stdlib.h>

#define M_DEFAULT 2.0f
#define M_OFFSET 3.0f
#define P_OFFSET 0.3f
#define MAX_VEL 200
#define MIN_STEP 0.1

#define COLOR_RED		1.0f, 0.0f, 0.0f
#define COLOR_GREEN		0.0f, 1.0f, 0.0f
#define COLOR_BLUE		0.0f, 0.0f, 1.0f



// This is a list of the controls for the RobotArm
// We'll use these constants to access the values 
// of the controls from the user interface.
enum SampleModelControls
{
	XPOS,
	YPOS,
	ZPOS,
	ROTATE,
	LEFTUPPERARMROTATEX,
	LEFTUPPERARMROTATEY,
	LEFTLOWERARMROTATEX,
	RIGHTUPPERARMROTATEX,
	RIGHTUPPERARMROTATEY,
	RIGHTLOWERARMROTATEX,
	CURLFINGERSPERCENTAGE,
	LEVELOFDETAIL,
	NUMCONTROLS
};
unsigned int texture;
int width, height, nrChannels;
unsigned char* dataData;
// To make a RobotArm, we inherit off of ModelerView
class RobotArm : public ModelerView
{
public:
	RobotArm(int x, int y, int w, int h, char* label)
		: ModelerView(x, y, w, h, label) {}
	virtual void draw();
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createRobotArm(int x, int y, int w, int h, char* label)
{
	return new RobotArm(x, y, w, h, label);
}

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

Mat4d getModelViewMatrix() {
	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	Mat4d matMV(
		m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]
	);
	return matMV.transpose();
}

void SpawnParticles(Mat4d CameraTransforms) {
	Mat4d ModelTransforms = CameraTransforms.inverse() * getModelViewMatrix();
	Vec4d temp = Vec4d(VAL(XPOS), VAL(YPOS) + 9, VAL(ZPOS) + 1, 0.8);
	Vec4d WorldPoint = Vec4d(
		*ModelTransforms[0] * temp[0] + *ModelTransforms[1] * temp[1] + *ModelTransforms[2] * temp[2] + *ModelTransforms[3] * temp[3],
		*ModelTransforms[4] * temp[0] + *ModelTransforms[5] * temp[1] + *ModelTransforms[6] * temp[2] + *ModelTransforms[7] * temp[3],
		*ModelTransforms[8] * temp[0] + *ModelTransforms[9] * temp[1] + *ModelTransforms[10] * temp[2] + *ModelTransforms[11] * temp[3],
		*ModelTransforms[12] * temp[0] + *ModelTransforms[13] * temp[1] + *ModelTransforms[14] * temp[2] + *ModelTransforms[15] * temp[3]
	);
	ParticleSystem *ps = ModelerApplication::Instance()->GetParticleSystem();
	map<double, std::vector<Particle>>::iterator it = ps->bakedParticles.find(ps->getBakeFps() + ps->currentT);
	if (ps->isSimulate()) {
		if (it == ps->bakedParticles.end()) {
			Particle p = Particle(
				Vec3d(
					temp[0],
					temp[1],
					temp[2]
				),
				Vec3d(
					+ ((float(rand()) / float(RAND_MAX)) * (2)) + -1,
					static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2)),
					static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2))
				)
			);
			ps->particles.push_back(p);
		}
	}
}

void drawTorso() {
	glRotated(VAL(ROTATE), 0.0, 0.0, 1.0);
	glTranslated(-1.5, -0.5, 3.75);
	glScaled(1, 1, 1);
	drawBox(3, 1, 4);
}

void drawHead() {
	if (VAL(LEVELOFDETAIL) >= 3) {
		glPushMatrix();
		glTranslated(0, 0, 0.75 + 0.75);
		drawSphere(0.75);
		glPopMatrix();
	}
}

void drawNeck() {
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, dataData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texture);

	glEnable(GL_TEXTURE_2D);
	if (VAL(LEVELOFDETAIL) >= 2) {
		// neck
		glPushMatrix();
		glTranslated(1.5, 0.5, 4);
		drawCylinder(0.75, 0.375, 0.375);
		// head
		drawHead();
		glPopMatrix();
	}
	glDisable(GL_TEXTURE_2D);
}

void drawLeftArmRightLowerJoint() {
	if (VAL(LEVELOFDETAIL) >= 6) {
		glPushMatrix();
		glRotated(VAL(CURLFINGERSPERCENTAGE) / 100.0 * -70, 1.0, 0.0, 0.0);
		glTranslated(0, 0, -0.2);
		drawSphere(0.1);
		glPushMatrix();
		glTranslated(0, 0, -0.15);
		drawCylinder(0.15, 0.1, 0.1);
		glPushMatrix();
		glTranslated(0, 0, 0);
		drawSphere(0.1);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
	}
}

void drawLeftArmRightMiddleJoint() {
	if (VAL(LEVELOFDETAIL) >= 5) {
		glPushMatrix();
		glRotated(VAL(CURLFINGERSPERCENTAGE) / 100.0 * -80, 1.0, 0.0, 0.0);
		glTranslated(0, 0, -0.2);
		drawSphere(0.1);
		glPushMatrix();
		glTranslated(0, 0, -0.15);
		drawCylinder(0.15, 0.1, 0.1);
		glPushMatrix();
		glTranslated(0, 0, 0);
		drawSphere(0.1);
		// lower joint
		drawLeftArmRightLowerJoint();
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
	}
}

void drawLeftArmRightUpperJoint() {
	if (VAL(LEVELOFDETAIL) >= 4) {
		// upper joint
		glPushMatrix();
		glRotated(VAL(CURLFINGERSPERCENTAGE) / 100.0 * -90, 1.0, 0.0, 0.0);
		glRotated(-20, 0, 1, 0);
		glTranslated(-0.1, 0, -0.4 - 0.375);
		drawCylinder(0.3, 0.1, 0.1);
		glPushMatrix();
		glTranslated(0, 0, 0);
		drawSphere(0.1);
		// middle joint
		drawLeftArmRightMiddleJoint();
		glPopMatrix();
		glPopMatrix();
	}
}

void drawLeftArmLeftLowerJoint() {
	if (VAL(LEVELOFDETAIL) >= 6) {
		glPushMatrix();
		glRotated(VAL(CURLFINGERSPERCENTAGE) / 100.0 * -70, 1.0, 0.0, 0.0);
		glTranslated(0, 0, -0.2);
		drawSphere(0.1);
		glPushMatrix();
		glTranslated(0, 0, -0.15);
		drawCylinder(0.15, 0.1, 0.1);
		glPushMatrix();
		glTranslated(0, 0, 0);
		drawSphere(0.1);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
	}
}

void drawLeftArmLeftMiddleJoint() {
	if (VAL(LEVELOFDETAIL) >= 5) {
		glPushMatrix();
		glRotated(VAL(CURLFINGERSPERCENTAGE) / 100.0 * -80, 1.0, 0.0, 0.0);
		glTranslated(0, 0, -0.2);
		drawSphere(0.1);
		glPushMatrix();
		glTranslated(0, 0, -0.15);
		drawCylinder(0.15, 0.1, 0.1);
		glPushMatrix();
		glTranslated(0, 0, 0);
		drawSphere(0.1);
		// lower joint
		drawLeftArmLeftLowerJoint();
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
	}
}

void drawLeftArmLeftUpperJoint() {
	if (VAL(LEVELOFDETAIL) >= 4) {
		// upper joint
		glPushMatrix();
		glRotated(VAL(CURLFINGERSPERCENTAGE) / 100.0 * -90, 1.0, 0.0, 0.0);
		glRotated(20, 0, 1, 0);
		glTranslated(-0.1, 0, -0.4 - 0.375);
		drawCylinder(0.3, 0.1, 0.1);
		glPushMatrix();
		glTranslated(0, 0, 0);
		drawSphere(0.1);
		// middle joint
		drawLeftArmLeftMiddleJoint();
		glPopMatrix();
		glPopMatrix();
	}
}

void drawLeftLowerArm(Mat4d CameraTransforms) {
	if (VAL(LEVELOFDETAIL) >= 3) {
		glTranslated(0, 0, -0.75);
		glRotated(VAL(LEFTLOWERARMROTATEX), 1.0, 0, 0);
		drawSphere(0.375);
		glPushMatrix();
		glTranslated(0, 0, -1.5);
		drawCylinder(1.5, 0.375, 0.375);
		glPushMatrix();
		glTranslated(0, 0, 0);
		drawSphere(0.375);
		SpawnParticles(CameraTransforms);
		// fingers
		// left finger
		drawLeftArmLeftUpperJoint();
		// right finger
		drawLeftArmRightUpperJoint();
		glPopMatrix();
		glPopMatrix();
	}
}

void drawLeftArm(Mat4d CameraTransforms) {
	if (VAL(LEVELOFDETAIL) >= 2) {
		glPushMatrix();
		// upper arm
		glTranslated(-0.375, 0.125 + 0.375, 4 - 0.375);
		glRotated(VAL(LEFTUPPERARMROTATEX), 1.0, 0, 0);
		glRotated(VAL(LEFTUPPERARMROTATEY), 0, 1.0, 0);
		drawSphere(0.375);
		glPushMatrix();
		glTranslated(0, 0, -1.5);
		drawCylinder(1.5, 0.375, 0.375);
		glPushMatrix();
		drawSphere(0.375);
		// lower arm
		drawLeftLowerArm(CameraTransforms);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
	}
}

void drawRightArmLeftLowerJoint() {
	if (VAL(LEVELOFDETAIL) >= 6) {
		// lower joint
		glPushMatrix();
		glRotated(VAL(CURLFINGERSPERCENTAGE) / 100.0 * -70, 1.0, 0.0, 0.0);
		glTranslated(0, 0, -0.2);
		drawSphere(0.1);
		glPushMatrix();
		glTranslated(0, 0, -0.15);
		drawCylinder(0.15, 0.1, 0.1);
		glPushMatrix();
		glTranslated(0, 0, 0);
		drawSphere(0.1);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
	}
}

void drawRightArmLeftMiddleJoint() {
	if (VAL(LEVELOFDETAIL) >= 5) {
		// middle joint
		glPushMatrix();
		glRotated(VAL(CURLFINGERSPERCENTAGE) / 100.0 * -80, 1.0, 0.0, 0.0);
		glTranslated(0, 0, -0.2);
		drawSphere(0.1);
		glPushMatrix();
		glTranslated(0, 0, -0.15);
		drawCylinder(0.15, 0.1, 0.1);
		glPushMatrix();
		glTranslated(0, 0, 0);
		drawSphere(0.1);
		drawRightArmLeftLowerJoint();
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
	}
}

void drawRightArmLeftUpperJoint() {
	if (VAL(LEVELOFDETAIL) >= 4) {
		// upper joint
		glPushMatrix();
		glRotated(VAL(CURLFINGERSPERCENTAGE) / 100.0 * -90, 1.0, 0.0, 0.0);
		glRotated(20, 0, 1, 0);
		glTranslated(-0.1, 0, -0.4 - 0.375);
		drawCylinder(0.3, 0.1, 0.1);
		glPushMatrix();
		glTranslated(0, 0, 0);
		drawSphere(0.1);
		drawRightArmLeftMiddleJoint();
		glPopMatrix();
		glPopMatrix();
	}
}

void drawRightArmRightLowerJoint() {
	if (VAL(LEVELOFDETAIL) >= 6) {
		// lower joint
		glPushMatrix();
			glRotated(VAL(CURLFINGERSPERCENTAGE) / 100.0 * -70, 1.0, 0.0, 0.0);
			glTranslated(0, 0, -0.2);
			drawSphere(0.1);
			glPushMatrix();
				glTranslated(0, 0, -0.15);
				drawCylinder(0.15, 0.1, 0.1);
				glPushMatrix();
					glTranslated(0, 0, 0);
					drawSphere(0.1);
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	}
}

void drawRightArmRightMiddleJoint() {
	if (VAL(LEVELOFDETAIL) >= 5) {
		// middle joint
		glPushMatrix();
		glRotated(VAL(CURLFINGERSPERCENTAGE) / 100.0 * -80, 1.0, 0.0, 0.0);
		glTranslated(0, 0, -0.2);
		drawSphere(0.1);
		glPushMatrix();
		glTranslated(0, 0, -0.15);
		drawCylinder(0.15, 0.1, 0.1);
		glPushMatrix();
		glTranslated(0, 0, 0);
		drawSphere(0.1);
		drawRightArmRightLowerJoint();
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
	}
}

void drawRightArmRightUpperJoint() {
	if (VAL(LEVELOFDETAIL) >= 4) {
		// upper joint
		glPushMatrix();
		glRotated(VAL(CURLFINGERSPERCENTAGE) / 100.0 * -90, 1.0, 0.0, 0.0);
		glRotated(-20, 0, 1, 0);
		glTranslated(-0.1, 0, -0.4 - 0.375);
		drawCylinder(0.3, 0.1, 0.1);
		glPushMatrix();
		glTranslated(0, 0, 0);
		drawSphere(0.1);
		drawRightArmRightMiddleJoint();
		glPopMatrix();
		glPopMatrix();
	}
}

void drawRightLowerArm() {
	if (VAL(LEVELOFDETAIL) >= 3) {
		glTranslated(0, 0, -0.75);
		glRotated(VAL(RIGHTLOWERARMROTATEX), 1.0, 0, 0);
		drawSphere(0.375);
		glPushMatrix();
		glTranslated(0, 0, -1.5);
		drawCylinder(1.5, 0.375, 0.375);
		glPushMatrix();
		glTranslated(0, 0, 0);
		drawSphere(0.375);
		// fingers
		// left finger
		drawRightArmLeftUpperJoint();
		// right finger
		drawRightArmRightUpperJoint();
		glPopMatrix();
		glPopMatrix();
	}
}

void drawRightArm() {
	if (VAL(LEVELOFDETAIL) >= 2) {
		glPushMatrix();
		// upper arm
		glTranslated(3 + 0.375, 0.125 + 0.375, 4 - 0.375);
		glRotated(VAL(RIGHTUPPERARMROTATEX), 1.0, 0, 0);
		glRotated(VAL(RIGHTUPPERARMROTATEY), 0, 1.0, 0);
		drawSphere(0.375);
		glPushMatrix();
		glTranslated(0, 0, -1.5);
		drawCylinder(1.5, 0.375, 0.375);
		glPushMatrix();
		drawSphere(0.375);
		// lower arm
		drawRightLowerArm();
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
	}
}

void drawLeftLeg() {
	if (VAL(LEVELOFDETAIL) >= 2) {
		glPushMatrix();
		glTranslated(0.25 + 0.375, 0.125 + 0.375, -0.375);
		drawSphere(0.375);
		glPushMatrix();
		glTranslated(0, 0, -3);
		drawCylinder(3, 0.375, 0.375);
		glPushMatrix();
		drawSphere(0.375);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
	}
}

void drawRightLeg() {
	if (VAL(LEVELOFDETAIL) >= 2) {
		glPushMatrix();
		glTranslated(0.25 + 2 * 0.375 + 1 + 0.375, 0.125 + 0.375, -0.375);
		drawSphere(0.375);
		glPushMatrix();
		glTranslated(0, 0, -3);
		drawCylinder(3, 0.375, 0.375);
		glPushMatrix();
		drawSphere(0.375);
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
	}
}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out RobotArm
void RobotArm::draw() {
	ModelerView::draw();	
	Mat4d CameraTransforms = getModelViewMatrix();


	// draw the floor
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(COLOR_RED);
	glPushMatrix();
	glTranslated(-5, 0, -5);
	drawBox(30, 0.01f, 30);
	glPopMatrix();

	// draw the sample model
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(COLOR_GREEN);
	glPushMatrix();
	glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
	glRotated(-90, 1, 0, 0);
	glPushMatrix();
	// torso
	drawTorso();
	//neck
	drawNeck();
	// arms
	// left arm
	drawLeftArm(CameraTransforms);
	// right arm
	drawRightArm();
	// legs
	// left leg
	drawLeftLeg();
	// right leg
	drawRightLeg();
	glPopMatrix();
	glPopMatrix();
	endDraw();
}

int main()
{
	glGenTextures(1, &texture);
	// load and generate the texture
	dataData = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	// stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, texture);
	// glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	ModelerControl controls[NUMCONTROLS];
	controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
	controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
	controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
	controls[ROTATE] = ModelerControl("Rotate", -135, 135, 1, 0);
	controls[LEFTUPPERARMROTATEX] = ModelerControl("Rotate left upper arm X", -90, 90, 1, 0);
	controls[LEFTUPPERARMROTATEY] = ModelerControl("Rotate left upper arm Y", 0, 180, 1, 0);
	controls[LEFTLOWERARMROTATEX] = ModelerControl("Rotate left lower arm X", -90, 0, 1, 0);
	controls[RIGHTUPPERARMROTATEX] = ModelerControl("Rotate right upper arm X", -90, 90, 1, 0);
	controls[RIGHTUPPERARMROTATEY] = ModelerControl("Rotate right upper arm Y", -180, 0, 1, 0);
	controls[RIGHTLOWERARMROTATEX] = ModelerControl("Rotate right lower arm X", -90, 0, 1, 0);
	controls[CURLFINGERSPERCENTAGE] = ModelerControl("Finger curl percentage", 0, 100, 1, 0);
	controls[LEVELOFDETAIL] = ModelerControl("Level of detail", 1, 6, 1, 6);


	// You should create a ParticleSystem object ps here and then
	// call ModelerApplication::Instance()->SetParticleSystem(ps)
	// to hook it up to the animator interface.

	ParticleSystem *ps = new ParticleSystem();
	ModelerApplication::Instance()->SetParticleSystem(ps);
	ModelerApplication::Instance()->Init(&createRobotArm, controls, NUMCONTROLS);
	return ModelerApplication::Instance()->Run();
}
