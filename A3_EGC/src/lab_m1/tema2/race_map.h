#pragma once

#include <string>
#include <vector>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

#define ROAD_ABOVE_GROUND 0.07f

namespace tema2
{

	struct moving_obstacle
	{
		float offset;
		// distance
		float segment_progress=0;
		int segment_id;

		float rotation_angle=0;
		glm::vec3 pos;
	};

	struct RaceMap
	{
		Mesh* map_mesh;
		int segments;
		float segment_width;

		std::vector<glm::vec3> center_points;
		std::vector<glm::vec3> left_side_points;
		std::vector<glm::vec3> right_side_points;

		std::vector<moving_obstacle> moving_obstacles;
		float moving_obstacles_speed=10;
		float moving_obstacles_radius=0.5;

	    std::vector<glm::vec3> tree_positions;
	};

	RaceMap LoadMap(const std::string &name, const std::string &file_path, glm::vec3 color, bool fill = true);
	Mesh* CreateTree(const std::string &name, bool fill=true);
	Mesh* CreateGrass(const std::string &name, bool fill=true);
	Mesh* CreateMovingObstacles(const std::string &name, glm::vec3 color, bool fill=true);

}   // namespace tema2
