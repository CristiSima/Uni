#include "lab_m1/tema2/car.h"

#include <vector>
#include <iostream>
#include <stdlib.h>

#include "transform2D.h"

#include "utils/glm_utils.h"

using namespace std;
using namespace m1;


// float mod(float a)
// {
//     if(a>0)
//         return a;
//     return -a;
// }

#define rand_in_range(start, fin) ((int)(start)) + ( std::rand() % ((int) ((int)(fin)) - ((int)(start)) + 1 ) )


Car::Car()
{
	baseMatrix= glm::mat4(1);
	direction=glm::vec4(1, 0, 0, 0);
	velocity=position=glm::vec4(0);
	max_speed=20;
	acceleration=5;
	min_speed_4_truning=1;
	forward_angle=0;

	max_turining_speed=1*RADIANS(45);
	min_turining_speed=1*RADIANS(30);

	perpendicular_friction=10;
	forward_friction=1;
}



Car::~Car()
{

}
