// *********************************************************************
//
// chromosome.cpp
//
// Created by: Elie Dolgin, University of Edinburgh
//
// First started: March 11, 2005
// Last edited:
//
// *********************************************************************

#include <iostream>
#include "chromosome.h"


Chromosome::Chromosome():
  chromNumber(0),
  chromCopy(0),
  length(0),
  rChrom(0)
  {
    headLocus = 0;
  }

Chromosome::Chromosome(int num, int copy, int len, double r):
  chromNumber(num),
  chromCopy(copy),
  length(len),
  rChrom(r)
  {
	headLocus = 0;
  }

Chromosome::~Chromosome()
{
	delete headLocus;
	headLocus = 0;
}

unsigned int Chromosome::GetChromTECount() const
{
	unsigned int count = 0;
	Locus * current = headLocus;
	while (current != 0)
	{
		count++;
		current = current->GetNext();
	}
	return count;
}

unsigned int Chromosome::GetChromTECountAffectingFitness() const
{
	unsigned int count = 0;
	Locus * current = headLocus;
	while (current != 0)
	{
		if (current->GetTransposon().GetEffect())
		  count++;
		current = current->GetNext();
	}
	return count;
}

unsigned int Chromosome::GetChromNumber() const
{
	return chromNumber;
}

unsigned int Chromosome::GetChromCopy() const
{
	return chromCopy;
}

unsigned int Chromosome::GetLength() const
{
	return length;
}

double Chromosome::GetRecRate() const
{
	return rChrom;
}

Locus * Chromosome::GetHeadLocus() const
{
	return headLocus;
}

void Chromosome::SetHeadLocus(Locus * newHead)
{
	headLocus = newHead;
}

void Chromosome::SetChromNumberAndCopy(int num, int copy)
{
	chromNumber = num;
	chromCopy = copy;
}

void Chromosome::SetChromLengthAndRecRate(int len, double r)
{
	length = len;
	rChrom = r;
}

bool Chromosome::TestEmpty(int site) const
{
	Locus * current = headLocus;

	while ((current != 0) && (current->GetTransposon().GetLocation() < site))
	  current = current->GetNext();

	if (current == 0)
	  return true;
	if (current->GetTransposon().GetLocation() == site)
	  return false;
	else
	  return true;
}

void Chromosome::Insert (Transposon te)
{
	Locus * newLocus = new Locus (te);
	int newLocation = te.GetLocation();

	if (headLocus == 0)
	{
		headLocus = newLocus;
		return;
	}

	// if the new inserted locus is upstream of head, then make it the new head
	if (headLocus->GetTransposon().GetLocation() > newLocation)
	{
		newLocus->SetNext(headLocus);
		headLocus = newLocus;
		return;
	}


	Locus * currentLocus = headLocus;
	Locus * nextLocus = 0;
	int nextLocation = 0;

	for(;;)
	{
		// if there is no next, put new inserted locus at the end
		if (currentLocus->GetNext() == 0)
		{
			currentLocus->SetNext(newLocus);
			return;
		}

		nextLocus = currentLocus->GetNext();
		nextLocation = nextLocus->GetTransposon().GetLocation();

		if (nextLocation > newLocation)
		{
			currentLocus->SetNext(newLocus);
			newLocus->SetNext(nextLocus);
			return;
		}
		currentLocus = nextLocus;
	}
}

void Chromosome::Delete(int pos)
{
	if ((pos > GetChromTECount()) || pos < 1)
	{
		std::cout << "Deletion error." << std::endl;
		return;
	}

	Locus * current = headLocus;

	if (pos == 1) // delete headLocus
	{
		headLocus = headLocus->GetNext();
		current->SetNext(0);
		delete current;
		current = 0;
		return;
	}

	for (int i=2; i<pos; i++)
		current = current->GetNext();

	Locus * next = current->GetNext();
	current->SetNext(next->GetNext());
	next->SetNext(0);
	delete next;
	next=0;
	return;
}

void Chromosome::ListChromSites() const
{
	Locus * loc = headLocus;

	std::cout << "Strand [" << chromNumber << "," << chromCopy << "]: ";
	if (loc != 0)
	{
	  while (loc->GetNext() != 0)
	  {
		std::cout<<loc->GetTransposon().GetLocation()<<", ";
		loc = loc->GetNext();
	  }
	  std::cout<<loc->GetTransposon().GetLocation()<<".";
	}
	std::cout << std::endl;
}