/* { dg-additional-options "-O3" }  */

/* This test is reduced from a larger example that used to give a
   "Memory access fault" on AMD GCN due to creating too many threads.  */

int main() {
  char Ac_0;
  short As[2871];
  int Ai[2871];
  int Ci_0;
  int Bi_0;
  int Ai_0;
  long All[2871];
  long Cll_0;
  long Bll_0;
  long All_0;
  float Af_0;
  double Cd_0;
  double Bd_0;
  double Ad_0;
  short Rs7, Rs10;
  int Ri1, Ri2, Ri5, Ri6, Ri7, Ri10;
  long Rll1, Rll2, Rll5, Rll6, Rll7, Rll10;
  float Rf1;
  double Rd2;

  int i = 0;
  for (int threads = 0; threads < 512; threads++)
    {
#pragma omp target teams num_teams(1) thread_limit(1024)
#pragma omp parallel if (threads > 1) num_threads(threads)
      {
	Rs7 |= 1 < 0;
	Rs10 = Rs10 || As[i] > 0;
	Ri1 += Ai_0 + 0;
	Ri2 += Ai_0 + (Bi_0 + Ci_0);
	Ri5 *= i % 1000 == 0 ? 2 : 1;
	Ri6 &= 0;
	Ri7 |= 1 < 0;
	Ri10 = Ri10 || Ai[i] > 0;
	Rll1 += 0;
	Rll2 += All_0 + (Bll_0 + Cll_0);
	Rll5 *= i % 1000 == 0 ? : 1;
	Rll6 &= 0;
	Rll7 |= 1ll < 0;
	Rll10 = Rll10 || All[i] > 0;
	Rf1 += Af_0 + 0;
	Rd2 += Ad_0 + (Bd_0 + Cd_0);
      }
    }
  return 0;
}
