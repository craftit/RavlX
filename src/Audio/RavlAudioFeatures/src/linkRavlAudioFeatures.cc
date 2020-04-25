
namespace RavlAudioN {


  void LinkVectorDelta12();
  void LinkPreEmphasis();
  void LinkMelCepstrum();
  void LinkMelSpectrum();


  void LinkRavlAudioFeatures()
  {
    LinkPreEmphasis();
    LinkVectorDelta12();
    LinkMelCepstrum();
    LinkMelSpectrum();
  }

}
