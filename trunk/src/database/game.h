#include <qstring.h>
class Game
{
public:
  Game();
  void addTag(const QString & s);
  void addInitialComment(const QString & s);
  void addComment(const QString & s);
  void addMove(const QString & s);
  void addResult(const QString & s);
  void addNag(const QString & s); // $xxx
  bool validateMoveNumber(const QString & s);
  void enterVariation();
  void exitVariation();
  void addAnnotationSymbol(const QString & s);//new ! ? = += etc.
private:
  static const bool debug = true; // Ejner - just for testing
};
