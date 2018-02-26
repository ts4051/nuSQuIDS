/*
Implementing a neutrino decoherence model in SQuIDS/nuSQuIDS
Tom Stuttard, Mikkel Jensen (Niels Bohr Institute)
*/

//TODO Create a .cpp file


#ifndef NUSQUIDSDECOH_H
#define NUSQUIDSDECOH_H

#include <nuSQuIDS/nuSQuIDS.h>

namespace nusquids{


/*
  nuSQuIDS extedned to include decohernece
*/

class nuSQUIDSDecoh: public nuSQUIDS {


  public:

    //Default void constructor (required by boost python).
    nuSQUIDSDecoh(){}

    // Constructor : Single energy mode
    nuSQUIDSDecoh(unsigned int numneu, NeutrinoType NT = neutrino)
      : nuSQUIDS(numneu,NT)
      , decoherence_matrix(numneu)
//      , hiBuffer(new double[nsun*nsun])  //TODO REMOVE, RENAME
    { init(); }

    // Constructor : Multi-energy mode
    nuSQUIDSDecoh(marray<double,1> E_vector, unsigned int numneu, NeutrinoType NT = both,
       bool iinteraction = false, std::shared_ptr<NeutrinoCrossSections> ncs = nullptr)
      : nuSQUIDS(E_vector,numneu,NT,iinteraction,ncs)
      , decoherence_matrix(numneu)
//      , hiBuffer(new double[nsun*nsun])  //TODO REMOVE, RENAME
    { init(); }

    // TODO Copy and move constructors...

  
    // Set the decoherence matrix elements (set each individually)
    void Set_DecoherenceMatrix(const marray<double,2>& dmat);

    // Set the decoherence matrix elements using the Gamma21,31,32 model
    // See https://arxiv.org/pdf/1708.05495.pdf equation 12
    void Set_DecoherenceMatrix(double Gamma21,double Gamma31,double Gamma32);

    // Get the current value of the decoherence matrix
    marray<double,2> Get_DecoherenceMatrix() const;


  protected:

    // Common initialisation tasks
    void init();

  private:

    // The deocherenc ematrix to use
    squids::SU_vector decoherence_matrix; //TODO Is this the correct type to store this as?

    // Function to return the Gamma decoherence matrix (basically D[rho] without the rho part)
    // TODO Can I access the rho part here?
    squids::SU_vector DecohGamma(unsigned int ei,unsigned int index_rho, double t) const override;

//    std::unique_ptr<double[]> hiBuffer; //TODO REMOVE, RENAME

};


/*
  nuSQUIDSAtm extended to include decoherence
*/

class nuSQUIDSAtmDecoh : public nuSQUIDSAtm<nuSQUIDSDecoh> {

  public:

    // Use the base class constructors
    using nuSQUIDSAtm<nuSQUIDSDecoh>::nuSQUIDSAtm;

    // TODO Copy and move constructors...

    // Wrap debug setter
    void Set_Debug(bool debug) {
      for(nuSQUIDSDecoh& nsq : nusq_array) nsq.Set_Debug(debug);
    }

    // Wrap decoherence matrix getters/setters
    void Set_DecoherenceMatrix(const marray<double,2>& dmat) {
      for(nuSQUIDSDecoh& nsq : nusq_array) nsq.Set_DecoherenceMatrix(dmat);
    }

    void Set_DecoherenceMatrix(double Gamma21,double Gamma31,double Gamma32) {
      for(nuSQUIDSDecoh& nsq : nusq_array) nsq.Set_DecoherenceMatrix(Gamma21,Gamma31,Gamma32);
    }

    marray<double,2> Get_DecoherenceMatrix() const {
      return nusq_array[0].Get_DecoherenceMatrix();
    }


};



} // close namespace

#endif //NUSQUIDSDECOH_H

