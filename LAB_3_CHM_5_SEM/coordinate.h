class Coordinate {
private:
    double x_coord;
public:
    double getX() const { return x_coord; }
    Coordinate(double x) : x_coord(x) {}
};