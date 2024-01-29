#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <istream>  // std::istream
#include <ostream>  // std::ostream
#include <string>   // std::string
#include <vector>   // std::vector

struct HighScore {
  friend std::istream& operator>>(std::istream&, HighScore&);
  friend std::ostream& operator<<(std::ostream&, const HighScore&);
  bool operator<(const HighScore&) const;
  bool operator>(const HighScore&) const;

  std::string name;
  unsigned short score;
};

class HighScores {
 public:
  HighScores(std::string);
  bool IsNewHighScore(unsigned short) const;
  void Save(std::string, unsigned short);
  void Print() const;

 private:
  std::vector<HighScore> high_scores_;
  void Add(std::string, unsigned short);
  void WriteToFile(std::string) const;
};

#endif
