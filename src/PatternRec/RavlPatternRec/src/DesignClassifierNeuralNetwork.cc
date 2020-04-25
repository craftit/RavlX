// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/DesignClassifierNeuralNetwork.cc"

#include "Ravl/PatternRec/DesignClassifierNeuralNetwork.hh"
#include "Ravl/PatternRec/ClassifierNeuralNetwork.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/fann/fann.h"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/DList.hh"
#include "Ravl/Text/TextFile.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN {

  //: Constructor.

  DesignClassifierNeuralNetworkBodyC::DesignClassifierNeuralNetworkBodyC(UIntT nLayers,
      UIntT nInputs,
      UIntT nHidden,
      UIntT nOutputs,
      RealT desiredError,
      UIntT maxEpochs,
      UIntT displayEpochs)
      : m_nLayers(nLayers),
        m_nInputs(nInputs),
        m_nHidden(nHidden),
        m_nOutputs(nOutputs),
        m_desiredError(desiredError),
        m_maxEpochs(maxEpochs),
        m_displayEpochs(displayEpochs),
        m_cascade(false) {
  }

  DesignClassifierNeuralNetworkBodyC::DesignClassifierNeuralNetworkBodyC(const XMLFactoryContextC & factory)
    : DesignClassifierSupervisedBodyC(factory),
      m_nLayers(factory.AttributeInt("numberOfLayers", 3)),
      m_nHidden(factory.AttributeInt("numberOfHiddenUnits", 7)),
      m_desiredError(factory.AttributeReal("desiredError", 0.0001)),
      m_maxEpochs(factory.AttributeInt("maxEpochs", 50000)),
      m_displayEpochs(factory.AttributeInt("displayEpochs", 100)),
      m_cascade(factory.AttributeBool("cascade", false))
  {
    // we will let the training data set decide these two...not sure why I didn't before
    m_nInputs = 0;
    m_nOutputs = 0;
  }

//: Load from stream.

  DesignClassifierNeuralNetworkBodyC::DesignClassifierNeuralNetworkBodyC(std::istream &strm)
      : DesignClassifierSupervisedBodyC(strm),
        m_nLayers(0),
        m_nInputs(0),
        m_nHidden(0),
        m_nOutputs(0),
        m_cascade(false)
  {
    int version;
    strm >> version;
    if (version != 0 && version != 1)
      throw ExceptionUnexpectedVersionInStreamC("DesignClassifierNeuralNetworkBodyC::DesignClassifierNeuralNetworkBodyC(std::istream &), Unrecognised version number in stream. ");
    strm >> m_nLayers;
    strm >> m_nInputs;
    strm >> m_nHidden;
    strm >> m_nOutputs;
    if (version == 1) {
      strm >> m_desiredError;
      strm >> m_maxEpochs;
      strm >> m_displayEpochs;
      strm >> m_cascade;
    } else {
      m_desiredError = 0.0001;
      m_maxEpochs = 50000;
      m_displayEpochs = 100;
    }
  }

//: Load from binary stream.

  DesignClassifierNeuralNetworkBodyC::DesignClassifierNeuralNetworkBodyC(BinIStreamC &strm)
      : DesignClassifierSupervisedBodyC(strm),
        m_nLayers(0),
        m_nInputs(0),
        m_nHidden(0),
        m_nOutputs(0),
        m_cascade(false)
  {
    int version;
    strm >> version;
    if (version != 0 && version != 1)
      throw ExceptionUnexpectedVersionInStreamC("DesignClassifierNeuralNetworkBodyC::DesignClassifierNeuralNetworkBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> m_nLayers;
    strm >> m_nInputs;
    strm >> m_nHidden;
    strm >> m_nOutputs;
    if (version == 1) {
      strm >> m_desiredError;
      strm >> m_maxEpochs;
      strm >> m_displayEpochs;
      strm >> m_cascade;
    } else {
      m_desiredError = 0.0001;
      m_maxEpochs = 50000;
      m_displayEpochs = 100;
    }
  }

//: Writes object to stream, can be loaded using constructor

  bool DesignClassifierNeuralNetworkBodyC::Save(std::ostream &out) const {
    if (!DesignClassifierSupervisedBodyC::Save(out))
      return false;
    int version = 1;
    out << version << std::endl;
    out << m_nLayers << std::endl;
    out << m_nInputs << std::endl;
    out << m_nHidden << std::endl;
    out << m_nOutputs << std::endl;
    out << m_desiredError << std::endl;
    out << m_maxEpochs << std::endl;
    out << m_displayEpochs << std::endl;
    out << m_cascade << std::endl;
    return true;
  }

//: Writes object to stream, can be loaded using constructor

  bool DesignClassifierNeuralNetworkBodyC::Save(BinOStreamC &out) const {
    if (!DesignClassifierSupervisedBodyC::Save(out))
      return false;
    int version = 1;
    out << version << m_nLayers << m_nInputs << m_nHidden << m_nOutputs;
    out << m_desiredError;
    out << m_maxEpochs;
    out << m_displayEpochs;
    out << m_cascade;
    return true;
  }

//: Create a clasifier.

  ClassifierC DesignClassifierNeuralNetworkBodyC::Apply(const SampleC<VectorC> &in, const SampleC<UIntT> &out) {
    RavlAssertMsg(in.Size() == out.Size(),
        "DesignClassifierNeuralNetworkBodyC::Apply(), Sample of vector and labels should be the same size.");

    m_nInputs = in.First().Size();

    // need to get number of outputs, i do dispair with our datasets
    UIntT maxLabel = 0;
    for (SampleIterC<UIntT> it(out); it; it++) {
      if (*it > maxLabel)
        maxLabel = *it;
    }
    m_nOutputs = maxLabel + 1;

    //: Need to convert our data set into a dataset for a neural net
    //: The easiest way for now is to save the data to file in the correct format for the library
    //: and then use the lib function to read it back in.  It sucks a little but it is only for training
    DataSet2C<SampleC<VectorC>, SampleC<UIntT> > dset(in, out);
    FilenameC tmpData = "/tmp/nndata";
    tmpData = tmpData.MkTemp();

    //: format of data
    //: data_set_size  number_of_inputs number_of_outputs
    //: input_data separated by space
    //: output_data separated by space
    //: ....
    //: ....

    {
      OStreamC os(tmpData);
      os << in.Size() << " " << m_nInputs << " " << m_nOutputs << std::endl;
      for (DataSet2IterC<SampleC<VectorC>, SampleC<UIntT> > it(dset); it; it++) {
        for (SArray1dIterC<RealT> vit(it.Data1()); vit; vit++) {
          os << *vit << " ";
        }
        os << " " << std::endl;

        VectorC nnOutput(m_nOutputs);
        nnOutput.Fill(-0.9);
        nnOutput[it.Data2()] = 0.9;
        for (SArray1dIterC<RealT> vit(nnOutput); vit; vit++) {
          os << *vit << " ";
        }

        os << " " << std::endl;
      }
    }

    return Train(tmpData);
  }


  FunctionC DesignClassifierNeuralNetworkBodyC::Apply(const SampleC<VectorC>&in, const SampleC<VectorC>&out) {
    m_nInputs = in.First().Size();
    m_nOutputs = out.First().Size();

    //: Need to convert our data set into a dataset for a neural net
    //: The easiest way for now is to save the data to file in the correct format for the library
    //: and then use the lib function to read it back in.  It sucks a little but it is only for training
    DataSet2C<SampleC<VectorC>, SampleC<VectorC> > dset(in, out);
    FilenameC tmpData = "/tmp/nndata";
    tmpData = tmpData.MkTemp();


    //: format of data
    //: data_set_size  number_of_inputs number_of_outputs
    //: input_data seperated by space
    //: output_data seperated by space
    //: ....
    //: ....

    {
      OStreamC os(tmpData);
      os << in.Size() << " " << m_nInputs << " " << m_nOutputs << std::endl;
      for (DataSet2IterC<SampleC<VectorC>, SampleC<VectorC> > it(dset); it; it++) {
        for (SArray1dIterC<RealT> vit(it.Data1()); vit; vit++) {
          os << *vit << " ";
        }
        os << " " << std::endl;

        for (SArray1dIterC<RealT> vit(it.Data2()); vit; vit++) {
          os << *vit << " ";
        }
        os << " " << std::endl;
      }
    }

    return Train(tmpData);

  }

  ClassifierC DesignClassifierNeuralNetworkBodyC::Train(const FilenameC & datafile) {



    FilenameC tmpNN = "/tmp/nn";
    tmpNN = tmpNN.MkTemp();

    //: Read the data back from the file
    struct fann_train_data *data = fann_read_train_from_file(datafile.chars());

    // Make empty network
    struct fann *ann = NULL;

    if (!m_cascade) {

      //: Next we can build the neural network
      const float desired_error = (const float) m_desiredError;
      const unsigned int max_epochs = m_maxEpochs;
      const unsigned int epochs_between_reports = m_displayEpochs;

      ann = fann_create_standard(m_nLayers, m_nInputs, m_nHidden, m_nOutputs);

      //: OK some network settings.   We should really make these available through the interface but not yet/
      //fann_set_activation_steepness_hidden(ann, 1);
      //fann_set_activation_steepness_output(ann, 1);
      fann_set_activation_function_hidden(ann, FANN_ELLIOT);
      fann_set_activation_function_output(ann, FANN_LINEAR);
      fann_set_training_algorithm(ann, FANN_TRAIN_RPROP);

      // Need to set some parameters when to stop.  Here some docs copied from FANN web-site
      // FANN_STOPFUNC_MSE Stop criteria is Mean Square Error (MSE) value.
      // FANN_STOPFUNC_BIT Stop criteria is number of bits that fail.
      // The number of bits; means the number of output neurons which differ more than
      // the bit fail limit (see fann_get_bit_fail_limit, fann_set_bit_fail_limit).
      // The bits are counted in all of the training data, so this number can be higher than
      // the number of training data.
      fann_set_train_stop_function(ann, FANN_STOPFUNC_BIT);
      fann_set_bit_fail_limit(ann, 0.1f);

      //: important to init on training data
      fann_init_weights(ann, data);

      //: do the training
      fann_train_on_data(ann, data, max_epochs, epochs_between_reports, desired_error);

    } else {

      // performing cascade training
      UIntT maxNeurons = 1000;
      ann = fann_create_shortcut(2, fann_num_input_train_data(data), fann_num_output_train_data(data));
      fann_cascadetrain_on_data(ann, data, maxNeurons, m_displayEpochs, m_desiredError);

    }

    //: work out error on training data
    printf("Testing network. %f\n", fann_test_data(ann, data));

    //: and save network
    fann_save(ann, tmpNN.chars());

    //: destroy
    fann_destroy(ann);

    //: What we want to do is to load this into a string
    DListC<StringC> nn;
    {
      TextFileC file(tmpNN);
      for (UIntT i = 1; i <= file.NoLines(); i++) {
        StringC line = file[i];
        line.gsub(" ", "#");
        line = line.TopAndTail();
        nn.InsLast(line);
      }
    }
    //: return classifier
    datafile.Remove();
    tmpNN.Remove();
    return ClassifierNeuralNetworkC(m_nLayers, m_nInputs, m_nHidden, m_nOutputs, nn);
  }

//: Create a classifier with weights for the samples.

  ClassifierC DesignClassifierNeuralNetworkBodyC::Apply(const SampleC<VectorC> &in,
      const SampleC<UIntT> &out,
      const SampleC<RealT> &weight) {
    RavlAssertMsg(0,
        "DesignClassifierNeuralNetworkBodyC::Apply(in,out,weight), Not implemented. Send a feature request! ");
    return ClassifierC();
  }

//////////////////////////////////////////////////////////

  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DesignClassifierNeuralNetworkBodyC,
      DesignClassifierNeuralNetworkC,
      DesignClassifierSupervisedC);

  RavlN::XMLFactoryRegisterHandleConvertC<DesignClassifierNeuralNetworkC, DesignClassifierSupervisedC> g_registerXMLFactoryDesignClassifierNeuralNetwork("RavlN::DesignClassifierNeuralNetworkC");

  void linkDesignClassifierNeuralNetwork() {
  }

}
