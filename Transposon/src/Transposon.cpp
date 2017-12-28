// *********************************************************************
//
// transposon.cpp
//
// Created by: Elie Dolgin, University of Edinburgh
//
// First started: March 11, 2005
// Last edited:
//
// *********************************************************************

#include "../include/Transposon.h"

Transposon::Transposon():
  location(0),
  effect(false)
  {}

Transposon::Transposon(int site, bool e):
  location(site),
  effect(e)
  {}

int Transposon::GetLocation() const
{
	return location;
}

bool Transposon::GetEffect() const
{
	return effect;
}
