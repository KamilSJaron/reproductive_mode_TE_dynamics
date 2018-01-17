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
#include <iostream>

Transposon::Transposon():
	location(0),
	u(0),
	effect(false)
	{}

Transposon::Transposon(int site, double transposition, bool e):
	location(site),
	u(transposition),
	effect(e)
	{}

int Transposon::GetLocation() const {
	return location;
}

bool Transposon::GetEffect() const {
	return effect;
}

double Transposon::GetTranspositionRate() const {
	return u;
}