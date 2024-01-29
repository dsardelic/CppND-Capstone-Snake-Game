#ifndef LOCATION_H
#define LOCATION_H

struct Location {
  Location();
  Location(unsigned short, unsigned short);
  bool operator==(const Location&) const;
  bool operator!=(const Location&) const;
  bool operator<(const Location&) const;
  unsigned short ManhattanDistance(const Location&) const;

  unsigned short x, y;
};

#endif
