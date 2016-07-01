#include "PPi0Example.h"

PPi0Example::PPi0Example()
{ 
    time 	= new GH1("time", 	"time", 	1400, -700, 700);
    time_cut 	= new GH1("time_cut", 	"time_cut", 	1400, -700, 700);

    time_2g 	= new GH1("time_2g",	"time_2g", 	1400, -700, 700);
    time_2g_cut = new GH1("time_2g_cut","time_2g_cut", 	1400, -700, 700);

    IM 		= new GH1("IM", 	"IM", 		400,   0, 400);
    IM_2g 	= new GH1("IM_2g", 	"IM_2g", 	400,   0, 400);
  
    MM		= new GH1("MM", 	"MM", 	 	400,   800, 1200);     
    MM_2g	= new GH1("MM_2g", 	"MM_2g", 	400,   800, 1200);

    h_theta = new TH1D("Theta", "Theta", 180,0,180);
    h_theta_withcharged = new TH1D("Theta_wCharge", "Theta_wCharge", 180,0,180);

    h_divided_pi0s = new TH1D("eff_pi0", "eff_pi0", 180,0,180);

    protonAngles = new TH1D("proton", "proton", 180,0,180);
    protonAngles->Sumw2();

    protonAngles_withcharged = new TH1D("proton_wc", "proton_wc", 180,0,180);
    protonAngles_withcharged->Sumw2();

    h_divided_protons = new TH1D("eff_proton", "eff_proton", 180,0,180);
    h_divided_protons->Sumw2();

    protonAngles2D = new TH2D("proton2D", "proton2D", 19, -5, 185, 160, 0, 800);
    protonAngles2D->Sumw2();

    protonAngles2D_withCharged = new TH2D("proton2D_wc", "proton2D_wc", 19, -5, 185, 160, 0, 800);
    protonAngles2D_withCharged->Sumw2();

    h_divided_protonAng2D = new TH2D("eff_proton2D", "eff_proton2D", 19, -5, 185, 160, 0, 800);
    h_divided_protonAng2D->Sumw2();

    proAngles_wc_diff = new TH2D("proton_Ang_diff", "proton_Ang_diff", 190, -5, 185, 160, 0, 800);
    proAngles_wc_diff->Sumw2();

    angleSeparation = new TH2D("proton_Ang_Sep", "proton_Ang_Sep", 190, -5, 185, 160, 0, 800);
    angleSeparation->Sumw2();

    tester = new TH1D("test", "test", 180, 0, 10);
}

PPi0Example::~PPi0Example()
{
}

Bool_t	PPi0Example::Init()
{
	cout << "Initialising physics analysis..." << endl;
	cout << "--------------------------------------------------" << endl << endl;

	if(!InitBackgroundCuts()) return kFALSE;
    if(!InitTargetMass()) return kFALSE;

    if(!PPhysics::Init()) return kFALSE;

    cout << "--------------------------------------------------" << endl;
	return kTRUE;
}

Bool_t	PPi0Example::Start()
{
    if(!IsGoATFile())
    {
        cout << "ERROR: Input File is not a GoAT file." << endl;
        return kFALSE;
    }
    SetAsPhysicsFile();

    TraverseValidEvents();

    return kTRUE;
}

void	PPi0Example::ProcessEvent()
{
    // Uncomment the following line to decode double hits in the tagger
    //GetTagger()->DecodeDoubles();

    // fill time diff (tagger - pi0), all pi0
    FillTime(*GetNeutralPions(),time);
    FillTimeCut(*GetNeutralPions(),time_cut);
	
	// fill missing mass, all pi0
    FillMissingMass(*GetNeutralPions(),MM);
	
	// fill invariant mass, all pi0
    FillMass(*GetNeutralPions(),IM);
		
    // Some neutral decays
    for (Int_t i = 0; i < GetNeutralPions()->GetNParticles(); i++)
    {
        // Fill MM for 2 photon decay
        if ((GetNeutralPions()->GetNSubParticles(i) == 2) & (GetNeutralPions()->GetNSubPhotons(i) == 2))
        {
		// fill time diff (tagger - pi0), this pi0
        FillTime(*GetNeutralPions(),i,time_2g);
        FillTimeCut(*GetNeutralPions(),i,time_2g_cut);
			
		// fill missing mass, this pi0
                FillMissingMass(*GetNeutralPions(),i,MM_2g);
            
		// fill invariant mass, this pi0
            FillMass(*GetNeutralPions(),i,IM_2g);
        }

    }

    //  --------- Kalli stuff -----------------------------------------

    // These two if-statements state the same thing, but the second one is more useful.

    // When we find a neutral pion (1) ... I think?:
    if (GetNeutralPions()->GetNParticles() == 1)
    {
        // Filling the histogram h_theta with Theta for the pion. 'GetTheta()' is a function that gives the angle in deg.
        h_theta->Fill(GetNeutralPions()->GetTheta(0));

        // With pion info, we can find the proton, zeros indicate 1st particle & tagger, respectively.
        //  Histogram, 'protonAngles', with the angle 'Theta' from the 4-vector, 'proton'.
        TLorentzVector proton = CalcMissingP4(*GetNeutralPions(), 0, 0);
        protonAngles->Fill(proton.Theta() * TMath::RadToDeg());

        // When a proton is detected, same process as above.
        if(GetRootinos()->GetNParticles() == 1)
        {
            h_theta_withcharged->Fill(GetNeutralPions()->GetTheta(0));
            protonAngles_withcharged->Fill(proton.Theta() * TMath::RadToDeg());
        }

    }

    // Note: this if-statement essentially does the same thing as the above, but creating a 2D histogram
    //  that plots the angle (theta), on the x-axis, and the incident photon energy on the y-axis.
    //  'proAngles_wc_diff is a histogram that finds the difference in theta between detected and
    //  expected protons (I think?).
    if(GetNeutralPions()->GetNParticles() == 1)
    {
        TLorentzVector proton_2D = CalcMissingP4(*GetNeutralPions(), 0, 0);
        protonAngles2D->Fill(proton_2D.Theta() * TMath::RadToDeg(), GetTagger()->GetTaggedEnergy(0));

        tester->Fill(GetRootinos()->GetNParticles());

        if(GetRootinos()->GetNParticles() == 1)
        {
            protonAngles2D_withCharged->Fill(proton_2D.Theta() * TMath::RadToDeg(), GetTagger()->GetTaggedEnergy(0));
           // protonAngles2D_withCharged->Draw();

            Double_t proton_detect = GetRootinos()->GetTheta(0);
            Double_t proton_expect = proton_2D.Theta() * TMath::RadToDeg();
            Double_t proton_diff = TMath::Abs(proton_detect - proton_expect);

            proAngles_wc_diff->Fill(proton_diff, GetTagger()->GetTaggedEnergy(0));

            TLorentzVector part = GetRootinos()->Particle(0);
            Double_t angle = TMath::RadToDeg() * part.Angle(proton_2D.Vect());


           // for(Int_t i = 0; i < GetRootinos()->GetNParticles(); i++)
            //{

                Double_t opening = TMath::RadToDeg() * proton_2D.Angle(GetRootinos()->GetUnitVector(0));

                //cout << opening << endl;

//opening = (TMath::RadToDeg()*missing.Angle(GetRootinos()->GetUnitVector(j)));

                angleSeparation->Fill(opening, GetTagger()->GetTaggedEnergy(0));
            //}
        }
    }

}

void	PPi0Example::ProcessScalerRead()
{
    PPhysics::ProcessScalerRead();
}

Bool_t	PPi0Example::Write()
{
    h_divided_pi0s->Add(h_theta_withcharged);
    h_divided_pi0s->Divide(h_theta);

    h_divided_protons->Add(protonAngles_withcharged);
    h_divided_protons->Divide(protonAngles);

    h_divided_protonAng2D->Add(protonAngles2D_withCharged);
    h_divided_protonAng2D->Divide(protonAngles2D);

 // ------- End of Kalli's stuff -------------------------------------------------------

    // Write all GH1's and TObjects defined in this class
    return GTreeManager::Write();
}
