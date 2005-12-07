#include <qapplication.h>
#include <qtextstream.h>
#include "tags.h"

static QTextStream out(stdout, IO_WriteOnly);

/*
void testTags(){
QString fname = "/home/ejner/chessX/tagfiles/test";
Tags tags;
if (tags.removeTagsFile(fname))
  out << "removed " << fname << "\n";
else{
  out << "error removing " << fname << "\n";
  return;
}
if (tags.create(fname))
  out << "created " << fname << "\n";
else{
  out << "error creating " << fname << "\n";
  return;
}

tags.close();
out << "closed " << fname << "\n";


if (tags.open(fname))
  out << "opened " << fname << "\n";
else{
  out << "error opening " << fname << "\n";
  return;
}

// add some values to known tags
out << "added a tag: " << tags.add(Tags::PlayerName,"Karpov, A") << "\n";
out << "added a tag: " << tags.add(Tags::PlayerName,"Kasparov, G") << "\n";
out << "added a tag: " << tags.add(Tags::PlayerName,"Anand, W") << "\n";
out << "added a tag: " << tags.add(Tags::PlayerName,"Holst, A") << "\n";
out << "added a tag: " << tags.add(Tags::PlayerName,"Hansen, Soren Bech") << "\n";
out << "added a tag: " << tags.add(Tags::PlayerName,"Rudolf, Michal") << "\n";
out << "added a tag: " << tags.add(Tags::PlayerName,"Hoggarth, William") << "\n";
out << "added a tag: " << tags.add(Tags::PlayerName,"Hopfgartner, Heinz R") << "\n";
out << "added a tag: " << tags.add(Tags::PlayerName,"Roets, Marius") << "\n";
out << "added a tag: " << tags.add(Tags::Event,"IECG-WC") << "\n";
out << "added a tag: " << tags.add(Tags::Event,"IECF-WC") << "\n";
out << "added a tag: " << tags.add(Tags::Site,"Scheveningen") << "\n";
out << "added a tag: " << tags.add(Tags::Site,"Wijk-Aan-Zee") << "\n";
out << "added a tag: " << tags.add(Tags::Site,"Los Angeles") << "\n";

tags.close();
tags.open(fname);

out << "count tags of name type: " << tags.count(Tags::PlayerName) << "\n";
out << "count tags of event type: " << tags.count(Tags::Event) << "\n";
out << "count tags of site type: " << tags.count(Tags::Site) << "\n";


// look up by index
for (int i=0; i<Tags::Source; i++){
  for (int j=0; j<tags.count(i); j++){
    out << "Tag Name: " << tags.tagName(i) << ": value for index " << j << ": " << tags.value(i, j) << "\n";
  }
}

// search by value
out << "index for Anand =" << tags.index(Tags::PlayerName,"Anand, W") << "\n";

// remove values
tags.remove(Tags::PlayerName,2);
out << "removed index 2\n";

// match strings
QValueVector<bool> vb = tags.find(Tags::PlayerName,"Ka");
for (int i=0; i<vb.count(); i++){
  if (vb[i])
    out << "index " << i << " matches prefix Ka \n";
  else
    out << "index " << i << " does not match prefix Ka \n";
}
vb = tags.find(Tags::Site,"L");
for (int i=0; i<vb.count(); i++){
  if (vb[i])
    out << "index " << i << " matches prefix L \n";
  else
    out << "index " << i << " does not match prefix L \n";
}


//TODO add some custom tags, and extract information on them


out << "file size is " << tags.tagFileSize() << "\n";

out << "closing " << fname << "\n";
tags.close();
}
*/

void testTags2(){
// for performance testing
QString fname = "/home/ejner/chessX/tagfiles/test";
Tags tags;
if (tags.removeTagsFile(fname))
  out << "removed " << fname << "\n";
else{
  out << "error removing " << fname << "\n";
  return;
}
if (tags.create(fname))
  out << "created " << fname << "\n";
else{
  out << "error creating " << fname << "\n";
  return;
}


QString s;
// add 419.000 players
for (int i=0; i<419000; i++){
   s.setNum(i);
   tags.add(Tags::PlayerName,"Karpov, A"+s);
}
// add 118.000 events
for (int i=0; i<118000; i++){
   s.setNum(i);
   tags.add(Tags::Event,"Scheveningen"+s);
}
// add 131.000 sites
for (int i=0; i<131000; i++){
   s.setNum(i);
   tags.add(Tags::Site,"Wijk-Aan-Zee"+s);
}
// add 1.400 rounds
for (int i=0; i<1400; i++){
   s.setNum(i);
   tags.add(Tags::Round,"1."+s);
}

out << "count tags of name type: " << tags.count(Tags::PlayerName) << "\n";
out << "count tags of event type: " << tags.count(Tags::Event) << "\n";
out << "count tags of site type: " << tags.count(Tags::Site) << "\n";
out << "count tags of round type: " << tags.count(Tags::Round) << "\n";

/*
// look up by index
for (int i=0; i<Tags::Source; i++){
  for (int j=0; j<tags.count(i); j++){
    out << "Tag Name: " << tags.tagName(i) << ": value for index " << j << ": " << tags.value(i, j) << "\n";
  }
}

// search by value
out << "index for Anand =" << tags.index(Tags::PlayerName,"Anand, W") << "\n";

// remove values
tags.remove(Tags::PlayerName,2);
out << "removed index 2\n";

// match strings
QValueVector<bool> vb = tags.find(Tags::PlayerName,"Ka");
for (int i=0; i<vb.count(); i++){
  if (vb[i])
    out << "index " << i << " matches prefix Ka \n";
  else
    out << "index " << i << " does not match prefix Ka \n";
}
vb = tags.find(Tags::Site,"L");
for (int i=0; i<vb.count(); i++){
  if (vb[i])
    out << "index " << i << " matches prefix L \n";
  else
    out << "index " << i << " does not match prefix L \n";
}
*/

out << "file size is " << tags.tagFileSize() << "\n";

// close and open to see performance
tags.close();
out << "closed " << fname << "\n";
tags.open(fname);
out << "opened " << fname << "\n";

out << "closing " << fname << "\n";
tags.close();
}



int main(int argv, char* argc[]) 
{
  QApplication app(argv, argc);
//  testTags();
  testTags2();
  return 0;
}


