#include "highscore.h"

#include <algorithm>   // std::sort, std::min_element
#include <filesystem>  // std::filesystem
#include <fstream>     // std::istream, std::ostream
#include <iomanip>     // std::setw, std::setfill
#include <iostream>    // std::cout, std:cerr
#include <sstream>     // std::stringstream
#include <string>      // std::string
#include <utility>     // std::move

#include "constants.h"  // kMaxHighScoresCount

std::istream& operator>>(std::istream& is, HighScore& r) {
  std::string name;
  getline(is, name, '|');
  r.name = name;
  std::string score;
  getline(is, score, '|');
  r.score = std::stoi(score);
  return is;
}
std::ostream& operator<<(std::ostream& os, const HighScore& r) {
  return os << r.name << "|" << r.score;
}

bool HighScore::operator<(const HighScore& rhs) const {
  return score < rhs.score;
}

bool HighScore::operator>(const HighScore& rhs) const {
  return score > rhs.score;
}

void HighScores::Add(std::string name, int score) {
  std::size_t i{0};
  for (; i < high_scores_.size(); ++i) {
    if (score > high_scores_.at(i).score) break;
  }
  if (i == high_scores_.size()) {
    high_scores_.push_back(HighScore{name, score});
  } else {
    high_scores_.insert(high_scores_.begin() + i, HighScore{name, score});
  }
  high_scores_.resize(Constants::kMaxHighScoresCount);
}

void HighScores::WriteToFile(std::string uri) const {
  std::ofstream ofile{uri};
  if (ofile.is_open()) {
    for (const auto& hs : high_scores_) {
      if (hs.score > 0) {
        ofile << hs << std::endl;
      } else {
        break;
      }
    }
  } else {
    std::cerr << "Error: could not write to file " << uri << std::endl;
  }
}

HighScores::HighScores(std::string uri) {
  if (!std::filesystem::exists(uri)) {
    // create empty file
    std::ofstream ofile{uri};
  }
  std::ifstream ifile{uri};
  if (ifile.is_open()) {
    std::string line;
    while (getline(ifile, line)) {
      std::stringstream ss(line);
      HighScore record;
      ss >> record;
      high_scores_.push_back(std::move(record));
    }
    high_scores_.resize(Constants::kMaxHighScoresCount);
    std::sort(
        high_scores_.begin(), high_scores_.end(), std::greater<HighScore>()
    );
  } else {
    std::cerr << "Error: could not read from file " << uri << std::endl;
  }
}

bool HighScores::IsNewHighScore(int score) const {
  if (score == 0) return false;
  if (high_scores_.size() < Constants::kMaxHighScoresCount) return true;
  auto min_high_score{
      std::min_element(high_scores_.begin(), high_scores_.end())->score
  };
  return score > min_high_score;
}

void HighScores::Save(std::string name, int score) {
  Add(name, score);
  WriteToFile(Constants::kHighScoresFileUri);
}

void HighScores::Print() const {
  if (high_scores_.size() > 0) {
    std::cout << "HIGH SCORES" << std::endl;
    std::size_t rank(0);
    for (const auto& hs : high_scores_) {
      ++rank;
      std::cout << std::right << std::setw(2) << std::setfill(' ') << rank;
      std::cout << ". ";
      std::cout << std::left << std::setw(11) << std::setfill(' ') << hs.name;
      std::cout << ": ";
      std::cout << std::right << std::setw(3) << std::setfill(' ') << hs.score;
      std::cout << std::endl;
    }
  }
}
