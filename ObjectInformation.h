

#ifndef __OBJECT_INFORMATION_H__
#define __OBJECT_INFORMATION_H__

struct ObjectInformation
{
	unsigned int	verticeNumber;
	BoundingBox		bbox;
	std::string		shaderProgramStr;
	Vec3			position;
	LightingEffects postProcesses;

};

#endif //__OBJECT_INFORMATION_H__