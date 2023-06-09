#include "ExampleApp.h"

#define FONTSTASH_IMPLEMENTATION
#include <fontstash.h>
#define GLFONTSTASH_IMPLEMENTATION
#include <glfontstash.h>

#include <glm/gtx/string_cast.hpp>

#include <glm/glm.hpp>

#include <config/VRDataIndex.h>

using namespace glm;

ExampleApp::ExampleApp(int argc, char** argv) : VRApp(argc, argv)
{
	_lastTime = 0.0;
	_angle = 0;
    turntable.reset(new TurntableManipulator(5.0,glm::radians(90.0f),glm::radians(45.0f)));
    turntable->setCenterPosition(vec3(0, 0, 0));

}

ExampleApp::~ExampleApp()
{
	glfonsDelete(fs);
	shutdown();
}

void ExampleApp::onAnalogChange(const VRAnalogEvent &event) {
    // This routine is called for all Analog_Change events.  Check event->getName()
    // to see exactly which analog input has been changed, and then access the
    // new value with event->getValue().
    
	if (event.getName() == "FrameStart") {
		_lastTime = _curFrameTime;
		_curFrameTime = event.getValue();
	}


}

void ExampleApp::onButtonDown(const VRButtonEvent &event) {
    // This routine is called for all Button_Down events.  Check event->getName()
    // to see exactly which button has been pressed down.
	//You can respond to individual events like this:
	/*
    if (event.getName() == _paintOnEvent) {
        _painting = true;
    }
    else if (event.getName() == _grabOnEvent) {
        _grabbing = true;
    }
	*/

	//std::cout << "ButtonDown: " << event.getName() << std::endl;
    turntable->onButtonDown(event);


}

void ExampleApp::onButtonUp(const VRButtonEvent &event) {
    // This routine is called for all Button_Up events.  Check event->getName()
    // to see exactly which button has been released.

	//std::cout << "ButtonUp: " << event.getName() << std::endl;
    turntable->onButtonUp(event);

}

void ExampleApp::onCursorMove(const VRCursorEvent &event) {
	// This routine is called for all mouse move events. You can get the absolute position
	// or the relative position within the window scaled 0--1.
	
	//std::cout << "MouseMove: "<< event.getName() << " " << event.getPos()[0] << " " << event.getPos()[1] << std::endl;
    turntable->onCursorMove(event);

}

void ExampleApp::onTrackerMove(const VRTrackerEvent &event) {
    // This routine is called for all Tracker_Move events.  Check event->getName()
    // to see exactly which tracker has moved, and then access the tracker's new
    // 4x4 transformation matrix with event->getTransform().

	// We will use trackers when we do a virtual reality assignment. For now, you can ignore this input type.
}

    
void ExampleApp::onRenderGraphicsContext(const VRGraphicsState &renderState) {
    // This routine is called once per graphics context at the start of the
    // rendering process.  So, this is the place to initialize textures,
    // load models, or do other operations that you only want to do once per
    // frame.
    
	// Is this the first frame that we are rendering after starting the app?
    if (renderState.isInitialRenderCall()) {

		//For windows, we need to initialize a few more things for it to recognize all of the
		// opengl calls.
		#ifndef __APPLE__
			glewExperimental = GL_TRUE;
			GLenum err = glewInit();
			if (GLEW_OK != err)
			{
				std::cout << "Error initializing GLEW." << std::endl;
			}
		#endif     


        glEnable(GL_DEPTH_TEST);
        glClearDepth(1.0f);
        glDepthFunc(GL_LEQUAL);

		glEnable(GL_CULL_FACE);
        glCullFace(GL_NONE);//GL_BACK);

		glEnable(GL_MULTISAMPLE);

		// This sets the background color that is used to clear the canvas
		glClearColor(0.2f, 1.0f, 1.0f, 1.0f);

		// This load shaders from disk, we do it once when the program starts up.
		reloadShaders();

    
		// Make our model objects
//		_box.reset(new Box(vec3(-0.5, -0.5, -0.5), vec3(0.5, 0.5, 0.5), vec4(1.0, 0.0, 0.0, 1.0)));

		initializeText();
        
        // Create the vertex and index lists

        
        //    std::shared_ptr<Texture> tex =
        //    Texture::create2DTextureFromFile(“campbells.jpg”);
        //    textures.push_back(tex);
        // x z y h r
		int size = 20;
		float feather_length = 0.5f;
		float angle = 45.0f;
		float feather_density = 0.05f;
		float feather_radius = 0.05f;
		double time = 0.0;
		int numSamples = 10;
		vec3* vertices = new vec3[size];
		for (int i = 0; i < size; i++) {
			vertices[i] = vec3(0.0f, i*feather_density, 0.0f);
		}
		
		//for (int i = 0; i < size; i++) {
		//	// Make sure we don't double up
		//	if (i > 0) {
		//		time += glm::sqrt(glm::length(original[i] - original[i - 1]));
		//	}
		//	drawSpline.append(time, original[i]);
		//}

		//std::vector<vec3> vertices;
		//vertices.push_back(vec3(0.0f, 0.0f, 0.0f));
		//double interval = drawSpline.totalTime() / (double)(numSamples - 1);
		//// Beginning and end might be snap points, so keep them the same to avoid floating point errors making them not match up any more
		//for (int i = 1; i < numSamples - 1; i++) {
		//	glm::dvec3 pt = drawSpline.evaluate(i * interval);
		//	vertices.push_back(pt);
		//}

        for (int i=0; i < size-1; i++){
			buildRachsis(vertices[i].x, vertices[i].y, vertices[i].z, vertices[i+1].x, vertices[i+1].y, vertices[i+1].z, 0.1f); //Rachis Segment
			vec3 direction = vec3(vertices[i + 1].x - vertices[i].x, vertices[i + 1].y - vertices[i].y, vertices[i + 1].z - vertices[i].z);
			//vec3 u = glm::normalize(direction);
			//vec3 w = glm::normalize(glm::cross(vec3(123, 231, 999), u));
			//vec3 leftBarb = vertices[i + 1] - feather_length/2 * (w+u);
			//vec3 rightBarb = vertices[i + 1] + feather_length/2 * (w+u);

			//vec3 rightBarb = vertices[i + 1] - feather_radius * (w);

            buildRachsis(vertices[i+1].x, vertices[i+1].y, vertices[i+1].z, vertices[i+1].x+feather_length*sin(angle), vertices[i + 1].y+feather_length*cos(angle), vertices[i + 1].z, feather_radius); //LeftBarb
            buildRachsis(vertices[i+1].x, vertices[i+1].y, vertices[i+1].z, vertices[i+1].x-feather_length*sin(angle), vertices[i+1].y+feather_length*cos(angle), vertices[i + 1].z, feather_radius); //RightBarb
			//buildRachsis(vertices[i + 1].x, vertices[i + 1].y, vertices[i + 1].z, leftBarb.x, leftBarb.y ,leftBarb.z, feather_radius);
			//buildRachsis(vertices[i + 1].x, vertices[i + 1].y, vertices[i + 1].z, rightBarb.x, rightBarb.y, rightBarb.z, feather_radius);

        }

		//delete[] vertices;

            
        
        
//		buildRachsis(0.0f, 0.0f, -0.5f, 0.0f,0.0, 0.5f, 0.2f);
//
//        buildRachsis(0.0f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0, 0.2f);
//        buildRachsis(0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.2f);
//        buildRachsis(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.2f);
//        buildRachsis(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.2f);

            
                const int numVertices = cpuVertexArray.size();
                const int cpuVertexByteSize = sizeof(Mesh::Vertex) * numVertices;
                const int cpuIndexByteSize = sizeof(int) * cpuIndexArray.size();
                _mesh.reset(new Mesh(textures, GL_TRIANGLES, GL_STATIC_DRAW,
                    cpuVertexByteSize, cpuIndexByteSize, 0, cpuVertexArray,
                    cpuIndexArray.size(), cpuIndexByteSize, &cpuIndexArray[0]));
            
            
    }

	// Update the angle every frame:
	_angle += glm::radians(1.0);
}


void ExampleApp::onRenderGraphicsScene(const VRGraphicsState &renderState) {
    // This routine is called once per eye/camera.  This is the place to actually
    // draw the scene.
    
	// clear the canvas and other buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Setup the view matrix to set where the camera is located in the scene
	//glm::vec3 eye_world = glm::vec3(0, 2, 5);
	//lm::mat4 view = glm::lookAt(eye_world, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 view = turntable->frame();
    //glm::mat4 model(1.0);

	// When we use virtual reality, this will be replaced by:
	// eye_world = glm::make_vec3(renderState.getCameraPos())
	// view = glm::make_mat4(renderState.getViewMatrix());

	// Setup the projection matrix so that things are rendered in perspective
	GLfloat windowHeight = renderState.index().getValue("FramebufferHeight");
	GLfloat windowWidth = renderState.index().getValue("FramebufferWidth");
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), windowWidth / windowHeight, 0.01f, 100.0f);
	// When we use virtual reality, this will be replaced by:
	// projection = glm::make_mat4(renderState.getProjectionMatrix())
	
	// Setup the model matrix
	glm::mat4 model = glm::mat4(1.0);

//	glm::mat4 rotate = glm::toMat4(glm::angleAxis(_angle, vec3(0, 1, 0))) * glm::toMat4(glm::angleAxis(glm::radians(20.0f), vec3(1.0, 0.0, 0.0)));
//	model = rotate * model;
    
	// Tell opengl we want to use this specific shader.
	_shader.use();
	
	_shader.setUniform("view_mat", view);
	_shader.setUniform("projection_mat", projection);
	
	_shader.setUniform("model_mat", model);
	_shader.setUniform("normal_mat", mat3(transpose(inverse(model))));
	//_shader.setUniform("eye_world", eye_world);

    _mesh-> draw(_shader);

//	_box->draw(_shader, model);

	
	double deltaTime = _curFrameTime - _lastTime;
	std::string fps = "FPS: " + std::to_string(1.0/deltaTime);
	drawText(fps, 10, 10, windowHeight, windowWidth);
}

void ExampleApp::drawText(const std::string text, float xPos, float yPos, GLfloat windowHeight, GLfloat windowWidth) {
	//float lh = 0;
	//fonsVertMetrics(fs, NULL, NULL, &lh);
	//double width = fonsTextBounds(fs, text.c_str(), NULL, NULL) + 40;
	//double height = lh + 40;

	_textShader.use();
	_textShader.setUniform("projection_mat", glm::ortho(0.f, windowWidth, windowHeight, 0.f, -1.f, 1.f));
	_textShader.setUniform("view_mat", glm::mat4(1.0));
	_textShader.setUniform("model_mat", glm::mat4(1.0));
	_textShader.setUniform("lambertian_texture", 0);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	fonsDrawText(fs, xPos, yPos, text.c_str(), NULL);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	_shader.use();

}

void ExampleApp::reloadShaders()
{
	_shader.compileShader("texture.vert", GLSLShader::VERTEX);
	_shader.compileShader("texture.frag", GLSLShader::FRAGMENT);
	_shader.link();
	_shader.use();
}

void ExampleApp::initializeText() {
	int fontNormal = FONS_INVALID;
	fs = nullptr;

	fs = glfonsCreate(512, 512, FONS_ZERO_TOPLEFT);
	if (fs == NULL) {
		assert(false);//Could not create stash
	}

	fontNormal = fonsAddFont(fs, "sans", "DroidSansMono.ttf");
	if (fontNormal == FONS_INVALID) {
		assert(false);// Could not add font normal.
	}

	unsigned int white = glfonsRGBA(255, 255, 255, 255);

	fonsClearState(fs);
	fonsSetSize(fs, 20);
	fonsSetFont(fs, fontNormal);
	fonsSetColor(fs, white);
	fonsSetAlign(fs, FONS_ALIGN_LEFT | FONS_ALIGN_TOP);

	_textShader.compileShader("textRendering.vert", GLSLShader::VERTEX);
	_textShader.compileShader("textRendering.frag", GLSLShader::FRAGMENT);
	_textShader.link();
}

void ExampleApp::buildRachsis(float cx,float cy, float cz, float cx1, float cy1, float cz1, float r){

    int num_segments = 60;
    vec3 direction = vec3(cx1-cx, cy1-cy, cz1-cz);
    vec3 u = glm::normalize(direction); 
    vec3 w = glm::normalize(glm::cross( vec3(123, 231, 999), u)); 
    vec3 v_prep = glm::normalize(glm::cross(u, w));//x
    //Center point
    Mesh::Vertex vert;

    //new approach maybe
	vert.position = vert.position = vec3(cx, cy, cz);
	vert.normal = vec3(normalize(direction));
	cpuVertexArray.push_back(vert);
	int topCenterIndex = cpuVertexArray.size() - 1;

		for (int ii = 1; ii <= num_segments+2; ii += 1) {
			float theta = glm::two_pi<float>() * float(ii) / float(num_segments);//get the current angle
			vert.position = vec3(cx, cy, cz) + r * (v_prep * cos(theta) + w * sin(theta));
			vert.normal = normalize(direction);
			cpuVertexArray.push_back(vert);

			if (ii > 1) {
				int baseIndex = cpuVertexArray.size();
				cpuIndexArray.push_back(topCenterIndex);
				cpuIndexArray.push_back(baseIndex - 1);
				cpuIndexArray.push_back(baseIndex);
			}
		}

         for (int ii = 0; ii <= num_segments; ii += 1)  {
          float theta = glm::two_pi<float>() * float(ii) / float(num_segments);//get the current angle
          vert.position = vec3(cx, cy, cz) + r * (v_prep * cos(theta) + w * sin(theta));
             std::cout<<": "<<glm::to_string(vert.position)<<std::endl;
          vert.normal = normalize(vert.position-vec3(cx,cy,cz));
          cpuVertexArray.push_back(vert);
          vert.position = vec3(cx1, cy1, cz1) + r * (v_prep * cos(theta) + w * sin(theta));
          vert.normal = normalize((vert.position-vec3(cx1,cy1,cz1)));
          cpuVertexArray.push_back(vert);

          if (ii > 0){
              int baseIndex = cpuVertexArray.size() - 4;
              cpuIndexArray.push_back(baseIndex+3);
              cpuIndexArray.push_back(baseIndex);
              cpuIndexArray.push_back(baseIndex+1);
              cpuIndexArray.push_back(baseIndex + 2);
              cpuIndexArray.push_back(baseIndex);
              cpuIndexArray.push_back(baseIndex+3);
			  // Reverse it
			  cpuIndexArray.push_back(baseIndex + 3);
			  cpuIndexArray.push_back(baseIndex + 1);
			  cpuIndexArray.push_back(baseIndex);
			  cpuIndexArray.push_back(baseIndex + 2);
			  cpuIndexArray.push_back(baseIndex + 3);
			  cpuIndexArray.push_back(baseIndex);
            

          }
        }

		 vert.position = vert.position = vec3(cx1, cy1, cz1);
		 vert.normal = vec3(normalize(direction));
		 cpuVertexArray.push_back(vert);
		 int bottonCenterIndex = cpuVertexArray.size()-1;

		 for (int ii = 1; ii <= num_segments + 2; ii += 1) {
			 float theta = glm::two_pi<float>() * float(ii) / float(num_segments);//get the current angle
			 vert.position = vec3(cx1, cy1, cz1) + r * (v_prep * cos(theta) + w * sin(theta));
			 vert.normal = normalize(direction);
			 cpuVertexArray.push_back(vert);

			 if (ii > 1) {
				 int baseIndex = cpuVertexArray.size();
				 cpuIndexArray.push_back(bottonCenterIndex);
				 cpuIndexArray.push_back(baseIndex);
				 cpuIndexArray.push_back(baseIndex-1);
			 }
		 }
//
//        //Draw the bottom circle of the can
//        for (int ii = 1; ii < num_segments+2; ii += 1)  {
//          float theta = glm::two_pi<float>() * float(ii) / float(num_segments);//get the current angle
//          float x = r * cos(theta);//calculate the x component
//          float z = r * sin(theta);//calculate the z component
//          vert.position = vec3(x+ cx, cy, z + cz);
//          //vert.position = vec3(cx, cy, cz) + r * (w * cos(theta) + v_prep * sin(theta));
//          vert.normal = normalize(direction);
//          vert.texCoord0 = glm::vec2(ii/num_segments, 1);
//          cpuVertexArray.push_back(vert);
//          if (ii > 1){
//            cpuIndexArray.push_back(0);
//            cpuIndexArray.push_back(ii-1);
//            cpuIndexArray.push_back(ii);
//          }
//        }
//
//        int first = (int)cpuVertexArray.size();
//
//
//
//        //Draw the top circle of the can
//        for (int ii = 1; ii < num_segments+2; ii += 1)  {
//          float theta = glm::two_pi<float>() * float(ii) / float(num_segments);//get the current angle
//          float x = r * cos(theta);//calculate the x component
//          float z = r * sin(theta);//calculate the y component
//          vert.position = vec3(x+ cx1, cy1, z + cz1);
//          //vert.position = vec3(cx1, cy1, cz1) + r * (w * cos(theta) + v_prep * sin(theta));
//          vert.normal = normalize(direction);
//          vert.texCoord0 = glm::vec2(ii/num_segments, 1);
//          cpuVertexArray.push_back(vert);
//          if (ii > 1){
//            cpuIndexArray.push_back(first);
//            cpuIndexArray.push_back(first + ii);
//            cpuIndexArray.push_back(first + ii-1);
//          }
//        }
//
//        // Draw the surrounding surface of the can
//        for (int ii = 0; ii < num_segments; ii++) {
//          float theta = glm::two_pi<float>() * float(ii + 1) / float(num_segments); // get the current angle
//          float x = r * cos(theta); // calculate the x component
//          float z = r * sin(theta); // calculate the z component
//          // Add TOP
//          vert.position = vec3(x + cx1, cy1, z + cz1);
//          vert.normal = normalize(vec3(x, 0, z));
//          vert.texCoord0 = glm::vec2(1-(ii + 1) / float(num_segments), 0);
//          cpuVertexArray.push_back(vert);
//          //ADD BOTTOM
//          vert.position = vec3(x + cx, cy, z + cz);
//          vert.normal = normalize(vec3(x, 0, z));
//          vert.texCoord0 = glm::vec2(1-(ii + 1) / float(num_segments), 1);
//          cpuVertexArray.push_back(vert);
//          if (ii > 0) {
//            int baseIndex = cpuVertexArray.size() - 4;
//            cpuIndexArray.push_back(baseIndex+3);
//            cpuIndexArray.push_back(baseIndex +1);
//            cpuIndexArray.push_back(baseIndex);
//            cpuIndexArray.push_back(baseIndex + 2);
//            cpuIndexArray.push_back(baseIndex + +3);
//            cpuIndexArray.push_back(baseIndex);
//          }
//        }
//
//}
//
//void ExampleApp::setCylinder(vec3 p1, vec3 p2){
//    vec3 difference = p2 - p1;
//    vec3 defaultVector = vec3(0,1,0);
//    vec3 rotationAxis = glm::cross(defaultVector, difference);
//
//    float angle = (180.0f/(glm::pi<float>())) * acos((glm::dot(defaultVector, difference))/(difference.length()));
//
//
//
//}
}
