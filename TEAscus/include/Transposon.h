// *********************************************************************
//
// transposon.h
//
// Created by: Elie Dolgin, University of Edinburgh
//
// First started: March 11, 2005
// Last edited:
//
// *********************************************************************

#ifndef TRANSPOSON_H_EDOLGIN_TE
#define TRANSPOSON_H_EDOLGIN_TE


class Transposon
{
  public:
	Transposon();
	Transposon(int, double, bool);
	int GetLocation() const;
	bool GetEffect() const;
	double GetTranspositionRate() const;

  private:
	int location;		// location along chromosome
	double u; // transposition rate
	bool effect;		// true if TE effects fitness

};


#endif