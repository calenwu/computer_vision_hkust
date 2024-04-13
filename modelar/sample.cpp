// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"

#include "stb_image.h"

#include <FL/gl.h>

#include "modelerglobals.h"

// #version 330 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
// layout (location = 2) in vec2 aTexCoord;

// out vec3 ourColor;
// out vec2 TexCoord;

unsigned int texture;
int width, height, nrChannels;
unsigned char *data;

// To make a SampleModel, we inherit off of ModelerView
class SampleModel : public ModelerView 
{
public:
    SampleModel(int x, int y, int w, int h, char *label) 
        : ModelerView(x, y, w, h, label) { }

    virtual void draw();
};

void Head() {
	glPushMatrix();
		glTranslated(0, 0, 0.75 + 0.75);
		drawSphere(0.75);
	glPopMatrix();
}

void Neck() {
	glPushMatrix();
		glTranslated(1.5, 0.5, 4);
		drawCylinder(0.75, 0.375, 0.375);
		// head
		Head();
	glPopMatrix();
}

void LowerJoint() {
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

void MiddleJoint() {
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
			// lower joint
			LowerJoint();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

void Finger(int angle) {
	// upper joint
	glPushMatrix();
		glRotated(VAL(CURLFINGERSPERCENTAGE) / 100.0 * -90, 1.0, 0.0, 0.0);
		glRotated(angle, 0, 1, 0);
		glTranslated(-0.1, 0, -0.4 - 0.375);
		drawCylinder(0.3, 0.1, 0.1);
		glPushMatrix();
			glTranslated(0, 0, 0);
			drawSphere(0.1);
			MiddleJoint();
		glPopMatrix();
	glPopMatrix();
}

void LowerArm() {
	// lower arm
	glPushMatrix();
		glTranslated(0, 0, -0.75);
		glRotated(VAL(LEFTLOWERARMROTATEX), 1.0, 0, 0);
		drawSphere(0.375);
		glPushMatrix();
			glTranslated(0, 0, -1.5);
			drawCylinder(1.5, 0.375, 0.375);
			glPushMatrix();
				glTranslated(0, 0, 0);
				drawSphere(0.375);
				// fingers
				// left finger
				Finger(20);
				// right finger
				Finger(-20);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

void UpperArm(int x, int y, int z) {
	glPushMatrix();
		// upper arm
		glTranslated(x, y, z);
		glRotated(VAL(LEFTUPPERARMROTATEX), 1.0, 0, 0);
		glRotated(VAL(LEFTUPPERARMROTATEY), 0, 1.0, 0);
		drawSphere(0.375);
		glPushMatrix();
			glTranslated(0, 0, -1.5);
			drawCylinder(1.5, 0.375, 0.375);
			glPushMatrix();
				drawSphere(0.375);
				LowerArm();
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

void Leg(int x, int y, int z) {
	glPushMatrix();
		glTranslated(x, y, z);
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

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createSampleModel(int x, int y, int w, int h, char *label)
{ 
    return new SampleModel(x, y, w, h, label); 
}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out SampleModel
void SampleModel::draw()
{
	// This call takes care of a lot of the nasty projection 
	// matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
	ModelerView::draw();

	// draw the floor
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(COLOR_RED);
	glPushMatrix();
		glTranslated(-5,0,-5);
		drawBox(10,0.01f,10);
	glPopMatrix();

	// draw the sample model
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(COLOR_GREEN);
	glPushMatrix();
		glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
		glRotated(-90, 1, 0, 0);
		glPushMatrix();

			// torso
			glRotated(VAL(ROTATE), 0.0, 0.0, 1.0);
			glTranslated(-1.5, -0.5, 3.75);
			glScaled(1, 0.9, 1);
			drawBox(3, 1, 4);

			// // neck
			// Neck();
			
			// // arms
			// // left arm
			// UpperArm(-0.375, 0.125 + 0.375, 4 - 0.375);
			// // right arm
			// UpperArm(3 + 0.375, 0.125 + 0.375, 4 - 0.375);

			// // legs
			// // left leg
			// Leg(0.25 + 0.375, 0.125 + 0.375, -0.375);
			// // right leg
			// Leg(0.25 + 2 * 0.375 + 1 + 0.375, 0.125 +0.375, -0.375);

			// neck
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glBindTexture(GL_TEXTURE_2D, texture);

			glEnable(GL_TEXTURE_2D);
				if (VAL(LEVELOFDETAIL) >= 2) {
					glPushMatrix();
						glTranslated(1.5, 0.5, 4);
						drawCylinder(0.75, 0.375, 0.375);
						// head
						if (VAL(LEVELOFDETAIL) >= 3) {
							glPushMatrix();
								glTranslated(0, 0, 0.75 + 0.75);
								drawSphere(0.75);
							glPopMatrix();
						}
					glPopMatrix();
				}
			glDisable(GL_TEXTURE_2D);
			// arms
			// left arm
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
											// fingers
											// left finger
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
																	glPopMatrix();
																glPopMatrix();
															glPopMatrix();
														}
													glPopMatrix();
												glPopMatrix();
											}
											// right finger
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
																	glPopMatrix();
																glPopMatrix();
															glPopMatrix();
														}
													glPopMatrix();
												glPopMatrix();
											}
										glPopMatrix();
									glPopMatrix();
								}
							glPopMatrix();
						glPopMatrix();
				glPopMatrix();
			}
			// right arm
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
																glPopMatrix();
															glPopMatrix();
														glPopMatrix();
													}
												glPopMatrix();
											glPopMatrix();
										}
										// right finger
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
																glPopMatrix();
															glPopMatrix();
														glPopMatrix();
													}
												glPopMatrix();
											glPopMatrix();
										}
									glPopMatrix();
								glPopMatrix();
							}
						glPopMatrix();
					glPopMatrix();
				glPopMatrix();
			}
			
			// legs
			// left leg
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
			// right leg
			if (VAL(LEVELOFDETAIL) >= 2) {
				glPushMatrix();
					glTranslated(0.25 + 2 * 0.375 + 1 + 0.375, 0.125 +0.375, -0.375);
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
		glPopMatrix();
		

		// glPushMatrix();
		// 	glTranslated(-1.5, 0, 0);
		// 	drawCylinder(5, 1, 1);
		// glPopMatrix();
		// glPushMatrix();
		// 	glTranslated(1.5, 0, 0);
		// 	drawCylinder(5, 1, 1);
		// glPopMatrix();

		// draw box
		// glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
		// glPushMatrix();
		// 	glTranslated(-1.5, 0, -2);
		// 	drawBox(3, 1, 4);
		// glPopMatrix();

		// draw cannon
		// glPushMatrix();
		// 	glRotated(VAL(ROTATE), 0.0, 1.0, 0.0);
		// 	glRotated(-90, 1.0, 0.0, 0.0);
		// 	drawCylinder(VAL(HEIGHT), 0.1, 0.1);

		// 	glTranslated(0.0, 0.0, VAL(HEIGHT));
		// 	drawCylinder(1, 1.0, 0.9);

		// 	glTranslated(0.0, 0.0, 0.5);
		// 	glRotated(90, 1.0, 0.0, 0.0);
		// 	drawCylinder(4, 0.1, 0.2);
		// glPopMatrix();
	glPopMatrix();
}


int main()
{
	glGenTextures(1, &texture);
	// load and generate the texture
	data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	// stbi_image_free(data);


	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// glEnableVertexAttribArray(2);  

	glBindTexture(GL_TEXTURE_2D, texture);
	// glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
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
	controls[LEVELOFDETAIL] = ModelerControl("Level of detail", 1, 6, 1, 1);

	ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
	return ModelerApplication::Instance()->Run();
}

