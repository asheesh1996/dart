/*
  RTQL8, Copyright (c) 2011 Georgia Tech Graphics Lab
  All rights reserved.

  Author	Sehoon Ha
  Date		06/12/2011
*/


#include "FileInfoDof.h"

#include <string>
#include <fstream>
#include "Skeleton.h"
#include "Dof.h"

using namespace std;

namespace model3d{

    FileInfoDof::FileInfoDof(Skeleton* _model, double _fps)
        : mModel(_model),mFPS(_fps),mNumFrames(0){
    }

    FileInfoDof::~FileInfoDof(){
        mDofs.clear();
        mNumFrames = 0;
    }

    bool FileInfoDof::loadFile(const char* _fName)
    {
        ifstream inFile(_fName);
        if(inFile.fail()==1) return false;

        inFile.precision(20);
        char buffer[256];
        int nDof;

        //nFrames =
        inFile >> buffer;
        inFile >> buffer;
        inFile >> mNumFrames;

        //nDof =
        inFile >> buffer;
        inFile >> buffer;
        inFile >> nDof;

        if( mModel==NULL || mModel->getNumDofs()!=nDof )
            return false;

        mDofs.resize(mNumFrames);

        //dof names
        for(int i=0; i<nDof; i++)
            inFile >> buffer;
        for(int j=0; j<mNumFrames; j++){
            for(int i=0; i<nDof; i++){
                double val;
                inFile >> val;
                mDofs[j].push_back(val);
            }
        }

        // fps
        inFile>>buffer;
        if(!inFile.eof()){
            inFile>>mFPS;
        }

        inFile.close();

        string text = _fName;
        int lastSlash = text.find_last_of("/");
        text = text.substr(lastSlash+1);
        strcpy(mFileName, text.c_str());
        return true;
    }

    bool FileInfoDof::saveFile( const char* _fName, int _start, int _end, double _sampleRate ){
        if(_end<_start) return false;

        ofstream outFile(_fName, ios::out);
        if( outFile.fail() ) return false;

        int first = _start<mNumFrames?_start:mNumFrames-1;
        int last = _end<mNumFrames?_end:mNumFrames-1;

        outFile.precision(20);
        outFile<<"frames = " <<last-first+1<<" dofs = "<<mModel->getNumDofs()<<endl;

        for(int i=0; i<mModel->getNumDofs(); i++)
            outFile << mModel->getDof(i)->getName() << ' ';
        outFile << endl;

        for(int i=first; i<= last; i++){
            for(int j=0; j<mModel->getNumDofs(); j++){
                outFile << mDofs[i][j] <<' ';
            }
            outFile << endl;
        }

        outFile<<"FPS "<<mFPS<<endl;

        outFile.close();

        string text = _fName;
        int lastSlash = text.find_last_of("/");
        text = text.substr(lastSlash+1);
        strcpy(mFileName, text.c_str());
        return true;
    }

}

