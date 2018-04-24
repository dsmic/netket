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

#ifndef NETKET_SGD_HH
#define NETKET_SGD_HH

#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <cassert>
#include <cmath>

namespace netket{

using namespace std;
using namespace Eigen;

class Sgd: public AbstractStepper{

  //decay constant
  double eta_;

  int npar_;

  double l2reg_;

  double decay_factor_;


public:

  Sgd(double eta,double l2reg=0):
    eta_(eta),l2reg_(l2reg){
    npar_=-1;

    SetDecayFactor(1.0);
  }

  //Json constructor
  Sgd(const json & pars):
    eta_(FieldVal(pars["Learning"],"LearningRate")),
    l2reg_(FieldOrDefaultVal(pars["Learning"],"L2Reg",0.0)){

    npar_=-1;

    SetDecayFactor(FieldOrDefaultVal(pars["Learning"],"DecayFactor",1.0));
  }

  void Init(const VectorXd & pars){
    npar_=pars.size();
  }

  void Init(const VectorXcd & pars){

    npar_=2*pars.size();
  }

  void Update(const VectorXd & grad,VectorXd & pars){
    assert(npar_>0);

    eta_*=decay_factor_;

    for(int i=0;i<npar_;i++){
      pars(i)=pars(i) - (grad(i)+l2reg_*pars(i))*eta_;
    }
  }

  void Update(const VectorXcd & grad,VectorXd & pars){
    Update(VectorXd(grad.real()),pars);
  }

  void Update(const VectorXcd & grad,VectorXcd & pars){

    eta_*=decay_factor_;

    for(int i=0;i<pars.size();i++){
      pars(i)=pars(i) - (grad(i)+l2reg_*pars(i))*eta_;
    }
  }

  void SetDecayFactor(double decay_factor){
    assert(decay_factor<=1.00001);
    decay_factor_=decay_factor;
  }

  void Reset(){
  }
};


}

#endif
