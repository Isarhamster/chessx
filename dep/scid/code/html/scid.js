/* =============================================================
		Javascript for HTML export of Scid
		http://prolinux.free.fr/scid
		(c) 2007 - Pascal Georges 
		Feel free to send comments/enhancements
		
		Modified 2009 Charly Founes - cf29.com
   ============================================================= */
// Globals
var oldColor      = "";
var highlightMove = "";
var turned = 0;

/* ------ handlekey ---------- */
function handlekey(e) {
	var keycode
	if ( !e ) {
	  return;
	}
	if ( e.keyCode ) {
	  keycode = e.keyCode;
	}
	else if ( e.which ) {
	  keycode = e.which;
	}
	else {
	  return;
	}
	if (keycode == 37)
		moveForward(0);
	else if (keycode == 39)
		moveForward(1);
}

/* ------ doinit ---------- */
function doinit() {
    var crossrule;
    
	initFen(movesArray[0],1);

    // Get colors for move highlighting from style sheet
    //
    if ( document.styleSheets[0].cssRules ) {
        crossrule = document.styleSheets[0].cssRules;
    }
    else if ( document.styleSheets[0].rules ) {
        crossrule = document.styleSheets[0].rules;
    }
   
    highlightMove = crossrule[0].style.color;
    oldColor      = crossrule[1].style.color;
}

/* ------ rotate ---------- */
function rotate() {
	if (turned == 0) turned=1; else turned=0;
	initFen(movesArray[current],1);
}

/* ------ moveForward ----------- */
// if dir = 0, move back
function moveForward(dir) {
	var s = new String(movesArray[current]);
	s = s.substring(s.indexOf(" ")+1);
	a = Number( s.substring( 0, s.indexOf(" ") ) );
	b = Number( s.substring( s.indexOf(" ")+1 ) );
	if (dir == 1) 
		gotoMove(b);
	else
		gotoMove(a);
}

/* ------ jump --------------- */
// depending on the parameter will go to start 
// or end of the game
function jump(x) {
	var oldcurrent = current;
	var oldcurrentDiv = "mv";
	var currentDiv = "mv";
	
	if (current != 0) {
		oldcurrentDiv += oldcurrent;
		document.getElementById(oldcurrentDiv).style.background = oldColor;	
	}
	
	if (x==0) { // goto start
		current = 0;
	}
	if (x==1) { // goto end
		current = movesArray.length-1;
		currentDiv += current;
		document.getElementById(currentDiv).style.background = highlightMove;
	}
	refresh(movesArray[current]);
}

/* ------ gotoMove ----------- */
function gotoMove(cur) {

	if (cur <0 || cur >= movesArray.length) return;
	
	var oldcurrent = current;
	current = cur;
	var oldcurrentDiv = "mv" + oldcurrent; // conflict with same id in diagram
	var currentDiv = "mv" + current; // id isn't supposed to start with a number
	if (oldcurrent != 0) {
		document.getElementById(oldcurrentDiv).style.background = oldColor;	
	}
	if (current != 0) {
		document.getElementById(currentDiv).style.background = highlightMove;
	}
	refresh(movesArray[current]);
}

/* ------ refresh ----------- */
function refresh(fen) {
	var s = new String(fen);
	s = trimfen(s);
	if (turned) s = inverse(s);
	var square = 0;
	for (i=0; i<s.length; i++) {
		var c = s.charAt(i);
		if ( c >= '1' && c <= '8' ) {
			for (j=0; j<c; j++) {
document.getElementById(square).src = "bitmaps/" + piece2gif(" ") + ".gif" ;
				square++;
			}
		} else if (c != '/') {
document.getElementById(square).src = "bitmaps/" + piece2gif(c) + ".gif" ;
			square++;
		}
	}
}

/* ------ inverse ---------- */
function inverse(s) {
	var t = new String("");
	for (i=s.length-1; i>=0; i--) {
		t += s.charAt(i);
	}
	return t;
}
/* ------ trimfen ---------- */
function trimfen(fen) {
	return fen.substring(0, fen.indexOf(" ", 0));
}
/* ------ colorSq ---------- */
function colorSq(sq) {
	if ( (sq%2) == 1 && Math.floor(sq/8) %2 == 0 || (sq%2) == 0 && Math.floor(sq/8) %2 == 1 ) {
		return "bs";
	} else { 
		return "ws";
	}
}

/* ------ initFen ---------- */
function initFen(fen, force) {
	var s = new String(fen);
	s = trimfen(s);
	var html = "";
	var square = 0;
	var ccol = new String("abcdefgh");
	var drow = 0;
	
	if (turned) s = inverse(s);
	
	var diagramContent = "";
	
	diagramContent += "<table id='diagramTable'>";
	var row = 8;
	if (turned) row = 1;
	
	diagramContent += "</tr><tr><td class='coordinates'>&nbsp;</td>";
	for (i = 0; i < 8; i++) {
		if (turned)
			diagramContent += "<td class='coordinates'>"+ccol.charAt(7-i)+"</td>";
		else
			diagramContent += "<td class='coordinates'>"+ccol.charAt(i)+"</td>";

	}
	diagramContent += "<td class='coordinates'>&nbsp;</td></tr>";
	
	diagramContent += "<tr><td class='coordinates'>"+row+"</td>";
	if (turned)	drow=1; else drow=-1;
	row += drow;
	for (i=0; i<s.length; i++) {
		var c = s.charAt(i);
		if ( c >= '1' && c <= '8' ) {
			for (j=0; j<c; j++) {
				html = "<td class=\"" + colorSq(square) + "\"><img width=\"40\" height=\"40\" src=bitmaps/" + piece2gif(" ") + ".gif id=\"" + square + "\" /></td>";
				diagramContent += html;
				square++;
			}
		} else if (c == '/') {
			diagramContent += "<td class='coordinates'>"+(row-drow)+"</td></tr><tr><td class='coordinates'>"+row+"</td>";
		row += drow;
		} else {
			html = "<td class=\"" + colorSq(square) + "\"><img width=\"40\" height=\"40\" src=bitmaps/" + piece2gif(c) + ".gif id=\"" + square + "\" /></td>";
			diagramContent += html;
			square++;
		}
	}
	
	diagramContent += "<td class='coordinates'>"+(row-drow)+"</td></tr><tr><td class='coordinates'></td>";
	for (i = 0; i < 8; i++) {
		if (turned)
			diagramContent += "<td class='coordinates'>"+ccol.charAt(7-i)+"</td>";
		else
			diagramContent += "<td class='coordinates'>"+ccol.charAt(i)+"</td>";

	}
	diagramContent += "<td class='coordinates'>&nbsp;</td>";

	diagramContent += "</tr></table>";
	document.getElementById("diagram").innerHTML = diagramContent;
}
/* ------  gotogame --------- */
function gotogame() {
	current = 0;
	var i = document.getElementById("gameselect").selectedIndex +1;
location.href = prefix+"_"+i+".html";
	refresh(movesArray[current]);

}

/* ------  gotoprevgame --------- */
function gotoprevgame() {
	var i = document.getElementById("gameselect").selectedIndex;
	if (i>0) {
		document.getElementById("gameselect").selectedIndex = i-1;
		gotogame();
	}
}

/* ------  gotonextgame --------- */
function gotonextgame() {
	var i = document.getElementById("gameselect").selectedIndex;
	if ( i < document.getElementById("gameselect").length - 1) {
		document.getElementById("gameselect").selectedIndex = i+1;
		gotogame();
	}
}

/* ------  piece2gif --------- */
function piece2gif(piece) {
		if (piece == "K") return "wk"; 
		if (piece == "k") return "bk"; 
		if (piece == "Q") return "wq"; 
		if (piece == "q") return "bq"; 
		if (piece == "R") return "wr"; 
		if (piece == "r") return "br"; 
		if (piece == "B") return "wb"; 
		if (piece == "b") return "bb"; 
		if (piece == "N") return "wn"; 
		if (piece == "n") return "bn"; 
		if (piece == "P") return "wp";  
		if (piece == "p") return "bp";
		if (piece == " ") return "sq";		 
}
