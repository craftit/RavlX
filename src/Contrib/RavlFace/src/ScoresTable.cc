/*****************************************************************************
 * $Id$
 * This file is used in conjunction with RAVL, Recognition And Vision Library
 * Copyright (C) 2006, OmniPerception Ltd.
 * This code may be redistributed under the terms of the GNU Lesser
 * General Public License (LGPL). See the gpl.licence file for details or
 * see http://www.gnu.org/copyleft/lesser.html
 *****************************************************************************/

#include "Ravl/Face/ScoresTable.hh"
#include <algorithm>
#include <math.h>
#include "Ravl/BinStream.hh"
#include <stdio.h>

namespace RavlN {
  namespace FaceN {

//------------------------------------------------------------------------------
    ScoresTableC::ScoresTableC()
    {
    }
//------------------------------------------------------------------------------
    void ScoresTableC::SetActualId(const StringC &ActualId, const StringC &ImgId)
    {
      unsigned int actualIdCode, imgIdCode;
      actualIds.MapForward(ActualId, actualIdCode, true);
      imgIds.MapForward(ImgId, imgIdCode, true);
      if (imgCodeToActualCodeMap.size() <= imgIdCode)
        imgCodeToActualCodeMap.push_back(actualIdCode);
      else {
        if (imgCodeToActualCodeMap[imgIdCode] != actualIdCode) {
          printf("Overriding old id %s with new %s\n", actualIds.Backward(imgCodeToActualCodeMap[imgIdCode]).chars(), ActualId.chars());
          imgCodeToActualCodeMap[imgIdCode] = actualIdCode;
        }
      }
    }
//------------------------------------------------------------------------------
    bool ScoresTableC::Add(const StringC &TableId, const StringC &Id1, const StringC &Id2, float Score)
    {
      UIntT id1Code, id2Code;
      imgIds.MapForward(Id1, id1Code, true);
      imgIds.MapForward(Id2, id2Code, true);

      UIntT tableId;
      if (!tableIds.MapForward(TableId, tableId, false)) {
        tableId = AddTable(TableId);
      }

      return Add(tableId, id1Code, id2Code, Score);
    }

//------------------------------------------------------------------------------
    /*! Add score
     * @param TableId table or expert id
     * @param Id1  id of image 1 or Id of template
     * @param Id2  id of image 2
     * @param Score   score
     */
    bool ScoresTableC::Add(UIntT TableId, UIntT Id1, UIntT Id2, float Score)
    {
      if (Id1 >= imgCodeToActualCodeMap.size() || Id2 >= imgCodeToActualCodeMap.size()) //no actual Id for the image Id
        return false;

      //printf("Add:  %i %s  %i %s\n", imgId1, ImgId1.c_str(), imgId2, ImgId2.c_str());

      scores[TableId].push_back(ScoreRecordC(Id1, Id2, Score));
      sortedType[TableId] = Unsorted;
      statValid[TableId] = false;
      return true;
    }

//------------------------------------------------------------------------------
    bool CompareScores1(const ScoreRecordC& V1, const ScoreRecordC& V2)
    {
      return V1.Score() < V2.Score();
    }
    bool CompareScores2(const ScoreRecordC& V1, const ScoreRecordC& V2)
    {
      return (V1.Id1() < V2.Id1()) || ((V1.Id1() == V2.Id1()) && (V1.Id2() < V2.Id2()));
    }
    void ScoresTableC::Sort(unsigned int TableIdCode, SortTypeE SortType)
    {
      if (sortedType[TableIdCode] == SortType)
        return;

      std::vector<ScoreRecordC> &curTable = scores[TableIdCode];

      cout << "Sorting scores table...\n";

      switch (SortType)
      {
        case Unsorted:
          break;
        case SortByScores:
          std::sort(curTable.begin(), curTable.end(), CompareScores1);
          break;
        case SortByIds:
          std::sort(curTable.begin(), curTable.end(), CompareScores2);
          break;
      }

      cout << "done.\n";
      sortedType[TableIdCode] = SortType;
    }
//------------------------------------------------------------------------------
    float ScoresTableC::GetEER(const StringC &TableId)
    {
      unsigned int tableIdCode;
      if (!tableIds.MapForward(TableId, tableIdCode, false))
        return -1;

      PrepareStatArrays(tableIdCode);

      std::vector<unsigned int> &cli_local = cli[tableIdCode];
      std::vector<unsigned int> &imp_local = imp[tableIdCode];

      float numCli_local = (float) numCli[tableIdCode];
      float numImp_local = (float) numImp[tableIdCode];
      cout << "numCli:" << numCli_local << endl;
      cout << "numImp:" << numImp_local << endl;

      unsigned int a = 0;
      unsigned int c = cli_local.size() - 1;
      float frr = 0., far = 0.;
      while ((c - a) > 1) {
        unsigned int b = (a + c) / 2;
        //printf("a:%i  b:%i  c:%i\n", a, b, c);
        frr = float(cli_local[b]) / numCli_local;
        far = (numImp_local - float(imp_local[b])) / numImp_local;
        if (far > frr)
          a = b;
        else
          c = b;
      }
      //printf("a:%i  c:%i  %g  %g\n", a, c, far_local[a], frr_local[a]);
      printf("threshold:%g\n", scores[tableIdCode][a].Score());

      return (far + frr) / 2.f;
    }

//------------------------------------------------------------------------------
//! returns eer
    float ScoresTableC::GetFRR(const StringC &TableId, float FAR)
    {
      unsigned int tableIdCode;
      if (!tableIds.MapForward(TableId, tableIdCode, false))
        return -1;

      PrepareStatArrays(tableIdCode);

      std::vector<unsigned int> &cli_local = cli[tableIdCode];
      std::vector<unsigned int> &imp_local = imp[tableIdCode];

      float numCli_local = (float) numCli[tableIdCode];
      float numImp_local = (float) numImp[tableIdCode];

      unsigned int a = 0;
      unsigned int c = cli_local.size() - 1;
      float frr = 0., far = 0.;
      while ((c - a) > 1) {
        unsigned int b = (a + c) / 2;
        //printf("a:%i  b:%i  c:%i\n", a, b, c);
        frr = float(cli_local[b]) / numCli_local;
        far = (numImp_local - float(imp_local[b])) / numImp_local;
        if (far > FAR)
          a = b;
        else
          c = b;
      }
      //printf("a:%i  c:%i  %g  %g\n", a, c, far_local[a], frr_local[a]);
      //printf("threshold:%g\n", scores_local[a].score);

      return frr;
    }

//------------------------------------------------------------------------------
//! get histograms of scores
    void ScoresTableC::GetHistograms(const StringC &TableId, RealHistogram1dC &ClientHist, RealHistogram1dC &ImpostorHist) const
    {
      unsigned int tableIdCode;
      if (!tableIds.MapForward(TableId, tableIdCode, false))
        throw ExceptionC("Table id not found");

      const std::vector<ScoreRecordC> &scores_local = scores[tableIdCode];

      // Find the min and maximum of whole scores table
      float min = scores_local[0].Score();
      float max = min;
      for (std::vector<ScoreRecordC>::const_iterator scoresIt = scores_local.begin(); scoresIt != scores_local.end(); scoresIt++) {
        const float score = scoresIt->Score();
        if (score < min)
          min = score;
        else if (score > max)
          max = score;
      }

      // write the histograms
      ClientHist = RealHistogram1dC(min, max, 100);
      ImpostorHist = RealHistogram1dC(min, max, 100);
      for (std::vector<ScoreRecordC>::const_iterator scoresIt = scores_local.begin(); scoresIt != scores_local.end(); scoresIt++) {
        const ScoreRecordC& val = *scoresIt;
        if (imgCodeToActualCodeMap[val.Id1()] == imgCodeToActualCodeMap[val.Id2()]) {
          ClientHist.Vote(val.Score());
        } else {
          ImpostorHist.Vote(val.Score());
        }
      }
    }

//------------------------------------------------------------------------------
//! generate ROC curve
    /*void ScoresTableC::PlotRoc(GnuPlotC& Plot, const StringC &TableId,
     RealT maxFa, RealT maxFr)
     {
     unsigned int tableIdCode;
     if(!tableIds.MapForward(TableId, tableIdCode, false))
     throw ExceptionC("Table id not found");

     const std::vector<ScoreRecordC> &scores_local = scores[tableIdCode];

     Sort(tableIdCode, SortByScores);

     UIntT totalCli = 0;
     UIntT totalImp = 0;

     for(std::vector<ScoreRecordC>::const_iterator scoresIt = scores_local.begin();
     scoresIt != scores_local.end(); scoresIt++)
     {
     const ScoreRecordC& val = *scoresIt;
     if(imgCodeToActualCodeMap[val.Id1()] == imgCodeToActualCodeMap[val.Id2()])
     {
     totalCli++;
     }
     else
     {
     totalImp++;
     }
     }

     UIntT numCli = 0;
     UIntT numImp = 0;

     RealT impostorssofar=0.0;
     DListC<Tuple2C<RealT, RealT> >data;

     for(ConstDLIterC<ClaimT>it(claims);it;it++) {

     if(it.Data().Data1()) clientssofar+=1.0;
     else impostorssofar+=1.0;

     RealT clientsrejected = clients - clientssofar;
     RealT impostorsaccepted = impostorssofar;

     //: the clever bit i hope
     RealT fa, fr;
     if(impostors>0) fa = impostorsaccepted/impostors;
     else fa = 0.0;
     if(clients>0) fr = clientsrejected/clients;
     else fr = 0.0;
     //: update date structure
     Tuple2C<RealT, RealT>elem(fa, fr);
     if(fa < maxFa && fr < maxFr)  data.InsLast(elem);
     }
     return data;

     FilenameC nn = "/tmp/graph.png";
     GnuPlotC plot(1, nn);
     for(DLIterC<Tuple2C<RealT, RealT> >it(points);it;it++) {
     plot.AddPoint(0, it.Data().Data1(), it.Data().Data2());
     }
     plot.Terminal("png");
     plot.Title("ROC Curve");
     plot.Xlabel("FA");
     plot.Ylabel("FR");
     plot.Xlo(0.0);
     plot.Xhi(maxFa);
     plot.Ylo(0.0);
     plot.Yhi(maxFr);
     return plot;

     }*/

//------------------------------------------------------------------------------
    void ScoresTableC::Print(const StringC &TableId)
    {
      unsigned int tableIdCode;
      if (!tableIds.MapForward(TableId, tableIdCode, false))
        return;

      Sort(tableIdCode, SortByIds);

      unsigned int numTotal = scores[tableIdCode].size();
      for (unsigned int i = 0; i < numTotal; i++) {
        const ScoreRecordC& val = scores[tableIdCode][i];
        printf("%i  %i %s   %i %s   %g\n",
            i,
            val.Id1(),
            imgIds.Backward(val.Id1()).chars(),
            val.Id2(),
            imgIds.Backward(val.Id2()).chars(),
            val.Score());
      }
    }
//------------------------------------------------------------------------------
    int ScoresTableC::GetMaxSize()
    {
      int retVal = 0;
      int numTables = tableIds.Size();
      for (int i = 0; i < numTables; i++) {
        int curSize = scores[i].size();
        if (curSize > retVal)
          retVal = curSize;
      }
      return retVal;
    }
//------------------------------------------------------------------------------
    void ScoresTableC::ReserveMemory(const StringC &TableId, unsigned int Size)
    {
      unsigned int tableIdCode;
      if (!tableIds.MapForward(TableId, tableIdCode, false)) {
        tableIdCode = AddTable(TableId);
      }
      //printf("reserving in %s (%i)  %i elements\n", TableId.c_str(), tableId, Size);
      scores[tableIdCode].reserve(Size);
      //printf("new capacity %i\n", scores[tableId].capacity());
    }
//------------------------------------------------------------------------------
    unsigned int ScoresTableC::AddTable(const StringC &TableId)
    {
      //printf("Adding table %s\n", TableId.c_str());
      unsigned int tableIdCode;
      if (tableIds.MapForward(TableId, tableIdCode, false))
        return tableIdCode;

      //add table
      tableIds.MapForward(TableId, tableIdCode, true);

      if (scores.size() == scores.capacity()) //make sure capacities preserved
          {
        //save capacities of scores
        std::vector<unsigned int> capacities(scores.size());
        for (int i = scores.size() - 1; i >= 0; i--)
          capacities[i] = scores[i].capacity();
        scores.push_back(std::vector<ScoreRecordC>());
        for (int i = capacities.size() - 1; i >= 0; i--)
          scores[i].reserve(capacities[i]);
      } else {
        scores.push_back(std::vector<ScoreRecordC>());
      }

      sortedType.push_back(Unsorted);
      statValid.push_back(false);
      numCli.push_back(0);
      numImp.push_back(0);
      cli.push_back(std::vector<unsigned int>(0));
      imp.push_back(std::vector<unsigned int>(0));
      //printf("New id %i\n", tableId);

      return tableIdCode;
    }
//------------------------------------------------------------------------------
    void ScoresTableC::NormTable(const StringC &TableId, double Mean, double Variance)
    {
      unsigned int tableIdCode;
      if (!tableIds.MapForward(TableId, tableIdCode, false)) {
        return;
      }

      std::vector<ScoreRecordC> &scTbl = scores[tableIdCode];

      long double mean = 0., var = 0.;
      long double max = -1e99, min = 1e99;
      for (std::vector<ScoreRecordC>::iterator it = scTbl.begin(); it != scTbl.end(); it++) {
        const long double curScore = it->Score();
        if (curScore > max)
          max = curScore;
        else if (curScore < min)
          min = curScore;
        mean += curScore;
        var += curScore * curScore;
      }
      mean /= scTbl.size();
      var = sqrt(var / scTbl.size() - mean * mean);
      printf("Table:%s  mean:%g  var:%g  max:%g  min:%g\n", TableId.chars(), float(mean), float(var), float(max), float(min));
      //const float norm = Variance / var;
      const float norm = (float) (max != min ? 2. * Variance / (max - min) : 1.);

      for (std::vector<ScoreRecordC>::iterator it = scTbl.begin(); it != scTbl.end(); it++) {
        //it->score = (it->score - mean) * norm + Mean;
        it->Score() = float((it->Score() - min) * norm - Variance);
      }
    }

//------------------------------------------------------------------------------
    UIntT ScoresTableC::GetImgCode(const StringC& ImgId)
    {
      unsigned int code;
      imgIds.MapForward(ImgId, code, true);
      return code;
    }

//------------------------------------------------------------------------------
    void ScoresTableC::Save(const StringC& Filename) const
    {
      BinOStreamC strm(Filename);
      if (!strm.Stream().good())
        throw ExceptionC("Can't save score table to " + Filename);
    }

//------------------------------------------------------------------------------
    void ScoresTableC::Save(BinOStreamC &Stream) const
    {
      //save actual ids
      UIntT numActualIds = actualIds.Size();
      Stream << numActualIds;
      for (UIntT i = 0; i < numActualIds; i++)
        Stream << actualIds.Backward(i);

      //save image ids
      UIntT numImgIds = imgIds.Size();
      Stream << numImgIds;
      for (UIntT i = 0; i < numImgIds; i++)
        Stream << imgIds.Backward(i);

      //save table ids
      UIntT numTableIds = tableIds.Size();
      Stream << numTableIds;
      for (UIntT i = 0; i < numTableIds; i++)
        Stream << tableIds.Backward(i);

      //save img to actual codes maps
      UIntT numImgToActualMaps = (UIntT) imgCodeToActualCodeMap.size();
      Stream << numImgToActualMaps;
      for (UIntT i = 0; i < numImgToActualMaps; i++)
        Stream << imgCodeToActualCodeMap[i];

      //Save score records
      Stream << (UIntT) scores.size();
      for (std::vector<std::vector<ScoreRecordC> >::const_iterator scoresIt1 = scores.begin(); scoresIt1 != scores.begin(); scoresIt1++) {
        const std::vector<ScoreRecordC>& scores_local = *scoresIt1;
        Stream << (UIntT) scores_local.size();
        for (std::vector<ScoreRecordC>::const_iterator scoresIt2 = scores_local.begin(); scoresIt2 != scores_local.end(); scoresIt2++) {
          const ScoreRecordC& rec = *scoresIt2;
          Stream << rec.Id1() << rec.Id2() << rec.Score();
        }
      }
    }

//------------------------------------------------------------------------------
    void ScoresTableC::Load(const StringC& Filename)
    {
      BinIStreamC strm(Filename);
      if (!strm.Stream().good())
        throw ExceptionC("Can't load score table from " + Filename);
    }

//------------------------------------------------------------------------------
    void ScoresTableC::Load(BinIStreamC &Stream)
    {
      //load actual ids
      UIntT numActualIds;
      Stream >> numActualIds;
      actualIds = TwoWayMapC<StringC>();
      for (UIntT i = 0; i < numActualIds; i++) {
        StringC id;
        Stream >> id;
        if (i != actualIds.Forward(id))
          throw ExceptionOperationFailedC("ScoresTableC::Load:error reading actual ids");
      }

      //load image ids
      UIntT numImgIds;
      Stream >> numImgIds;
      imgIds = TwoWayMapC<StringC>();
      for (UIntT i = 0; i < numImgIds; i++) {
        StringC id;
        Stream >> id;
        if (i != imgIds.Forward(id))
          throw ExceptionOperationFailedC("ScoresTableC::Load:error reading image ids");
      }

      //load table ids
      UIntT numTableIds;
      Stream >> numTableIds;
      tableIds = TwoWayMapC<StringC>();
      for (UIntT i = 0; i < numTableIds; i++) {
        StringC id;
        Stream >> id;
        if (i != tableIds.Forward(id))
          throw ExceptionOperationFailedC("ScoresTableC::Load:error reading table ids");
      }

      //load img to actual codes maps
      UIntT numImgToActualMaps;
      Stream >> numImgToActualMaps;
      imgCodeToActualCodeMap.clear();
      imgCodeToActualCodeMap.resize(numImgToActualMaps, 0);
      for (UIntT i = 0; i < numImgToActualMaps; i++)
        Stream >> imgCodeToActualCodeMap[i];

      //Load score records
      UIntT numTables;
      Stream >> numTables;
      scores.clear();
      scores.resize(numTables);
      for (std::vector<std::vector<ScoreRecordC> >::iterator scoresIt1 = scores.begin(); scoresIt1 != scores.begin(); scoresIt1++) {
        UIntT tableSize;
        Stream >> tableSize;
        std::vector<ScoreRecordC>& scores_local = *scoresIt1;
        scores_local.resize(tableSize);
        for (std::vector<ScoreRecordC>::iterator scoresIt2 = scores_local.begin(); scoresIt2 != scores_local.end(); scoresIt2++) {
          UIntT id1, id2;
          float score;
          Stream >> id1 >> id2 >> score;
          *scoresIt2 = ScoreRecordC(id1, id2, score);
        }
      }
    }

//------------------------------------------------------------------------------
    void ScoresTableC::PrepareStatArrays(unsigned int TableIdCode)
    {
      if (statValid[TableIdCode])
        return;

      Sort(TableIdCode, SortByScores);

      std::vector<unsigned int> &cli_local = cli[TableIdCode];
      std::vector<unsigned int> &imp_local = imp[TableIdCode];
      std::vector<ScoreRecordC> &scores_local = scores[TableIdCode];

      cli_local.clear();
      imp_local.clear();

      cli_local.resize(scores_local.size(), 0);
      imp_local.resize(scores_local.size(), 0);

      unsigned int curNumCli = 0, curNumImp = 0;

      std::vector<ScoreRecordC>::const_iterator scoresIt = scores_local.begin();
      std::vector<unsigned int>::iterator cliIt = cli_local.begin();
      std::vector<unsigned int>::iterator impIt = imp_local.begin();
      for (; scoresIt != scores_local.end() && cliIt != cli_local.end() && impIt != imp_local.end(); scoresIt++, cliIt++, impIt++) {
        const ScoreRecordC& val = *scoresIt;
        if (imgCodeToActualCodeMap[val.Id1()] == imgCodeToActualCodeMap[val.Id2()]) {
          curNumCli++;
        } else {
          curNumImp++;
        }
        *cliIt = curNumCli;
        *impIt = curNumImp;
        //printf("%i %g %g\n", i, far[i], frr[i]);
      }
      numImp[TableIdCode] = curNumImp;
      numCli[TableIdCode] = curNumCli;
      statValid[TableIdCode] = true;
    }
//------------------------------------------------------------------------------

  }
}
