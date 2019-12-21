#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <fstream>
#include <queue>

using namespace std;


class Cycle
{
  public:
    Cycle() { Size = 0; Current = -1; };
    ~Cycle() {};
    void PushBack(int x)
    {
      Values[Size] = x;
      ++Size;
    };
    int next()
    {
      Current += 1;
      return Values[Current % 646022];
    };
    void Read(string filename)
    {
      ifstream infile;
      infile.open(filename);
      short temp;
      int count = 0;
      while(!infile.fail())
      {
        infile >> temp;
        Values[count] = temp;
        count++;
      }
    };
    void Restart() { Current = -1; };
    int GetSize() { return Size; };
    int GetCurrent() { return Current; };
  private:
    int Size;
    short Values [646022];
    int Current;
};

//quick prime generator to make our wheels....

class Prime
{
  private:
    vector<long> Primes;
  public:
    Prime(int num)
    {
      Primes.push_back(2);
      Primes.push_back(3);
      int candidate = 3;
      bool flag;
      while ( Primes.size() < num )
      {
        candidate += 2;
        flag = true;
        for (int i = 0; i < Primes.size(); i++)
        {
          if (candidate % Primes[i] == 0)
          {
            flag = false;
            break;
          }
        }
        if (flag) Primes.push_back(candidate);
      }
    };
    ~Prime() {};
    int GetPrimes(int index) { return Primes[index]; };
};

class Wheel
{
  public:
    Wheel(int size)
    {
      vector<long> primes;
      Prime obj(700000);
      int check = 1;
      Size = size;
      for (int i = 0; i < 700000; i++)
      {
        primes.push_back(obj.GetPrimes(i));
        if(i < size) check *= obj.GetPrimes(i);
      }
      int sum = 0;
      int ind = size;
      while ( sum < check )
      {
        int dif = primes[ind+1] - primes[ind];
        sum += dif; ind++;
        Values.push_back(dif);
      }
    };
    ~Wheel(){};
    void GetValues()
    {
      for (int i = 0; i < Values.size(); i++)
      {
        cout << Values[i] << " ";
      }
      cout << endl;
    };
    int GetV(int ind) { return Values[ind]; };
    int GetSz() { return Values.size(); };
    void Store(string name = "yes")
    {
      ofstream outfile;
      if (name.compare("yes") == 0)
      {
        name = "Wheel";
        name.append(to_string(Size));
        name.append(".txt");
      }
      outfile.open(name);
      for (int i = 0; i < Values.size(); i++)
      {
        string temp = to_string(Values[i]);
        temp.append(" ");
        if (i % 20 == 0) outfile << endl;
        outfile << temp;
      }
      outfile.close();
    }
  private:
    vector<int> Values;
    int Size;
};

class Stepper
{
  private:
    Cycle wheel;
    unsigned long Prime;
    short Step;
    unsigned long Candidate;
  public:
    Stepper(int start, unsigned long prime)
    {
      wheel.Read("Wheel8.txt");
      Step = 0;
      Prime = prime;
      for (int i = 0; i < start-1; i++) wheel.next();
      Step = wheel.next();
    };
    ~Stepper(){};
    void Update()
    {
      Step = wheel.next();
      Candidate += Step * Prime;
    };
    unsigned long GetPrime() { return Prime; };
    short GetStep() { return Step; };
    unsigned long long GetCandidate() { return Candidate; };
};
//sieve seems to work, just ended up being too complex to be fast... I didn't realize what was actually going on inside with the original wheel sieve until
// I had to write it myself..
// the candidate prediction method is the reason it fails I think, and also the reason it's not terribly fast..
//going to create a different next method that addresses this issue and actually uses the sieve of eratosthenes..
// This fails for sure at a certain point pretty deep into the primes
class Sieve
{
  public:
    Sieve(int num)
    {
      Num = 4 * pow(num, .5);
      wheel.Read("Wheel8.txt"); //makes our 2-3-5-7-11-13-17-19 wheel.
      Cand = 23;
      Current = 19;
      Size = 0;
    };
    ~Sieve(){};
    unsigned long long next()
    {
      while(1)
      {
        long count = 0;
        Cand += wheel.next();
        int ind;
        if (Search(Cand, ind))
        {
          Predictions[ind].Update();
        }
        else if (Cand < Current * Current)
        {
          if (Cand < Num) p.push(Cand);
          return Cand;
        }
        else
        {
          Current = p.front();
          Stepper obj(count, Current);
          Predictions.push_back(obj);
          p.pop();
          count += 1;
        }
      }
    };
    bool Search(unsigned long long cand, int &ind)
    {
      for (int i = 0; i < Size; i++)
      {
        if (cand == Predictions[i].GetCandidate())
        {
          ind = i;
          return true;
        }
      }
      return false;
    }

    unsigned long long ErNext()
    {
     return 0;
    };
  private:
    long Num;
    Cycle wheel;
    long Size;
    vector<Stepper> Predictions;
    unsigned long long Current;
    unsigned long long Cand;
    queue<unsigned long long> p;

};


class Era
{
  public:
    Era() {};
    ~Era(){};

    unsigned long long next()
    {
      count = 1;
      count1 = 0;
      bool gen = true;
      Max = floor(pow(count*2000000, .5)) + 1;
      n = 2;
      while (n < Max)
      {
        for (int i = 0; i < 1000000; i += n)
        {
          //field[i] = false;
        }
      }
      return 0;
    };
  private:
    int count;
    int count1;
    int ind;
    bool gen;
    int n;
    int Max;
};

bool is_prime(long number)
{
  int current = 2;
  long num = number;
  while (current < floor(pow(num, .5)) + 1)
  {
    if (num % current == 0)
    {
      num = num / current;
      current = 2;
    }
    current += 1;
  }
  if (num == number) return true;
  return false;
}

// wheeled sieve of eratosthenes. it works and it's pretty damn fast.. less than a second to generate a file with all primes below one million
// ~2s for n = 100 000 0000 :)
void Primes(int n, string filename)
{
  Cycle wheel;
  wheel.Read("Wheel8.txt");
  ofstream outfile;
  outfile.open(filename);
  outfile << 2 << " " << 3 << " " << 5 << " " << 7 << " ";
  bool* prime = new bool[n];
  int step = 6;
  int max = floor(pow(n, .5)) + 1;
  int current = step*step / 4;
  while (step < 23*2)
  {
    //cout << "current: " << current << " step: " << step << endl;
    prime[(current-9)/2] = true;;
    current += step;
    if (current >= n)
    {
      step += 4;
      current = step * step /4;
    }
  }
  while (step < max*2)
  {
    prime[(current-9)/2] = true;;
    current += step;
    if (current >= n)
    {
      step += 2*wheel.next();
      current = step*step /4 ;
    }
  }

  int count = 0;
  int i = 0;
  while (2*i + 9 < n)
  {
    i++;
    if (!prime[i])
    {
      if(count == 20)
      {
        outfile << endl;
        count = 0;
      }
      if(2*i + 9 < n)
      {
        outfile << 2*i + 9 << " ";
        count ++;
      }
    }
  }
}

int main()
{



  //Primes(100000000, "test");
  ifstream infile;
  infile.open("newdoc");
  int temp;
  // while (!infile.fail())
  // {
  //   infile >> temp;
  //   if(!is_prime(temp))
  //   {
  //     cout << "fail: " << temp << endl;
  //   }
  // }

  cout << is_prime(99996679) << endl;

  return 0;
}












//yop
