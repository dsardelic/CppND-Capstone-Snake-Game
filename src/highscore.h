#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <string>  // std::string
#include <vector>  // std::vector

struct HighScore {
  std::string name;
  int score;

  friend std::istream& operator>>(std::istream&, HighScore&);
  friend std::ostream& operator<<(std::ostream& os, const HighScore& r);
  bool operator<(const HighScore&) const;
  bool operator>(const HighScore&) const;
};

class HighScores {
 private:
  std::vector<HighScore> high_scores_;
  void Add(std::string, int);
  void WriteToFile(std::string) const;

 public:
  HighScores(std::string);
  bool IsNewHighScore(int) const;
  void Save(std::string, int);
  void Print() const;
};

#endif
