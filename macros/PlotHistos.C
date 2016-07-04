/* 

	This macro plots overlays mutiple histograms as specified by the user. It takes 4 parameters as follows: 
	1) numHists: The number of histograms you wish to plot.
	2) graphTitle: The title of the graph.
	3) XaxisTitle: The title of the X-Axis.
	4) YaxisTitle: The title fo the Y-Axis.
 
*/


void PlotHistos(Int_t numHists, TString graphTitle, TString XaxisTitle, TString YaxisTitle)
{


	//Prompts the user to specify whether all files can be found in the same location with similar names.
	TString answer;
	std::cout << "Do all your files have the same name and directory except for file numbers? (Yes/No)" << endl;
	std::cin >> answer;

	//If the answer from the user is 'No', then the user will be continuously prompted for the names and plotting the
	// new histograms with the others, each in a different color.
	if(answer == "No")
	{
	
		
		TString fileName, histoName
		std::cout << "Please input the full file name and its directory." << endl;
		std::cin >> fileName;
		
		std::cout << "Please input the name of the histogram you want" << endl;
		std::cin >> histoName;
		
		std::cout << "The file name is: " << fileName << "." endl;

		TFile *file = TFile::Open(fileName);
		TH1 *h1 = (TH1*)file->Get(histoName);
		h1->SetLineColor(1);
		h1->Draw();
		h1->SetTitle(graphTitle);
		h1->GetXaxis()->SetTitle(XaxisTitle);
		h1->GetYaxis()->SetTitle(YaxisTitle);
	
		//This is the counter used to loop through the appropriate amount of times.
		Int_t count = 1;

		//Performs the same as above except that the Draw("SAME") command is used in place of the Draw() command.
		// The counter also acts as a way to designate a new color to new graphs.
		while(count<numHists)
		{
	
			TString fileName;
			std::cout << "Please input the full file name and the directory of the next file." << endl;
			std::cin >> fileName;

			std::cout << "The file name is: " << fileName << "." << endl;
	
			TFile *file = TFile::Open(fileName);
			TH1 *h1 = (TH1*)file->Get(histoName);
			h1->SetLineColor(count+1);
			h1->Draw("SAME");
			count++;
	
		} //while(count<numHists)
	} //if(answer == No)

	//If the user says 'Yes' (or anything other than 'No') then the user will be prompted for different parts
	// of the file name which are saved as strings and concatenated together. The user will be prompted repeatedly 
	// for the new file number because that is the only changing part. 
	else
	{


		TString prefix, suffix, fileNum, fileName, histoName;
		std::cout << "Please input the histogram's name." << endl;
		std::cin >> histoName;
	//	std::cout << "Please input the file directory and name without the number (i.e. /home/output/TaggEff_" << endl;
	//	std::cin >> prefix;
	//	std::cout << "Please input the type of file (i.e. '.root')" << endl;
	//	std::cin >> suffix;
		std::cout << "Please input file number" << endl;
		std::cin >> fileNum;
		
		prefix = "/home/hood/a2GoAT/output_goat/BkgSub_TaggEff_";
		suffix = ".root";

		fileName = prefix + fileNum + suffix;
		std::cout << "The file name is: " << fileName << endl;

		TFile *file = TFile::Open(fileName);
		TH1 *h1 = (TH1*)file->Get(histoName);
		h1->SetTitle(graphTitle);
		h1->GetXaxis()->SetTitle(XaxisTitle);
		h1->GetYaxis()->SetTitle(YaxisTitle);
		h1->SetLineColor(1);
		h1->Draw();
	

		Int_t count = 1;

		while(count<numHists)
		{
			std::cout << "Please input file number" << endl;
			std::cin >> fileNum;

			fileName = prefix + fileNum + suffix;
			std::cout << "The file name is: " << fileName << endl;
	
			TFile *file = TFile::Open(fileName);
			TH1 *h1 = (TH1*)file->Get(histoName);
			h1->SetLineColor(count+1);
			h1->Draw("SAME");
			count++;
	
		} //while(count<numHists)	
	} // else 
	

}
