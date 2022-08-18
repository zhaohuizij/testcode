// do this third !
// fix plane and change 23 paddles first ;then change plane.
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH1I.h>
#include <TCanvas.h>

using namespace std;
void read_records_rdset67(int paddle)
{
    std::string hodoNames = "H1B";// change here: planeID

    TH1I* hist_all;
    TH1I* hist_acc;
    TH1D* hist_effName;
    TGraphAsymmErrors* hist_eff;
    double Nevents_all= 0 ;
    double Nevents_acc= 0;
    char buffer[20];
        
    sprintf(buffer, "%s_No.%dall", hodoNames.c_str(),paddle);
    hist_all = new TH1I(buffer, buffer, 1, 485000, 685000);
//    hist_all->Sumw2();

    sprintf(buffer, "%s_No.%dacc", hodoNames.c_str(),paddle);
    hist_acc = new TH1I(buffer, buffer, 1,485000,685000);
  //  hist_acc->Sumw2();

    hist_eff = new TGraphAsymmErrors();
    sprintf(buffer, "%s_No.%d", hodoNames.c_str(),paddle);
    hist_effName = new TH1D(buffer, buffer, 1,485000,685000);
   // hist_effName->Sumw2();
    double spillID[1]={0};
    for (int i = 0 ; i < 1; i++)
    {
	    spillID[i] = 585000+20000*i;
	
    }

    int a = 0;
    string line;
    double Nhits_all[1];
    ifstream myFile;
    ostringstream filename;
    filename <<"Nhits_all_read_H1B_No"<<paddle<<".txt";
    myFile.open(filename.str().c_str());
     
    if(myFile.is_open())
    {
	while ( getline (myFile,line) )
	{
	    Nhits_all[a] = std::stod(line);
	    cout<<a<<":  "<<Nhits_all[a]<<endl;
	    a++;
	}
    }
    cout<<"a="<<a<<endl;
  //a should ==b==188
    int b = 0;
    string line1;
    double Nhits_acc[1];
    ifstream myFile1;
    ostringstream filename1;
    filename1 <<"Nhits_acc_read_H1B_No"<<paddle<<".txt";
    myFile1.open(filename1.str().c_str());

    if(myFile1.is_open())
    {
        while ( getline (myFile1,line1) )
        {
            Nhits_acc[b] = std::stod(line1);
            cout<<b<<":  "<<Nhits_acc[b]<<endl;
            b++;
        }
    }
    cout<<"b="<<b<<endl;  
    if(a!=b ) cout<<"something is wrong !"<<endl;
    else cout<<"Normal"<<endl;
    
    ostringstream oss;
    TCanvas* c1 = new TCanvas("c1", "");
    c1->SetGrid();
    for(int j=0; j < a;  j++) 
	{
	    hist_all->SetBinContent(j+1, Nhits_all[j]);
//	    cout<<j<<" spillID:"<<spillID[j]<<"  Bincontents: "<<Nhits_all[j]<<endl;
	    Nevents_all = Nevents_all+ Nhits_all[j];
	    hist_all->SetBinError(j+1,TMath::Sqrt(Nhits_all[j]));
            hist_acc->SetBinContent(j+1, Nhits_acc[j]);
	    Nevents_acc = Nevents_acc+ Nhits_acc[j];
            hist_acc->SetBinError(j+1,TMath::Sqrt(Nhits_acc[j]));
	}
	cout<<" Nevents_all: "<<Nevents_all<<" Nevents_acc: "<<Nevents_acc<<endl;

	gStyle->SetOptStat(kFALSE);
        hist_eff ->Divide(hist_acc,hist_all,"cl=0.683 b(1,1) mode");
        oss.str("");
        oss <<hist_effName->GetName()<<".png";
        hist_eff->SetMarkerColor(kRed);
        hist_eff->SetLineColor  (kRed);
        hist_eff->SetMarkerStyle(20);
        hist_eff->SetTitle(";spillID;Paddle Efficiency");
        hist_eff->Draw("APE");
        c1->Update();
        c1->Pad()->Update();
        TAxis *ay = hist_eff->GetYaxis();
        ay->SetRangeUser(0.1,1.01);
        c1->SaveAs(oss.str().c_str());
	double eff_mean[1];
        for (int j=0;j<a ;j++)
        {
            eff_mean[j] = Nhits_acc[j]/Nhits_all[j]  ;
            cout<<"mean of eff:"<<j<<" "<<eff_mean[j]<<endl;
        }
	double binerr=0;
	for(int i=0;i<1;i++ )
	{
	    binerr = hist_eff ->GetErrorY(i);
	    cout<<"binerr:"<<i<<"  "<<binerr<<endl;
	}

	oss.str("");
        gStyle->SetOptStat(111111);
        oss << hist_all->GetName()<<".png";
        hist_all->Draw();
        hist_all->SetFillColor(kBlue-7);
        c1->SaveAs(oss.str().c_str());	

	oss.str("");
        gStyle->SetOptStat(111111);
        oss << hist_acc->GetName()<<".png";
        hist_acc->Draw();
        hist_acc->SetFillColor(kBlue-7);
        c1->SaveAs(oss.str().c_str());
	cout<< "31  "<<paddle<<"  "<<eff_mean[0]<<"  "<<binerr<<endl;
}

