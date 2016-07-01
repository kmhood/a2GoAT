#ifndef __PPi0Example_h__
#define __PPi0Example_h__

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

#include "GTreeManager.h"
#include "PPhysics.h"

class	PPi0Example  : public PPhysics
{
private:
    GH1*	time;
    GH1*	time_cut;
    GH1*	time_2g;      
    GH1*	time_2g_cut;   
     
    GH1*	IM;
    GH1*	IM_2g;

    GH1*	MM;
    GH1*	MM_2g;

    TH1*    h_theta;
    TH1*    h_theta_withcharged;
    TH1*    h_divided_pi0s;
    TH1*    protonAngles;
    TH1*    protonAngles_withcharged;
    TH1*    h_divided_protons;

    TH2*    protonAngles2D;
    TH2*    protonAngles2D_withCharged;
    TH2*    h_divided_protonAng2D;
    TH2*    proAngles_wc_diff;
    TH2*    angleSeparation;

    TH1*    tester;


protected:
    virtual Bool_t  Start();
    virtual void    ProcessEvent();
    virtual void	ProcessScalerRead();
    virtual Bool_t    Write();
			
public:
    PPi0Example();
    virtual ~PPi0Example();
    virtual Bool_t  Init();

};
#endif
