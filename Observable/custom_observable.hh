// Copyright 2018 The Simons Foundation, Inc. - All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef NETKET_CUSTOM_OBSERVABLE_HH
#define NETKET_CUSTOM_OBSERVABLE_HH

#include <vector>
#include <string>

namespace netket{

class CustomObservable:public AbstractObservable{

  std::vector<LocalOperator> operators_;
  const Hilbert & hilbert_;
  std::string name_;

public:

  using MatType=LocalOperator::MatType;

  CustomObservable(const Hilbert & hilbert,const std::vector<MatType> & jop,
                   const std::vector<vector<int>> & sites,std::string name):
    hilbert_(hilbert),name_(name){


    if(sites.size()!=jop.size()){
      std::cerr<<"The custom Observable definition is inconsistent:"<<endl;
      std::cerr<<"Check that ActingOn is defined"<<endl;
      std::abort();
    }

    for(int i=0;i<jop.size();i++){
      operators_.push_back(LocalOperator(hilbert_,jop[i],sites[i]));
    }

  }

  void FindConn(const VectorXd & v,
    vector<std::complex<double>> & mel,
    vector<vector<int>> & connectors,
    vector<vector<double>> & newconfs)
  {
    connectors.clear();
    newconfs.clear();
    mel.resize(0);

    for(int i=0;i<operators_.size();i++){
      operators_[i].AddConn(v,mel,connectors,newconfs);
    }
  }

  const Hilbert & GetHilbert()const{
    return hilbert_;
  }

  const std::string Name()const{
    return name_;
  }
};
}
#endif
