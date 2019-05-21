#pragma once

enum EntityType
{
	BALL = 0,
	GOAL,
	AGENT
};

struct b2UserData
{
	b2UserData(EntityType type) : entity_type(type) {}
	b2UserData(EntityType type, void* pData) : entity_type(type), data(pData) {}

	EntityType entity_type;
	void* data;
};