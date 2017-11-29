// *********************************************************************
// 
// locus.h
// 
// Created by: Elie Dolgin, University of Edinburgh
//
// First started: March 11, 2005
// Last edited:
//
// *********************************************************************

#ifndef LOCUS_H_EDOLGIN_TE
#define LOCUS_H_EDOLGIN_TE

#include "transposon.h"


class Locus
{
  public:
	//Locus();
	Locus(Transposon te);
	Locus(Transposon te, Locus * n);
	Locus(const Locus &);
	~Locus();

	Transposon GetTransposon() const;
	Locus * GetNext() const;
	void SetNext(Locus * loc);
  
  private:
	Transposon transElem;
	Locus * next;

};


#endif