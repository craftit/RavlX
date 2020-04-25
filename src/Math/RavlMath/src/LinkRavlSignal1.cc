
namespace RavlN {
  void LinkRealCepstral();
  void LinkPowerSpectrum1d();

  void LinkRavlSignal1() {
    LinkRealCepstral();
    LinkPowerSpectrum1d();
  }
}
