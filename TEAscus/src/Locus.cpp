// *********************************************************************
//
// locus.cpp
//
// Created by: Elie Dolgin, University of Edinburgh
//
// First started: March 11, 2005
// Last edited:
//
// *********************************************************************

#include <iostream>

#include "../include/Locus.h"

//Locus::Locus():
//	transElem(0),
//	next(0)
//	{}

Locus::Locus(Transposon te):
	transElem(te),
	next(0) { }

Locus::Locus(Transposon te, Locus * n):
	transElem(te),
	next(n)
	{}

Locus::Locus(const Locus & rhs) {
	transElem = rhs.GetTransposon();
	next = rhs.GetNext();
}

Locus::~Locus() {
	delete next;
	next = 0;
	//std::cout << "Locus destructor w/ te: " << GetTransposon().GetLocation() << std::endl;
}

Transposon Locus::GetTransposon() const {
	return transElem;
}

Locus * Locus::GetNext() const {
	return next;
}

int Locus::GetPosition() const {
	return transElem.GetLocation();
}

double Locus::GetTranspositionRate() const {
	return transElem.GetTranspositionRate();
}

void Locus::SetNext(Locus * loc) {
	next = loc;
}