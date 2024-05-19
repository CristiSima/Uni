#include "race_map.h"
#include "transform2D.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "core/engine.h"
#include "utils/gl_utils.h"

#include "utils/text_utils.h"

#define STEPS 50
#define ROUND_SEGMENT_ANGLE 2
#define STREIGHT_SEGMENT_DISTANCE 0.001f

#define ROAD_WIDTH 0.1f

#define debug(var) std::cout<<#var<<":"<<__LINE__<<"\t"<<var<<"\n";

#define SCALE_FACTOR 50

#define rand_in_range(start, fin) (((int)(start)) + ( std::rand() % ((int) ((int)(fin)) - ((int)(start)) + 1 ) ))

std::vector<glm::vec3> fix_coords(std::vector<glm::vec3> old_coords)
{
	std::vector<glm::vec3> new_coords;
	for(auto old_point:old_coords)
		new_coords.push_back(glm::vec3(
			old_point.x*SCALE_FACTOR,
			ROAD_ABOVE_GROUND,
			old_point.y*SCALE_FACTOR
		));

	return new_coords;
}

void populate_tree_positions(tema2::RaceMap &race_map);



tema2::RaceMap tema2::LoadMap(
    const std::string &name,
    const std::string &file_path,
    glm::vec3 color,
    bool fill)
{
	std::vector<VertexFormat> vertices = {  };
	RaceMap race_map;
	std::vector<glm::vec3> center_points = {  };
	std::vector<glm::vec3> left_side_points = {  };
    std::vector<glm::vec3> right_side_points = {  };

	system("ls");
	system("pwd");
	// std::ifstream in(PATH_JOIN("/home/student/gfx/local/src/lab_m1/tema2", "tracks", file_path));
	std::ifstream in(file_path);

	int track_components=0;
	float distance, angle, radius;
	float total_interior_angle=0;

	int steps;
	int total_steps=0;

	in>>track_components;



	glm::vec3 last_pos=glm::vec3(0, 0, 0);
	glm::vec3 direction=glm::vec3(1, 0, 0);
	glm::vec3 road_perpendicular=glm::vec3(0, 1, 0);

	glm::vec3 circle_center, circle_enter, circle_leave;
	glm::vec3 next_edge, next_direction, next_road_perpendicular;
	glm::vec3 center_pos, curve_perpendicular, start_radius;
	glm::mat3 temp_rotate;

	std::cout << track_components << '\n';
	for(int i=1;i<track_components;i++)
	{
		in>>distance>>angle>>radius;
		if(angle>0)
			total_interior_angle+=180-angle;
		else if(angle<0)
			total_interior_angle+=180-angle; // -- == +
			// {std::cout<<"IMPLEMENTEAZA SI AICI BAI\n";exit(-1);}


		next_edge=last_pos+float(distance)*direction;

		next_direction=direction*transform2D::Rotate(RADIANS(angle));
		next_road_perpendicular=road_perpendicular*transform2D::Rotate(RADIANS(angle));

		// std::cout<<direction*transform2D::Rotate(RADIANS((float)angle/2))<<"\n";

		if(angle>0)
			circle_center=next_edge- float(radius)*road_perpendicular;
		else
			circle_center=next_edge+ float(radius)*road_perpendicular;
		circle_enter=next_edge;

		steps=float(distance)/STREIGHT_SEGMENT_DISTANCE;
		total_steps+=steps+1;
		// std::cout<<"STREIGHT\n";
		for(int i=0;i<=steps;i++)
		{
			center_pos=last_pos+direction*((float)i*STREIGHT_SEGMENT_DISTANCE);
			center_points.push_back(center_pos);

			left_side_points.push_back(center_pos+road_perpendicular*ROAD_WIDTH);
			// vertices.push_back(VertexFormat(
			// 	center_pos+road_perpendicular*ROAD_WIDTH,
			// 	color
			// ));

			right_side_points.push_back(center_pos-road_perpendicular*ROAD_WIDTH);
			// vertices.push_back(VertexFormat(
			// 	center_pos-road_perpendicular*ROAD_WIDTH,
			// 	color
			// ));
			// std::cout<<center_pos+road_perpendicular<<" "<<center_pos-road_perpendicular<<"\n";
		}

		steps=abs(angle)/ROUND_SEGMENT_ANGLE;
		total_steps+=steps+1;
		start_radius=circle_enter-circle_center;
		// std::cout<<"ROUND\n";
		// std::cout<<circle_enter<<"\n";
		// std::cout<<circle_center<<"\n";
		// std::cout<<start_radius<<"\n";
		for(int i=0;i<=steps;i++)
		{
			temp_rotate=transform2D::Rotate(RADIANS((float)angle*i/steps));
			center_pos=circle_center+start_radius*temp_rotate;
			curve_perpendicular=road_perpendicular*temp_rotate;
			center_points.push_back(center_pos);

			left_side_points.push_back(center_pos+curve_perpendicular*ROAD_WIDTH);
			// vertices.push_back(VertexFormat(
			// 	center_pos+curve_perpendicular*ROAD_WIDTH,
			// 	color
			// ));
			right_side_points.push_back(center_pos-curve_perpendicular*ROAD_WIDTH);
			// vertices.push_back(VertexFormat(
			// 	center_pos-curve_perpendicular*ROAD_WIDTH,
			// 	color
			// ));
			// std::cout<<center_pos+curve_perpendicular<<" "<<center_pos-curve_perpendicular<<"\n";
		}
		circle_leave=center_pos;
		last_pos=circle_leave;
		direction=next_direction;
		road_perpendicular=next_road_perpendicular;
	}

	float a=direction.y/direction.x;
	float b=last_pos.y-a*last_pos.x;

	float dist=(-b/a -last_pos.x);
	// std::cout << "LINE\n";

	// std::cout<<a<<"\n";
	// std::cout<<b<<"\n";
	// std::cout<<"D"<<dist<<"\n";
	// std::cout<<last_pos<<"\n";
	// std::cout<<direction<<"\n";
	in>>radius;
	// std::cout<<"R"<<radius<<"\n";

	// while(total_interior_angle>360)
	// 	total_interior_angle-=360;
	// while(total_interior_angle<-360)
	// 	total_interior_angle+=360;

	// std::cout<<"A"<<total_interior_angle<<"\n";

	// angle=360-90-total_interior_angle;
	// (2-n)*180
	angle=(track_components-2)*180-total_interior_angle;
	angle=180-angle;
	std::cout<<"A"<<angle<<"\n";
	// dist=dist/cos(angle);
	std::cout<<"D"<<dist<<"\n";

	// if(angle==90)
	// 	dist=abs(last_pos.y);
	// else
		dist=abs((double)last_pos.y/sin(RADIANS(angle)));

	std::cout<<"D"<<dist<<"\n";

	float dist_from_Ox=0;
	dist_from_Ox+=radius;
	dist_from_Ox+=abs(road_perpendicular.y);
	// debug(radius);
	// debug(dist_from_Ox);
	// debug(road_perpendicular);

	float dist_bef=abs(dist)-float(dist_from_Ox);
	// debug(dist);
	// debug(dist_bef);
	steps=(dist_bef)/STREIGHT_SEGMENT_DISTANCE;
	total_steps+=steps+1;
	std::cout<<"STREIGHT "<<steps<<" \n";
	std::cout<<abs(dist)<<" \n";
	std::cout<<dist_from_Ox<<" \n";
	for(int i=0;i<=steps;i++)
	{
		center_pos=last_pos+direction*((float)i*STREIGHT_SEGMENT_DISTANCE);
		if(i==steps){
			debug(center_pos);
			center_pos=last_pos+direction*dist_bef;
			debug(center_pos);
			// center_pos-=direction*(center_pos.y+float(dist_from_Ox));
			// center_pos-=direction*(center_pos.y+float(dist_from_Ox));
			debug(center_pos);
		}
		center_points.push_back(center_pos);

		left_side_points.push_back(center_pos+road_perpendicular*ROAD_WIDTH);
		// vertices.push_back(VertexFormat(
		// 	center_pos+road_perpendicular*ROAD_WIDTH,
		// 	color
		// ));
		right_side_points.push_back(center_pos-road_perpendicular*ROAD_WIDTH);
		// vertices.push_back(VertexFormat(
		// 	center_pos-road_perpendicular*ROAD_WIDTH,
		// 	color
		// ));
		// std::cout<<center_pos<<"\n";
		// std::cout<<center_pos+road_perpendicular<<" "<<center_pos-road_perpendicular<<"\n";
	}

	circle_enter=center_pos;
	std::cout<<circle_enter<<"\n";

	if(angle>0){
		circle_center=circle_enter- float(radius)*road_perpendicular;
		// angle=180-angle;
	}
	else{
		circle_center=circle_enter+ float(radius)*road_perpendicular;
		// angle=angle+180;
	}
	std::cout<<"A"<<angle<<"\n";


	steps=abs(angle)/ROUND_SEGMENT_ANGLE;
	total_steps+=steps+1;
	int alignment=sign(circle_enter).y;
	start_radius=circle_enter-circle_center;
	std::cout<<"ROUND\n";
	std::cout<<circle_enter<<"\n";
	std::cout<<circle_center<<"\n";
	std::cout<<start_radius<<"\n";
	for(int i=0;i<=steps;i++)
	{
		temp_rotate=transform2D::Rotate(RADIANS((float)(angle)*i/steps));
		if(i==steps)
			temp_rotate=transform2D::Rotate(RADIANS((float)(angle)));

		center_pos=circle_center+start_radius*temp_rotate;
		curve_perpendicular=road_perpendicular*temp_rotate;
		if(i==steps)
			center_pos.y=0;

		if(sign(center_pos).y!=alignment)
			center_pos.y=0;

		center_points.push_back(center_pos);

		left_side_points.push_back(center_pos+curve_perpendicular*ROAD_WIDTH);
		// vertices.push_back(VertexFormat(
		// 	center_pos+curve_perpendicular*ROAD_WIDTH,
		// 	color
		// ));
		right_side_points.push_back(center_pos-curve_perpendicular*ROAD_WIDTH);
		// vertices.push_back(VertexFormat(
		// 	center_pos-curve_perpendicular*ROAD_WIDTH,
		// 	color
		// ));
		// std::cout<<center_pos+curve_perpendicular<<" "<<center_pos-curve_perpendicular<<"\n";
	}

	last_pos=center_pos;
	road_perpendicular=glm::vec3(0, 1, 0);

	dist=abs(last_pos.x);
	steps=(dist)/STREIGHT_SEGMENT_DISTANCE;
	total_steps+=steps;
	std::cout<<"STREIGHT "<<steps<<" \n";
	std::cout<<road_perpendicular<<"\n";
	std::cout<<last_pos<<"\n";
	for(int i=0;i<steps;i++)
	{
		center_pos=last_pos*(1-(float)i/steps);
		center_points.push_back(center_pos);

		left_side_points.push_back(center_pos+road_perpendicular*ROAD_WIDTH);
		// vertices.push_back(VertexFormat(
		// 	center_pos+road_perpendicular*ROAD_WIDTH,
		// 	color
		// ));
		right_side_points.push_back(center_pos-road_perpendicular*ROAD_WIDTH);
		// vertices.push_back(VertexFormat(
		// 	center_pos-road_perpendicular*ROAD_WIDTH,
		// 	color
		// ));
		// std::cout<<center_pos<<"\n";
		// std::cout<<center_pos+road_perpendicular<<" "<<center_pos-road_perpendicular<<"\n";
	}
	// fix the fact I'm bad at math
	swap(left_side_points, right_side_points);

	race_map.segments=total_steps;
	race_map.segment_width=SCALE_FACTOR*ROAD_WIDTH;
	race_map.center_points=fix_coords(center_points);
	race_map.left_side_points=fix_coords(left_side_points);
	race_map.right_side_points=fix_coords(right_side_points);

	for(int i=0;i<total_steps;i++)
		for(auto side_points_vector: {race_map.left_side_points, race_map.right_side_points})
			vertices.push_back( VertexFormat( side_points_vector[i], color));

    std::vector<unsigned int> indices = { };

    for(int i=0;i<total_steps-1;i++)
    {
        indices.push_back(2*i);
        indices.push_back(2*i+1);
        indices.push_back(2*i+2);

        indices.push_back(2*i+1);
        indices.push_back(2*i+2);
		indices.push_back(2*i+3);
    }

    indices.push_back(2*total_steps-2);
	indices.push_back(2*total_steps-1);
    indices.push_back(0);
	//
	indices.push_back(2*total_steps-1);
    indices.push_back(0);
	indices.push_back(1);
    // indices.push_back(0);
    // indices.push_back(1);
    // indices.push_back(STEPS);



	int moving_obstacles;
	in>>moving_obstacles;
	race_map.moving_obstacles=std::vector<tema2::moving_obstacle>(moving_obstacles);
	for(int i=0;i<moving_obstacles;i++)
		in>>race_map.moving_obstacles[i].offset;

	for(int i=0;i<moving_obstacles;i++)
		race_map.moving_obstacles[i].segment_id=rand_in_range(3, race_map.center_points.size()-3);
	race_map.moving_obstacles[0].segment_id=3;

	for(int i=0;i<moving_obstacles;i++)
		race_map.moving_obstacles[i].pos=race_map.center_points[race_map.moving_obstacles[i].segment_id];

    race_map.map_mesh = new Mesh(name);

    if (!fill) {
        std::cout<<"AI UITAT SA IMPLEMENTEZI AICI \n";
        exit(0);
    }

	populate_tree_positions(race_map);
    race_map.map_mesh->InitFromData(vertices, indices);
    return race_map;
}


#define TREE_NUMBER 500
void populate_tree_positions(tema2::RaceMap &race_map) {
	float max_x=FLT_MIN, max_z=FLT_MIN;
	float min_x=FLT_MAX, min_z=FLT_MAX;

	#define PRECISION 100
	#define CLEAR_RADIUS_MULTIPLYER 2

	#define local_rand_in_range(a, b) ((float)rand_in_range(int(a*PRECISION), int(b*PRECISION))/PRECISION)


	for(auto pos: race_map.center_points)
	{
		max_x=std::max(max_x, pos.x);
		max_z=std::max(max_z, pos.z);

		min_x=std::min(min_x, pos.x);
		min_z=std::min(min_z, pos.z);
	}
	max_x+=3*SCALE_FACTOR*ROAD_WIDTH*CLEAR_RADIUS_MULTIPLYER;
	max_z+=3*SCALE_FACTOR*ROAD_WIDTH*CLEAR_RADIUS_MULTIPLYER;
	min_x-=3*SCALE_FACTOR*ROAD_WIDTH*CLEAR_RADIUS_MULTIPLYER;
	min_z-=3*SCALE_FACTOR*ROAD_WIDTH*CLEAR_RADIUS_MULTIPLYER;
	race_map.tree_positions={
		// glm::vec3(max_x, 0, max_z),
		// glm::vec3(max_x, 0, min_z),
		// glm::vec3(min_x, 0, max_z),
		// glm::vec3(min_x, 0, min_z),
	};
	bool pos_ok;
	glm::vec3 pos;
	for (int tree_id = 0; tree_id < TREE_NUMBER; tree_id++) {
		do
		{
			pos_ok=true;
			pos=glm::vec3(
				local_rand_in_range(min_x, max_x),
				0,
				local_rand_in_range(min_z, max_z)
			);
			for(auto track_pos:race_map.center_points)
				if(glm::length(track_pos-pos)<SCALE_FACTOR*ROAD_WIDTH*CLEAR_RADIUS_MULTIPLYER)
				{
					pos_ok=false;
					break;
				}
		}while(!pos_ok);
		// debug(pos);
		race_map.tree_positions.push_back(pos);
	}
}

#define TREE_HEIGHT 10
#define TREE_TRUNK_RADIUS 1
#define TREE_CORONA_RADIUS 4
#define CIRCLE_POINTS 30
Mesh* tema2::CreateTree(const std::string &name, bool fill)
{
	std::vector<VertexFormat> vertices = {  };

	glm::vec3 color_brown=NormalizedRGB(165, 42, 42);
	glm::vec3 color_green=NormalizedRGB(97, 138, 61);

	int steps;
	int total_steps=0;
	for(int i=0;i<CIRCLE_POINTS;i++)
	{
		vertices.push_back( VertexFormat( glm::vec3(
			TREE_TRUNK_RADIUS*sin(RADIANS(360)*(float)i/CIRCLE_POINTS),
			0,
			TREE_TRUNK_RADIUS*cos(RADIANS(360)*(float)i/CIRCLE_POINTS)
		), color_brown));

		vertices.push_back( VertexFormat( glm::vec3(
			TREE_TRUNK_RADIUS*sin(RADIANS(360)*(float)i/CIRCLE_POINTS),
			TREE_HEIGHT,
			TREE_TRUNK_RADIUS*cos(RADIANS(360)*(float)i/CIRCLE_POINTS)
		), color_brown));
	}
	total_steps+=2*CIRCLE_POINTS;

	// std::cout << "START" << '\n';
	for(int i=0;i<CIRCLE_POINTS;i++)
	{
		float angle1=RADIANS(180) * (float)i / CIRCLE_POINTS;
		for(int j=0;j<CIRCLE_POINTS;j++)
		{
			float angle2=RADIANS(360) * (float)j / CIRCLE_POINTS;

			vertices.push_back( VertexFormat( glm::vec3(
				TREE_CORONA_RADIUS				*sin(angle1)*cos(angle2),
				TREE_HEIGHT+TREE_CORONA_RADIUS	*cos(angle1),
				TREE_CORONA_RADIUS				*sin(angle1)*sin(angle2)
			), color_green));
		}
	}
	// debug(vertices.size());

    std::vector<unsigned int> indices = { };

	for(int i=0;i<CIRCLE_POINTS-1;i++)
	{
		indices.push_back(2*i);
		indices.push_back(2*i+1);
		indices.push_back(2*i+2);


		indices.push_back(2*i+1);
		indices.push_back(2*i+2);
		indices.push_back(2*i+3);
	}

	indices.push_back(2*CIRCLE_POINTS-2);
	indices.push_back(2*CIRCLE_POINTS-1);
	indices.push_back(0);

	indices.push_back(2*CIRCLE_POINTS-1);
	indices.push_back(0);
	indices.push_back(1);


	for(int i=1;i<CIRCLE_POINTS;i++)
	{
		for(int j=1;j<CIRCLE_POINTS;j++)
		{
			indices.push_back(total_steps+ i   *CIRCLE_POINTS+j  );
			indices.push_back(total_steps+ i   *CIRCLE_POINTS+j-1);
			indices.push_back(total_steps+(i-1)*CIRCLE_POINTS+j-1);

			indices.push_back(total_steps+ i   *CIRCLE_POINTS+j  );
			indices.push_back(total_steps+(i-1)*CIRCLE_POINTS+j-1);
			indices.push_back(total_steps+(i-1)*CIRCLE_POINTS+j  );
		}
		indices.push_back(total_steps+ i   *CIRCLE_POINTS    );
		indices.push_back(total_steps+ i   *CIRCLE_POINTS+CIRCLE_POINTS-1);
		indices.push_back(total_steps+(i-1)*CIRCLE_POINTS+CIRCLE_POINTS-1);

		indices.push_back(total_steps+ i   *CIRCLE_POINTS+0  );
		indices.push_back(total_steps+(i-1)*CIRCLE_POINTS+CIRCLE_POINTS-1);
		indices.push_back(total_steps+(i-1)*CIRCLE_POINTS+0  );
	}

	Mesh *map_mesh = new Mesh(name);

    map_mesh->InitFromData(vertices, indices);

	return map_mesh;
}

#define GRASS_DENSITY 100
Mesh* tema2::CreateGrass(const std::string &name, bool fill)
{
	std::vector<VertexFormat> vertices = {  };

	glm::vec3 color_green=NormalizedRGB(0x32, 0xcd, 0x32);

	int steps;
	int total_steps=0;
	for(int i=0;i<=GRASS_DENSITY;i++)
		for(int j=0;j<=GRASS_DENSITY;j++)
			vertices.push_back( VertexFormat( glm::vec3(
				(float)i/GRASS_DENSITY-0.5f,
				0,
				(float)j/GRASS_DENSITY-0.5f
			), color_green));

    std::vector<unsigned int> indices = { };


	for(int i=1;i<GRASS_DENSITY;i++)
	{
		for(int j=1;j<GRASS_DENSITY;j++)
		{
			indices.push_back( i   *GRASS_DENSITY+j  );
			indices.push_back( i   *GRASS_DENSITY+j-1);
			indices.push_back((i-1)*GRASS_DENSITY+j-1);

			indices.push_back( i   *GRASS_DENSITY+j  );
			indices.push_back((i-1)*GRASS_DENSITY+j-1);
			indices.push_back((i-1)*GRASS_DENSITY+j  );
		}
		indices.push_back( i   *GRASS_DENSITY    );
		indices.push_back( i   *GRASS_DENSITY+GRASS_DENSITY-1);
		indices.push_back((i-1)*GRASS_DENSITY+GRASS_DENSITY-1);

		indices.push_back( i   *GRASS_DENSITY+0  );
		indices.push_back((i-1)*GRASS_DENSITY+GRASS_DENSITY-1);
		indices.push_back((i-1)*GRASS_DENSITY+0  );
	}

	Mesh *mesh = new Mesh(name);

    mesh->InitFromData(vertices, indices);

	return mesh;
}

#define OBSTACLE_HEIGHT 0.75
#define OBSTACLE_WIDTH 1
#define OBSTACLE_LENGTH 2
Mesh* dymy(const std::string &name, bool fill)
{
	std::vector<VertexFormat> vertices = {  };

	glm::vec3 color=NormalizedRGB(0x32, 0xcd, 0x32);

	int steps;
	int total_steps=0;

	for(int z=0;z<=OBSTACLE_WIDTH;z++)
		for(int x=0;x<=OBSTACLE_LENGTH;x++)
			vertices.push_back( VertexFormat( glm::vec3(
				(float)x-OBSTACLE_LENGTH/2,
				(float)0,
				(float)z-OBSTACLE_WIDTH/2
			), color));

	for(int z=0;z<=OBSTACLE_WIDTH;z++)
		for(int x=0;x<=OBSTACLE_LENGTH;x++)
			vertices.push_back( VertexFormat( glm::vec3(
				(float)x-OBSTACLE_LENGTH/2,
				(float)OBSTACLE_HEIGHT,
				(float)z-OBSTACLE_WIDTH/2
			), color));

	for(int y=1;y<OBSTACLE_HEIGHT;y++)
	{
		for(int z=0;z<=OBSTACLE_WIDTH;z++)
			for(int x=0;x<=OBSTACLE_LENGTH;x++)
				vertices.push_back( VertexFormat( glm::vec3(
					(float)x-OBSTACLE_LENGTH/2,
					(float)y,
					(float)z-OBSTACLE_WIDTH/2
				), color));

	}

    std::vector<unsigned int> indices = { };


	for(int i=1;i<OBSTACLE_WIDTH;i++)
	{
		for(int j=1;j<OBSTACLE_LENGTH;j++)
		{
			indices.push_back( i   *GRASS_DENSITY+j  );
			indices.push_back( i   *GRASS_DENSITY+j-1);
			indices.push_back((i-1)*GRASS_DENSITY+j-1);

			indices.push_back( i   *GRASS_DENSITY+j  );
			indices.push_back((i-1)*GRASS_DENSITY+j-1);
			indices.push_back((i-1)*GRASS_DENSITY+j  );
		}
		indices.push_back( i   *GRASS_DENSITY    );
		indices.push_back( i   *GRASS_DENSITY+GRASS_DENSITY-1);
		indices.push_back((i-1)*GRASS_DENSITY+GRASS_DENSITY-1);

		indices.push_back( i   *GRASS_DENSITY+0  );
		indices.push_back((i-1)*GRASS_DENSITY+GRASS_DENSITY-1);
		indices.push_back((i-1)*GRASS_DENSITY+0  );
	}

	Mesh *mesh = new Mesh(name);

    mesh->InitFromData(vertices, indices);

	return mesh;
}
Mesh* tema2::CreateMovingObstacles(const std::string &name, glm::vec3 color, bool fill)
{
	std::vector<VertexFormat> vertices = {  };



	int steps;
	int total_steps=0;

	vertices.push_back( VertexFormat( glm::vec3(
		(float)-OBSTACLE_LENGTH/2,
		(float)0,
		(float)-OBSTACLE_WIDTH/2
	), color));
	vertices.push_back( VertexFormat( glm::vec3(
		(float)-OBSTACLE_LENGTH/2,
		(float)0,
		(float)OBSTACLE_WIDTH-OBSTACLE_WIDTH/2
	), color));
	vertices.push_back( VertexFormat( glm::vec3(
		(float)OBSTACLE_LENGTH-OBSTACLE_LENGTH/2,
		(float)0,
		(float)OBSTACLE_WIDTH-OBSTACLE_WIDTH/2
	), color));
	vertices.push_back( VertexFormat( glm::vec3(
		(float)OBSTACLE_LENGTH-OBSTACLE_LENGTH/2,
		(float)0,
		(float)-OBSTACLE_WIDTH/2
	), color));

	vertices.push_back( VertexFormat( glm::vec3(
		(float)-OBSTACLE_LENGTH/2,
		(float)OBSTACLE_HEIGHT,
		(float)-OBSTACLE_WIDTH/2
	), color));
	vertices.push_back( VertexFormat( glm::vec3(
		(float)-OBSTACLE_LENGTH/2,
		(float)OBSTACLE_HEIGHT,
		(float)OBSTACLE_WIDTH-OBSTACLE_WIDTH/2
	), color));
	vertices.push_back( VertexFormat( glm::vec3(
		(float)OBSTACLE_LENGTH-OBSTACLE_LENGTH/2,
		(float)OBSTACLE_HEIGHT,
		(float)OBSTACLE_WIDTH-OBSTACLE_WIDTH/2
	), color));
	vertices.push_back( VertexFormat( glm::vec3(
		(float)OBSTACLE_LENGTH-OBSTACLE_LENGTH/2,
		(float)OBSTACLE_HEIGHT,
		(float)-OBSTACLE_WIDTH/2
	), color));


    std::vector<unsigned int> indices = {
		0,  1,  2,
		2,  3,  0,

		4,  5,  6,
		6,  7,  4,

		0,  1,  4,
		1,  5,  4,

		1,  2,  5,
		2,  6,  5,

		2,  3,  6,
		3,  7,  6,

		3,  0,  7,
		0,  4,  7,
	};




	Mesh *mesh = new Mesh(name);

    mesh->InitFromData(vertices, indices);

	return mesh;
}
