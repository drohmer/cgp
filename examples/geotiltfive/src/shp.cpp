#include "shp.hpp"

#include "third_party/src/shapelib/shapefil.h"
#include "cgp/cgp.hpp"

namespace cgp
{

	numarray<polygon> polygon_load_file_shp(const std::string& filename)
	{
		assert_file_exist(filename);

		SHPHandle shp = SHPOpen(filename.c_str(), "rb");
		assert_cgp_no_msg(shp != NULL);

		int		nShapeType, nEntities;
		double 	adfMinBound[4], adfMaxBound[4];
		SHPGetInfo(shp, &nEntities, &nShapeType, adfMinBound, adfMaxBound);

		assert_cgp_no_msg(nShapeType != 1); // points

		std::cout << "Reading " << filename << " (type=" << nShapeType << ") ... " << std::flush;

		numarray<polygon> polygons;
		for (int i = 0; i < nEntities; i++)
		{
			SHPObject* obj = SHPReadObject(shp, i);
			polygon poly;
			poly.ring.resize(obj->nParts);
			for (int j = 0; j < obj->nParts; j++) {
				int start = obj->panPartStart[j];
				int end = (j + 1 == obj->nParts) ? obj->nVertices : obj->panPartStart[j + 1];
				for (int k = start; k < end; ++k)
				{
					poly.ring[j].push_back(vec3(obj->padfX[k], obj->padfY[k], obj->padfZ[k]));
				}
			}
			SHPDestroyObject(obj);
			polygons.push_back(poly);
		}

		SHPClose(shp);

		std::cout << "OK" << std::endl;

		return polygons;
	}

	numarray<vec3> points_load_file_shp(const std::string& filename)
	{
		assert_file_exist(filename);

		SHPHandle shp = SHPOpen(filename.c_str(), "rb");
		assert_cgp_no_msg(shp != NULL);

		int		nShapeType, nEntities;
		double 	adfMinBound[4], adfMaxBound[4];
		SHPGetInfo(shp, &nEntities, &nShapeType, adfMinBound, adfMaxBound);

		assert_cgp_no_msg(nShapeType == 1); // points

		std::cout << "Reading " << filename << " (type=" << nShapeType << ") ... " << std::flush;

		numarray<vec3> points;
		for (int i = 0; i < nEntities; i++)
		{
			SHPObject* obj = SHPReadObject(shp, i);
			vec3 point(obj->padfX[0], obj->padfY[0], obj->padfZ[0]);
			points.push_back(point);
			SHPDestroyObject(obj);
		}

		SHPClose(shp);

		std::cout << "OK" << std::endl;

		return points;
	}

}
