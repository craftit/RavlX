/******************************************************************************
 * $Id$
 * This file is used in conjunction with RAVL, Recognition And Vision Library
 * Copyright (C) 2006, OmniPerception Ltd.
 * This code may be redistributed under the terms of the GNU Lesser
 * General Public License (LGPL). See the gpl.licence file for details or
 * see http://www.gnu.org/copyleft/lesser.html
 ******************************************************************************/
#ifndef RAVL_SCORES_TABLE_HH
#define RAVL_SCORES_TABLE_HH

#include "Ravl/TwoWayMap.hh"
#include "Ravl/String.hh"
#include "Ravl/RealHistogram1d.hh"

namespace RavlN {
  namespace FaceN {
    /**
     @author Alexey Kostin
     */

    using namespace RavlN;

//------------------------------------------------------------------------------
    class ScoreRecordC
    {
    public:
      ScoreRecordC() :
          id1(0), id2(0), score(0.f)
      {
      }

      ScoreRecordC(int Id1, int Id2, float &Score) :
          id1(Id1), id2(Id2), score(Score)
      {
      }

      const unsigned int& Id1() const
      {
        return id1;
      }

      const unsigned int& Id2() const
      {
        return id2;
      }

      float& Score()
      {
        return score;
      }

      const float& Score() const
      {
        return score;
      }

    private:
      unsigned int id1, id2;
      float score;
    };

//------------------------------------------------------------------------------
//! Class for dealing with scores table
    class ScoresTableC
    {
    public:
      enum SortTypeE
      {
        Unsorted, SortByScores, SortByIds
      };

      ScoresTableC();

      ~ScoresTableC()
      {
      }

      /*! Add score
       * @param TableId table or expert id
       * @param Id1  id of image 1 or Id of template
       * @param Id2  id of image 2
       * @param Score   score
       */
      bool Add(const StringC &TableId, const StringC &Id1, const StringC &Id2, float Score);

      /*! Add score
       * @param TableId table or expert id
       * @param Id1  id of image 1 or Id of template
       * @param Id2  id of image 2
       * @param Score   score
       */
      bool Add(UIntT TableId, UIntT Id1, UIntT Id2, float Score);

      /*! Set actual Id for ImgId
       * @param ActualId actual id
       * @param ImgId    image id
       */
      void SetActualId(const StringC &ActualId, const StringC &ImgId);

      void Sort(unsigned int TableIdCode, SortTypeE SortType);

      //! returns eer
      float GetEER(const StringC &TableId);

      //! returns eer
      float GetFRR(const StringC &TableId, float FAR);

      //! get histograms of scores
      void GetHistograms(const StringC &TableId, RealHistogram1dC &ClientHist, RealHistogram1dC &ImpostorHist) const;

      //! generate ROC curve
      //void PlotRoc(GnuPlotC& Plot, const StringC &TableId, RealT maxFa, RealT maxFr);

      void Print(const StringC &TableId);

      int GetMaxSize();

      void ReserveMemory(const StringC &TableId, unsigned int Size);

      //! Access scores table
      std::vector<std::vector<ScoreRecordC> >& Data()
      {
        return scores;
      }

      UIntT AddTable(const StringC &TableId);

      void NormTable(const StringC &TableId, double Mean, double Variance);

      int NumTables() const
      {
        return tableIds.Size();
      }

      const StringC& GetTableId(int Id)
      {
        return tableIds.Backward(Id);
      }

      UIntT GetImgCode(const StringC& ImgId);

      void Save(const StringC& Filename) const;
      void Save(BinOStreamC &Stream) const;

      void Load(const StringC& Filename);
      void Load(BinIStreamC &Stream);

    private:
      void PrepareStatArrays(unsigned int TableIdCode);

      RavlN::TwoWayMapC<StringC> actualIds; //!< Map for coding actualIds
      RavlN::TwoWayMapC<StringC> imgIds; //!< map for coding imageIds
      RavlN::TwoWayMapC<StringC> tableIds; //!< String names of tables
      std::vector<unsigned int> imgCodeToActualCodeMap; //!< Map of imageId code to actualId code

      // auxilary arrays for fast calculation of statictics
      std::vector<bool> statValid; //!< statistic for a table is valid
      std::vector<unsigned int> numCli; //!< number of lients in a table
      std::vector<unsigned int> numImp; //!< number of impostors in a table
      std::vector<std::vector<unsigned int> > cli; //!< cli curve
      std::vector<std::vector<unsigned int> > imp; //!< imp curve

      std::vector<SortTypeE> sortedType; //!< how table is sorted
      std::vector<std::vector<ScoreRecordC> > scores; //!< container for scores
    };
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

  }
}

#endif
