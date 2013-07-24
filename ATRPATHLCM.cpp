#include "ATRPATHLCM.h"

ATRPATHLCM::ATRPATHLCM(string name,BIGNUM *lcm, BIGNUM *p,int track)
{
	this->name = name;
	this->lcm = BN_new();
	this->p = BN_new();
	this->track = track;

	BN_copy(this->lcm,lcm);
	BN_copy(this->p,p);

}
ATRPATHLCM::ATRPATHLCM(string name,BIGNUM *p)
{
	this->name = name;
	this->lcm = BN_new();
	this->p = BN_new();
	this->track = -1;

	BN_one(this->lcm);
	BN_copy(this->p,p);
}
ATRPATHLCM::ATRPATHLCM()
{
	this->lcm = NULL;
	this->p = NULL;
}
ATRPATHLCM::ATRPATHLCM(const ATRPATHLCM &AM)
{
	this->name = AM.name;
	this->lcm = BN_new();
	this->p = BN_new();
	this->track = AM.track;

	BN_copy(this->lcm,AM.lcm);
	BN_copy(this->p,AM.p);

}
string ATRPATHLCM::GetName()
{
	return name;
}

BIGNUM * ATRPATHLCM::GetLCM()
{
	BIGNUM * copy = BN_new();
	BN_copy(copy,lcm);
	return copy; 
}

BIGNUM * ATRPATHLCM::GetP()
{
	BIGNUM * copy = BN_new();
	BN_copy(copy,p);
	return copy; 
}
int ATRPATHLCM::GetTrack()
{
	return track;
}

void  ATRPATHLCM::SetName(string name)
{
	this->name = name;
}

void  ATRPATHLCM::SetLCM(BIGNUM *lcm)
{
	BN_copy(this->lcm,lcm);
}

void  ATRPATHLCM::SetP(BIGNUM *p)
{
	BN_copy(this->p,p);
}

void ATRPATHLCM::SetTrack(int track)
{
	this->track = track;
}

void ATRPATHLCM::MulLagCoe(BIGNUM *lc)
{
    BN_CTX * ctx = BN_CTX_new();
    BN_mod_mul(lcm,lcm,lc,p,ctx);
}

ATRPATHLCM::~ATRPATHLCM()
{
    BN_free(lcm);
    BN_free(p);
    lcm = NULL;	
    p = NULL;
}

bool ATRPATHLCM::IsNULL()
{
	if(lcm == NULL
		&&p == NULL)
	{
		return true;
	}
	else
	{
		return false;
	}

}
ATRPATHLCM & ATRPATHLCM::operator= (const ATRPATHLCM &AM)
{
	this->name = AM.name;
	this->lcm = BN_new();
	this->p = BN_new();
	this->track = AM.track;

	BN_copy(this->lcm,AM.lcm);
	BN_copy(this->p,AM.p);
	
	return *this;
}



