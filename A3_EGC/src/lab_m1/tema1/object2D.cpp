#include "object2D.h"

#include <vector>
#include <iostream>



#include "core/engine.h"
#include "utils/gl_utils.h"

#define STEPS 50

Mesh* object2D::CreateElipse(
    const std::string &name,
    glm::vec3 center,
    float lengthX, float lengthY,
    glm::vec3 color,
    bool fill)
{
    // std::cout<<color<<"\n";
    std::vector<VertexFormat> vertices = { VertexFormat(center, color) };
    for(int i=1;i<=STEPS;i++)
        vertices.push_back(VertexFormat(glm::vec3(lengthX*sin((float)2*3.1415f*i/STEPS), lengthY*cos((float)2*3.1415f*i/STEPS), 0), color));

    std::vector<unsigned int> indices = { };

    for(int i=1;i<STEPS;i++)
    {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back((i+1)%(STEPS+1));
    }
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(STEPS);

    Mesh* square = new Mesh(name);

    if (!fill) {
        std::cout<<"AI UITAT SA IMPLEMENTEZI AICI \n";
        exit(0);
    }

    square->InitFromData(vertices, indices);
    return square;
}


Mesh* object2D::CreateTriangleEquilateral(
    const std::string &name,
    glm::vec3 center,
    float edgeLength,
    glm::vec3 color,
    bool fill)
{
	float radians=3.1415f/3;
	// center-=glm::vec3(edgeLength*sin(radians), edgeLength*cos(radians)/2, 0);
    std::vector<VertexFormat> vertices = {
		VertexFormat(center, color),
		VertexFormat(center+glm::vec3(edgeLength, 0, 0), color),
		VertexFormat(center+glm::vec3(edgeLength*cos(radians), edgeLength*sin(radians), 0), color),
	};
    std::vector<unsigned int> indices = { 0, 1, 2};


    Mesh* square = new Mesh(name);

    if (!fill) {
        std::cout<<"AI UITAT SA IMPLEMENTEZI AICI \n";
        exit(0);
    }

    square->InitFromData(vertices, indices);
    return square;
}


#define STEPS 50

float wingCurveFunction(float x)
{
	// cos(2x)^(8x) - 1.6sin(x)^(21x) - x/20
	// https://www.desmos.com/calculator/9g6rjisltj
	return pow(cos(2*x),8*x) - 1.6f*pow(sin(x),21*x) - x/20;
}

Mesh* object2D::CreateDuckWing(
    const std::string &name,
    glm::vec3 center,
    glm::vec3 color,
    bool fill)
{
	// center-=glm::vec3(edgeLength*sin(radians), edgeLength*cos(radians)/2, 0);
    std::vector<VertexFormat> vertices = { };
    std::vector<unsigned int> indices = { };

    // x for the highest point on the courve
	vertices.push_back(VertexFormat(glm::vec3(0.092, 0, 0), color));

	float x=0.004;
	float step=(0.553-x)/STEPS;

	for(int i=0;i<STEPS;i++, x+=step)
		vertices.push_back(VertexFormat(glm::vec3(x, wingCurveFunction(x), 0), color));

	for(int i=1;i<STEPS;i++)
	{
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i+1);
	}
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(1+STEPS);

    Mesh* square = new Mesh(name);

    if (!fill) {
        std::cout<<"AI UITAT SA IMPLEMENTEZI AICI \n";
        exit(0);
    }

    square->InitFromData(vertices, indices);
    return square;
}


Mesh* object2D::CreateTarget(
    const std::string &name,
    glm::vec3 center,
    float outerRadius, float innerRadius,
    glm::vec3 color,
    bool fill)
{
    // std::cout<<color<<"\n";
    std::vector<VertexFormat> vertices = {};
    for(int i=1;i<=STEPS;i++)
    {
        // std::cout<<"#"<<outerRadius*sin(RADIANS((float)360*i/STEPS))<<" "<<outerRadius*cos(RADIANS((float)360*i/STEPS))<<"\n";
        // std::cout<<"@"<<innerRadius*sin(RADIANS((float)360*i/STEPS))<<" "<<innerRadius*cos(RADIANS((float)360*i/STEPS))<<"\n";
        vertices.push_back(VertexFormat(
            center+glm::vec3(
                innerRadius*sin(RADIANS((float)360*i/STEPS)),
                innerRadius*cos(RADIANS((float)360*i/STEPS)),
                0
            ),
            color));
        vertices.push_back(VertexFormat(
            center+glm::vec3(
                outerRadius*sin(RADIANS((float)360*i/STEPS)),
                outerRadius*cos(RADIANS((float)360*i/STEPS)),
                0
            ),
            color));

    }

    std::vector<unsigned int> indices = { };

    for(int i=0;i<STEPS-1;i++)
    {
        // indices.push_back(2*(i+1)  );
        indices.push_back(2*(i+1)  );
        indices.push_back(2* i   +1);
        indices.push_back(2* i     );

        indices.push_back(2* i   +1);
        indices.push_back(2*(i+1)  );
        indices.push_back(2*(i+1)+1);
        // std::cout<<2* i   +1<<" "<<
        //            2*(i+1)  <<" "<<
        //            2*(i+1)+1<<"\n";
    }
    // int i=10;
    // std::cout<<RADIANS((float)360*i/STEPS)<<" "<<cos(RADIANS((float)360*i/STEPS))<<" "<<outerRadius*cos(RADIANS((float)360*i/STEPS))<<"\n";
    // std::cout<<"$"<<vertices[0].position<<"\n";
	// std::cout<<"$"<<vertices[1].position<<"\n";
	// std::cout<<"$"<<vertices[2].position<<"\n";
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2*STEPS-1);
    indices.push_back(2*STEPS-1);
    indices.push_back(2*STEPS-2);
    indices.push_back(0);
    // indices.push_back(3);
    // indices.push_back(STEPS);

    Mesh* square = new Mesh(name);

    if (!fill) {
        std::cout<<"AI UITAT SA IMPLEMENTEZI AICI \n";
        exit(0);
    }

    square->InitFromData(vertices, indices);
    return square;
}



float buletCurveFunction(float x)
{
	// cos(2x)^(8x) - 1.6sin(x)^(21x) - x/20
	// https://www.desmos.com/calculator/c3eniond7t
	#define BEFORE_DENT 1
	#define DENT_RADIUS 1
	#define AFTER_DENT 2
	#define FACE_SCALE 1.3f

	#define RADIUS 3

	if(x<=BEFORE_DENT)
		return RADIUS;
	x-=BEFORE_DENT;

	x-=DENT_RADIUS;
	if(x<=DENT_RADIUS)
		return RADIUS-DENT_RADIUS*sqrt(1-x*x);
	x-=DENT_RADIUS;

	if(x<=AFTER_DENT)
		return RADIUS;
	x-=AFTER_DENT;
	// std::cout<<"#"<<x<<"\n";
	return sqrt(RADIUS*RADIUS-(x*x/FACE_SCALE/FACE_SCALE));
	#define BULLED_LENGTH (BEFORE_DENT+DENT_RADIUS+DENT_RADIUS+AFTER_DENT+RADIUS*FACE_SCALE)
}

#undef STEPS
#define STEPS 300

Mesh* object2D::CreateBuletIcon(
	const std::string &name,
	glm::vec3 center,
	glm::vec3 color,
	bool fill)
{
	// center-=glm::vec3(edgeLength*sin(radians), edgeLength*cos(radians)/2, 0);
    std::vector<VertexFormat> vertices = { };
    std::vector<unsigned int> indices = { };

    // x for the highest point on the courve

	float x=0;
	float step=(float)BULLED_LENGTH/(STEPS);
	// std::cout<<step<<"\n";

	for(int i=0;i<STEPS;i++, x+=step)
	{
		vertices.push_back(VertexFormat(glm::vec3(x, buletCurveFunction(x), 0), color));
		vertices.push_back(VertexFormat(glm::vec3(x, -buletCurveFunction(x), 0), color));
		// std::cout<<x<<" ";
		// std::cout<<buletCurveFunction(x)<<" ";
	}

	// std::cout<<"\n";
	for(int i=1;i<STEPS;i++)
	{
		indices.push_back(2*(i+1)  );
		indices.push_back(2* i   +1);
		indices.push_back(2* i     );

		indices.push_back(2* i   +1);
		indices.push_back(2*(i+1)  );
		indices.push_back(2*(i+1)+1);
	}

	// indices.push_back(2* STEPS   -1);
	// indices.push_back(2*(STEPS-1)  );
	// indices.push_back(2*(STEPS-1)-1);

    Mesh* square = new Mesh(name);

    if (!fill) {
        std::cout<<"AI UITAT SA IMPLEMENTEZI AICI \n";
        exit(0);
    }

    square->InitFromData(vertices, indices);
    return square;
}
